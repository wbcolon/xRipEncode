/*
 * This file is part of xRipEncode.
 *
 * xRipEncode is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * xRipEncode is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "xArchiveFile.h"

#include <archive.h>
#include <archive_entry.h>

#include <QRegularExpression>
#include <QDebug>

// Currently supported lookup schemes.
const QStringList xArchiveFile::TagLookupSchemes { "Qobuz", "7Digital", "Bandcamp", "HighResAudio", "HDtracks" };

xArchiveFile::xArchiveFile(QObject *parent):
        QThread(parent) {
}

int xArchiveFile::getFiles() const {
    return archiveFileNames.count();
}

const QString& xArchiveFile::getFileName() const {
    return archiveFileName;
}

void xArchiveFile::analyze(const QString& file) {
    // Save file name.
    archiveFileName = file;
    // Clear file names and file sizes.
    archiveFileNames.clear();
    archiveFileSizes.clear();
    // Use libarchive to scan archive file.
    struct archive* archiveFile;
    struct archive_entry* archiveEntry;
    int result;
    // Setup libarchive to read archives.
    archiveFile = archive_read_new();
    archive_read_support_filter_all(archiveFile);
    archive_read_support_format_all(archiveFile);
    result = archive_read_open_filename(archiveFile, archiveFileName.toStdString().c_str(), 16384);
    if (result != ARCHIVE_OK) {
        emit messages(QString("[error] unable to open the archive: %1").arg(archiveFileName));
        return;
    }
    while ((result = archive_read_next_header(archiveFile, &archiveEntry)) != ARCHIVE_EOF) {
        // Clear archived file names if error in archive file.
        if (result != ARCHIVE_OK) {
            archiveFileNames.clear();
            archiveFileSizes.clear();
            emit messages(QString("[error] error reading file: %1").arg(archiveFileName));
            break;
        }
        // Do we have a regular file.
        if (archive_entry_filetype(archiveEntry) & AE_IFREG) {
            auto fileName = QString(archive_entry_pathname(archiveEntry));
            emit messages("[analyze] "+fileName);
            if (validOutputFile(fileName)) {
                archiveFileNames.push_back(fileName);
                archiveFileSizes.push_back(archive_entry_size(archiveEntry));
            }
        }
        archive_read_data_skip(archiveFile);
    }
    archive_read_close(archiveFile);
    archive_read_free(archiveFile);
    emit archivedFiles(archiveFileNames, archiveFileSizes);
}

void xArchiveFile::queueExtract(const QList<xAudioFile*>& files) {
    for (const auto& file : files) {
        // audio track nr start with index 1.
        auto trackNr = file->getAudioTrackNr() - 1;
        if ((trackNr < 0 ) || (trackNr >= archiveFileNames.count())) {
            qCritical() << "Illegal track: " << file->getAudioTrackNr();
            continue;
        }
        auto archivFileNameIndex = trackNr;
        if (!archiveFileTrackNrs.empty()) {
            // Find the index of the the track number.
            archivFileNameIndex = archiveFileTrackNrs.indexOf(archivFileNameIndex+1);
            if ((archivFileNameIndex < 0) || (archivFileNameIndex >= archiveFileNames.count())) {
                qCritical() << "Illegal track lookup: " << trackNr << "," << archivFileNameIndex;
                continue;
            }
        }
        queue.push_back(xArchiveFileQueue{file, archiveFileNames[archivFileNameIndex]});
        qDebug() << "Added to rip queue: " << file->getFileName();
    }
}

void xArchiveFile::run() {
    QList<xAudioFile*> files;
    // Use libarchive to scan archive file.
    struct archive* archiveFile;
    struct archive* outputFile;
    struct archive_entry* archiveEntry;
    int result;

    archiveFile = archive_read_new();
    archive_read_support_filter_all(archiveFile);
    archive_read_support_format_all(archiveFile);
    result = archive_read_open_filename(archiveFile, archiveFileName.toStdString().c_str(), 16384);
    if (result != ARCHIVE_OK) {
        qCritical() << "xArchiveFile::extract: unable to open the archive: " << archiveFileName;
        return;
    }
    outputFile = archive_write_disk_new();
    archive_write_disk_set_options(outputFile, ARCHIVE_EXTRACT_PERM|ARCHIVE_EXTRACT_ACL|ARCHIVE_EXTRACT_FFLAGS);
    archive_write_disk_set_standard_lookup(outputFile);
    while ((result = archive_read_next_header(archiveFile, &archiveEntry)) != ARCHIVE_EOF) {
        if (result != ARCHIVE_OK) {
            qCritical() << "xArchiveFile::extract: unable to read archive header: " << archive_error_string(archiveFile);
            break;
        }
        auto queueEntry = findQueueEntry(archiveEntry);
        if (queueEntry == nullptr) {
            qWarning() << "xArchiveFile::extract: skipping over: " << archive_entry_pathname(archiveEntry);
            continue;
        }
        emit messages("[extract] "+queueEntry->getFileName());
        archive_entry_set_pathname(archiveEntry, queueEntry->getFileName().toStdString().c_str());
        if (archive_write_header(outputFile, archiveEntry) != ARCHIVE_OK) {
            emit messages(QString("[error] unable to write output file header: %1").arg(archive_error_string(outputFile)));
        } else {
            result = extractOutputFile(archiveFile, outputFile);
            if (result != ARCHIVE_OK) {
                emit messages(QString("[error] unable to extract output file: %1").arg(archive_error_string(archiveFile)));
                break;
            }
            emit extractProgress(queueEntry->getAudioTrackNr(), 100);
            files.push_back(queueEntry);
        }
    }
    archive_read_close(archiveFile);
    archive_read_free(archiveFile);
    archive_write_close(outputFile);
    archive_write_free(outputFile);
    // Emit extracted audio files. Transfer to encoding view.
    emit audioFiles(files);
}

xArchiveFileTags xArchiveFile::extractTags(const QString& scheme) {
    if (scheme.compare(TagLookupSchemes[0], Qt::CaseInsensitive) == 0) {
        return extractTagsFromQobuz();
    }
    if (scheme.compare(TagLookupSchemes[1], Qt::CaseInsensitive) == 0) {
        return extractTagsFrom7Digital();
    }
    if (scheme.compare(TagLookupSchemes[2], Qt::CaseInsensitive) == 0) {
        return extractTagsFromBandcamp();
    }
    // No valid scheme found. Return an empty structure.
    return xArchiveFileTags{ "", "", {}, {}, 0 };
}

xArchiveFileTags xArchiveFile::extractTagsFromQobuz() {
    // Parse the file names in order to determine artist, album, track nr and track name.
    // Keep in mind that the names within the archive are not necessarily sorted.
    QRegularExpression qobuzRegExp(R"((?<artist>.*?)-(?<album>.*)/(?<disc>\d\d)-(?<nr>\d\d)-(?<name>.*))");
    QRegularExpressionMatch qobuzMatch;
    xArchiveFileTags qobuzTags;
    // Determine the number of (relevant) files in the archive.
    auto files = archiveFileNames.count();
    // Unordered vector of track names based on their position in the archive file.
    QVector<QString> trackName(files);
    // Qobuz uses [disc number]-[track nr] which we need to convert to an absolute track number.
    // Therefore we need to know the max number of tracks per disc.
    QVector<int> discMax(files+1, 0);
    // Index translation to get from archived file name to desired output file name.
    archiveFileTrackNrs.resize(files);
    for (auto index = 0; index < files; ++index) {
        qobuzMatch = qobuzRegExp.match(archiveFileNames[index].toLower());
        if (qobuzMatch.hasMatch()) {
            auto disc = qobuzMatch.captured("disc").toInt();
            auto nr = qobuzMatch.captured("nr").toInt();
            if (discMax[disc] < nr) {
                discMax[disc] = nr;
            }
            archiveFileTrackNrs[index] = ((disc-1) * 1000) + nr;
            qobuzTags.artist = qobuzMatch.captured("artist");
            qobuzTags.artist.replace("_", " ");
            qobuzTags.album = qobuzMatch.captured("album");
            qobuzTags.album.replace("_", " ");
            trackName[index] = qobuzMatch.captured("name");
            trackName[index].replace("_", " ");
            if (trackName[index].startsWith(qobuzTags.artist)) {
                // Remove artist in track name plus the "-".
                trackName[index].remove(0, qobuzTags.artist.size()+1);
            }
            if (trackName[index].contains("-smr.flac")) {
                // We have an HD album if track name contains "-smr".
                qobuzTags.bitsPerSample = 24;
                trackName[index].replace("-smr.flac", ".flac");
            } else if (trackName[index].contains("-lls.flac")) {
                qobuzTags.bitsPerSample = 16;
                trackName[index].replace("-lls.flac", ".flac");
            } else {
                qCritical() << "extractTagsFromQobuz: no valid quality tag: " << trackName[index];
                // Return an empty structure.
                return xArchiveFileTags{ "", "", {}, {}, 0 };
            }
        }
    }
    // Compute final track numbers for index translation.
    for (auto index = 0; index < files; ++index) {
        archiveFileTrackNrs[index] = discMax[archiveFileTrackNrs[index]/1000] + (archiveFileTrackNrs[index]%1000);
    }
    // Create return output. Order archived files according to their track number.
    qobuzTags.trackName.resize(files);
    qobuzTags.trackSize.resize(files);
    for (auto index = 0; index < files; ++index) {
        auto trackNr = archiveFileTrackNrs[index]-1;
        if ((trackNr >= 0) && (trackNr < files)) {
            qobuzTags.trackName[archiveFileTrackNrs[index]-1] = trackName[index].remove(".flac");
            qobuzTags.trackSize[archiveFileTrackNrs[index]-1] = archiveFileSizes[index];
        } else {
            qCritical() << "extractTagsFromQobuz: invalid track number: " << trackNr;
            // Return an empty structure.
            return xArchiveFileTags{ "", "", {}, {}, 0 };
        }
    }
    return qobuzTags;
}

xArchiveFileTags xArchiveFile::extractTagsFrom7Digital() {
    // Parse the file names in order to determine artist, album, track nr and track name.
    // Keep in mind that the names within the archive are not necessarily sorted.
    QRegularExpression digitalRegExp(R"((?<artist>.*)/(?<album>.*)/(.*) - (?<nr>\d\d). (?<name>.*))");
    QRegularExpressionMatch digitalMatch;
    xArchiveFileTags digitalTags;
    // Determine the number of (relevant) files in the archive.
    auto files = archiveFileNames.count();
    // Unordered vector of track names based on their position in the archive file.
    QVector<QString> trackName(files);
    // Index translation to get from archived file name to desired output file name.
    archiveFileTrackNrs.resize(files);
    for (auto index = 0; index < files; ++index) {
        digitalMatch = digitalRegExp.match(archiveFileNames[index].toLower());
        if (digitalMatch.hasMatch()) {
            archiveFileTrackNrs[index] = digitalMatch.captured("nr").toInt();
            digitalTags.artist = digitalMatch.captured("artist");
            digitalTags.album = digitalMatch.captured("album");
            trackName[index] = digitalMatch.captured("name");
            digitalTags.bitsPerSample = 16;
        }
    }
    // Create return output. Order archived files according to their track number.
    digitalTags.trackName.resize(files);
    digitalTags.trackSize.resize(files);
    for (auto index = 0; index < files; ++index) {
        auto trackNr = archiveFileTrackNrs[index]-1;
        if ((trackNr >= 0) && (trackNr < files)) {
            digitalTags.trackName[archiveFileTrackNrs[index]-1] = trackName[index].remove(".flac");
            digitalTags.trackSize[archiveFileTrackNrs[index]-1] = archiveFileSizes[index];
        } else {
            qCritical() << "extractTagsFromQobuz: invalid track number: " << trackNr;
            // Return an empty structure.
            return xArchiveFileTags{ "", "", {}, {}, 0 };
        }
    }
    return digitalTags;
}

xArchiveFileTags xArchiveFile::extractTagsFromBandcamp() {
    // Parse the file names in order to determine artist, album, track nr and track name.
    // Keep in mind that the names within the archive are not necessarily sorted.
    QRegularExpression bandcampRegExp(R"((?<artist>.*?) - (?<album>.*) - (?<nr>\d\d) (?<name>.*))");
    QRegularExpressionMatch bandcampMatch;
    xArchiveFileTags bandcampTags;
    // Determine the number of (relevant) files in the archive.
    auto files = archiveFileNames.count();
    // Unordered vector of track names based on their position in the archive file.
    QVector<QString> trackName(files);
    // Index translation to get from archived file name to desired output file name.
    archiveFileTrackNrs.resize(files);
    for (auto index = 0; index < files; ++index) {
        bandcampMatch = bandcampRegExp.match(archiveFileNames[index].toLower());
        if (bandcampMatch.hasMatch()) {
            archiveFileTrackNrs[index] = bandcampMatch.captured("nr").toInt();
            bandcampTags.artist = bandcampMatch.captured("artist");
            bandcampTags.album = bandcampMatch.captured("album");
            trackName[index] = bandcampMatch.captured("name");
            bandcampTags.bitsPerSample = 16;
        }
    }
    // Create return output. Order archived files according to their track number.
    bandcampTags.trackName.resize(files);
    bandcampTags.trackSize.resize(files);
    for (auto index = 0; index < files; ++index) {
        auto trackNr = archiveFileTrackNrs[index]-1;
        if ((trackNr >= 0) && (trackNr < files)) {
            bandcampTags.trackName[archiveFileTrackNrs[index]-1] = trackName[index].remove(".flac");
            bandcampTags.trackSize[archiveFileTrackNrs[index]-1] = archiveFileSizes[index];
        } else {
            qCritical() << "extractTagsFromQobuz: invalid track number: " << trackNr;
            // Return an empty structure.
            return xArchiveFileTags{ "", "", {}, {}, 0 };
        }
    }
    return bandcampTags;
}


xAudioFile* xArchiveFile::findQueueEntry(struct archive_entry *entry) {
    // Search queue for output file name.
    auto pathName = archive_entry_pathname(entry);
    for (const auto& queueEntry : queue) {
        // Use pathname in order to find the queue entry.
        if (queueEntry.archiveFileName.compare(pathName) == 0) {
            return queueEntry.audioFile;
        }
    }
    return nullptr;
}

bool xArchiveFile::extractOutputFile(struct archive* archiveFile, struct archive* outputFile) {
    const void* outputBuffer;
    size_t outputSize;
    int64_t outputOffset;
    int result;

    while ((result = archive_read_data_block(archiveFile, &outputBuffer, &outputSize, &outputOffset)) != ARCHIVE_EOF) {
        if (result != ARCHIVE_OK) {
            return false;
        }
        if (archive_write_data_block(outputFile, outputBuffer, outputSize, outputOffset) != ARCHIVE_OK) {
            return false;
        }
    }
    return (archive_write_finish_entry(outputFile) == ARCHIVE_EOF);
}

bool xArchiveFile::validOutputFile(const QString& fileName) {
    auto lowerFileName = fileName.toLower();
    if (lowerFileName.endsWith(".flac") || lowerFileName.endsWith(".wav")) {
        return true;
    }
    return false;
}