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

#include "xAudioFile.h"
#include "xRipEncodeConfiguration.h"
#include <QDebug>



xAudioFileEncoding::xAudioFileEncoding(const QList<std::pair<xAudioFile*,QString>>& files, bool flac, QObject* parent):
        QThread(parent),
        encodeFiles(files),
        encodeFlac(flac) {
}

void xAudioFileEncoding::run() {
    if (encodeFlac) {
        for (auto i = 0; i < encodeFiles.count(); ++i) {
            encodeFiles[i].first->encodeFlac(encodeFiles[i].second);
            emit encodingProgress(i+1, 100);
        }
    } else {
        for (auto i = 0; i < encodeFiles.count(); ++i) {
            encodeFiles[i].first->encodeWavPack(encodeFiles[i].second);
            emit encodingProgress(i+1, 100);
        }
    }
}



xAudioFile::xAudioFile():
        QObject(),
        inputFileName(),
        inputAudioTrackNr(-1),
        encodingArtist(),
        encodingAlbum(),
        encodingTrackNr(),
        encodingTrackName(),
        encodingTag(),
        encodingTagId(-1),
        jobId(0) {
}

xAudioFile::xAudioFile(const QString& fileName, int audioTrackNr, const QString& artist, const QString& album,
                       const QString& trackNr, const QString& trackName, const QString& tag, int tagId,
                       quint64 id, QObject* parent):
        QObject(parent),
        inputFileName(fileName),
        inputAudioTrackNr(audioTrackNr),
        encodingArtist(artist),
        encodingAlbum(album),
        encodingTrackNr(trackNr),
        encodingTrackName(trackName),
        encodingTag(tag),
        encodingTagId(tagId),
        jobId(id) {
}

xAudioFile::xAudioFile(const xAudioFile& copy):
        QObject(copy.parent()),
        inputFileName(copy.inputFileName),
        inputAudioTrackNr(copy.inputAudioTrackNr),
        encodingArtist(copy.encodingArtist),
        encodingAlbum(copy.encodingAlbum),
        encodingTrackNr(copy.encodingTrackNr),
        encodingTrackName(copy.encodingTrackName),
        encodingTag(copy.encodingTag),
        encodingTagId(copy.encodingTagId),
        jobId(copy.jobId) {
}

xAudioFile::~xAudioFile() noexcept {
    remove();
}

void xAudioFile::remove() {
    try {
        std::filesystem::remove(inputFileName.toStdString());
    } catch (std::filesystem::filesystem_error& e) {
        qWarning() << "Unable to audio file: " << inputFileName << ", error: " << e.what() << ", ignoring.";
    }
}

const QString& xAudioFile::getFileName() const {
    return inputFileName;
}

int xAudioFile::getAudioTrackNr() const {
    return inputAudioTrackNr;
}

void xAudioFile::setArtist(const QString& artist) {
    encodingArtist = artist;
}

const QString& xAudioFile::getArtist() const {
    return encodingArtist;
}

void xAudioFile::setAlbum(const QString& album) {
    encodingAlbum = album;
}

const QString& xAudioFile::getAlbum() const {
    return encodingAlbum;
}

void xAudioFile::setTrackNr(const QString& trackNr) {
    encodingTrackNr = trackNr;
}

const QString& xAudioFile::getTrackNr() const {
    return encodingTrackNr;
}

void xAudioFile::setTrackName(const QString& trackName) {
    encodingTrackName = trackName;
}

const QString& xAudioFile::getTrackName() const {
    return encodingTrackName;
}

void xAudioFile::setTag(const QString& tag) {
    encodingTag = tag;
}

const QString& xAudioFile::getTag() const {
    return encodingTag;
}

int xAudioFile::getTagId() const {
    return encodingTagId;
}

quint64 xAudioFile::getJobId() const {
    return jobId;
}

xAudioFileWav::xAudioFileWav():
        xAudioFile(),
        process(nullptr) {
}

xAudioFileWav::xAudioFileWav(const QString& fileName, int audioTrackNr, const QString& artist, const QString& album,
                             const QString& trackNr, const QString& trackName, const QString& tag, int tagId,
                             quint64 id, QObject *parent):
        xAudioFile(fileName, audioTrackNr, artist, album, trackNr, trackName, tag, tagId, id, parent),
        process(nullptr) {
}

xAudioFileWav::xAudioFileWav(const xAudioFileWav& copy):
        xAudioFile(copy),
        process(nullptr) {
}

bool xAudioFileWav::encodeWavPack(const QString& wavPackFileName) {
    // Create directory.
    auto wavPackFileDirectory = wavPackFileName.left(wavPackFileName.lastIndexOf('/'));
    try {
        if (wavPackFileDirectory != wavPackFileName) {
            qDebug() << "xAudioFileWav::backupWavPack: create directory: " << wavPackFileDirectory;
            std::filesystem::create_directories(wavPackFileDirectory.toStdString());
        }
    } catch (std::filesystem::filesystem_error& e) {
        // Ignore errors.
    }
    std::filesystem::path wavPackFile(wavPackFileName.toStdString());
    // Backup file.
    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(xRipEncodeConfiguration::configuration()->getWavPack(), { {"-h"}, {"-y"}, inputFileName, {"-o"}, wavPackFileName } );
    qDebug() << "xAudioFileWav::backupWavPack: process arguments: " << process->arguments();
    process->waitForFinished(-1);
    auto exitCode = process->exitCode();
    auto exitError = process->errorString();
    delete process;
    if (exitCode == QProcess::NormalExit) {
        return true;
    } else {
        qCritical() << "xAudioFileWav::encodeWavPack: error: " << exitError;
        try {
            std::filesystem::remove(wavPackFile);
        } catch (std::filesystem::filesystem_error& e) {
            qWarning() << "Unable to remove corrupt output file: " << wavPackFileName << ", error: " << e.what() << ", ignoring.";
        }
        return false;
    }
}

bool xAudioFileWav::encodeFlac(const QString& flacFileName) {
    // Create directory.
    auto flacFileDirectory = flacFileName.left(flacFileName.lastIndexOf('/'));
    try {
        if (flacFileDirectory != flacFileName) {
            qDebug() << "xAudioFileWav::encodeFlac: create directory: " << flacFileDirectory;
            std::filesystem::create_directories(flacFileDirectory.toStdString());
        }
    } catch (std::filesystem::filesystem_error& e) {
        // Ignore errors.
    }
    // Encode file.
    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(xRipEncodeConfiguration::configuration()->getFlac(), { {"-8"}, {"-f"}, inputFileName,
                                                                          {"-o"}, flacFileName, { "--tag=ARTIST="+encodingArtist }, { "--tag=ALBUM="+encodingAlbum },
                                                                          { "--tag=TRACKNUMBER="+encodingTrackNr }, { "--tag=TITLE="+encodingTrackName } });
    qDebug() << "xAudioFileWav::encodeFlac: process arguments: " << process->arguments();
    process->waitForFinished(-1);
    auto exitCode = process->exitCode();
    auto exitError = process->errorString();
    delete process;
    if (exitCode == QProcess::NormalExit) {
        return true;
    } else {
        qCritical() << "xAudioFileWav::encodeFlac: error: " << exitError;
        try {
            std::filesystem::remove(flacFileName.toStdString());
        } catch (std::filesystem::filesystem_error& e) {
            qWarning() << "Unable to remove corrupt output file: " << flacFileName << ", error: " << e.what() << ", ignoring.";
        }
        return false;
    }
}

xAudioFileFlac::xAudioFileFlac():
        xAudioFile(),
        process(nullptr) {
}

xAudioFileFlac::xAudioFileFlac(const QString& fileName, int audioTrackNr, const QString& artist, const QString& album,
                              const QString& trackNr, const QString& trackName, const QString& tag, int tagId,
                              quint64 id, QObject *parent):
        xAudioFile(fileName, audioTrackNr, artist, album, trackNr, trackName, tag, tagId, id, parent),
        process(nullptr) {
}

xAudioFileFlac::xAudioFileFlac(const xAudioFileFlac& copy):
        xAudioFile(copy),
        process(nullptr) {
}

bool xAudioFileFlac::encodeWavPack(const QString& wavPackFileName) {
    // We do not support wavpack encoding to flac files.
    Q_UNUSED(wavPackFileName)
    return false;
}

bool xAudioFileFlac::encodeFlac(const QString& flacFileName) {
    // Create directory.
    auto flacFileDirectory = flacFileName.left(flacFileName.lastIndexOf('/'));
    try {
        if (flacFileDirectory != flacFileName) {
            qDebug() << "xAudioFileFlac::encodeFlac: create directory: " << flacFileDirectory;
            std::filesystem::create_directories(flacFileDirectory.toStdString());
        }
    } catch (std::filesystem::filesystem_error& e) {
        // Ignore errors.
    }
    // Copy file.
    try {
        qCritical() << "Copy file: " << inputFileName << "to" << flacFileName;
        std::filesystem::copy(inputFileName.toStdString(), flacFileName.toStdString());
    } catch (std::filesystem::filesystem_error& e) {
        qCritical() << "Unable to copy file: " << inputFileName << "to" << flacFileName;
        return false;
    }
    // Tag the target file.
    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(xRipEncodeConfiguration::configuration()->getLLTag(), { {"--yes"},
                                                                          { "--ARTIST"}, encodingArtist, { "--ALBUM" }, encodingAlbum,
                                                                          { "--NUMBER"}, encodingTrackNr, { "--TITLE" }, encodingTrackName,
                                                                          flacFileName });
    qDebug() << "xAudioFileFlac::encodeFlac: process arguments: " << process->arguments();
    process->waitForFinished(-1);
    auto exitCode = process->exitCode();
    auto exitError = process->errorString();
    delete process;
    if (exitCode == QProcess::NormalExit) {
        return true;
    } else {
        qCritical() << "xAudioFileFlac::encodeFlac: error: " << exitError;
        try {
            std::filesystem::remove(flacFileName.toStdString());
        } catch (std::filesystem::filesystem_error& e) {
            qWarning() << "Unable to remove corrupt output file: " << flacFileName << ", error: " << e.what() << ", ignoring.";
        }
        return false;
    }
}


