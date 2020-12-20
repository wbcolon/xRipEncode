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
        process(nullptr),
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
        process(nullptr),
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
        process(nullptr),
        jobId(copy.jobId) {
}

xAudioFile::~xAudioFile() noexcept {
    remove();
}

bool xAudioFile::encodeWavPack(const QString& wavPackFileName) {
    // Create directory.
    auto wavPackFileDirectory = wavPackFileName.left(wavPackFileName.lastIndexOf('/'));
    try {
        if (wavPackFileDirectory != wavPackFileName) {
            qDebug() << "xAudioFile::backupWavPack: create directory: " << wavPackFileDirectory;
            std::filesystem::create_directories(wavPackFileDirectory.toStdString());
        }
    } catch (std::filesystem::filesystem_error& e) {
        // Ignore errors.
    }
    std::filesystem::path wavPackFile(wavPackFileName.toStdString());
    // Backup file.
    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, &QProcess::readyReadStandardOutput, this, &xAudioFile::processBackupOutput);
    process->start(xRipEncodeConfiguration::configuration()->getWavPack(), { {"-h"}, inputFileName, {"-o"}, wavPackFileName } );
    qDebug() << "xAudioFile::backupWavPack: process arguments: " << process->arguments();
    process->waitForFinished(-1);
    disconnect(process, &QProcess::readyReadStandardOutput, this, &xAudioFile::processBackupOutput);
    auto exitCode = process->exitCode();
    delete process;
    if (exitCode == QProcess::NormalExit) {
        return true;
    } else {
        try {
            std::filesystem::remove(wavPackFile);
        } catch (std::filesystem::filesystem_error& e) {
            qWarning() << "Unable to remove corrupt output file: " << wavPackFileName << ", error: " << e.what() << ", ignoring.";
        }
        return false;
    }
}

bool xAudioFile::encodeFlac(const QString& flacFileName) {
    // Create directory.
    auto flacFileDirectory = flacFileName.left(flacFileName.lastIndexOf('/'));
    try {
        if (flacFileDirectory != flacFileName) {
            qDebug() << "xAudioFile::encodeFlac: create directory: " << flacFileDirectory;
            std::filesystem::create_directories(flacFileDirectory.toStdString());
        }
    } catch (std::filesystem::filesystem_error& e) {
        // Ignore errors.
    }
    // Encode file.
    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, &QProcess::readyReadStandardOutput, this, &xAudioFile::processEncodeOutput);
    process->start(xRipEncodeConfiguration::configuration()->getFlac(), { {"-8"}, {"-f"}, inputFileName,
                   {"-o"}, flacFileName, { "--tag=ARTIST="+encodingArtist }, { "--tag=ALBUM="+encodingAlbum },
                   { "--tag=NUMBER="+encodingTrackNr }, { "--tag=TITLE="+encodingTrackName } });
    qDebug() << "xAudioFile::encodeFlac: process arguments: " << process->arguments();
    process->waitForFinished(-1);
    disconnect(process, &QProcess::readyReadStandardOutput, this, &xAudioFile::processEncodeOutput);
    auto exitCode = process->exitCode();
    delete process;
    if (exitCode == QProcess::NormalExit) {
        return true;
    } else {
        try {
            std::filesystem::remove(flacFileName.toStdString());
        } catch (std::filesystem::filesystem_error& e) {
            qWarning() << "Unable to remove corrupt output file: " << flacFileName << ", error: " << e.what() << ", ignoring.";
        }
        return false;
    }
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

const QString& xAudioFile::getArtist() const {
    return encodingArtist;
}

const QString& xAudioFile::getAlbum() const {
    return encodingAlbum;
}

const QString& xAudioFile::getTrackNr() const {
    return encodingTrackNr;
}

const QString& xAudioFile::getTrackName() const {
    return encodingTrackName;
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

void xAudioFile::processEncodeOutput() {
}

void xAudioFile::processBackupOutput() {
}
