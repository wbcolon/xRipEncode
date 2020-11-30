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

#include "xMovieFileTrack.h"
#include "xRipEncodeConfiguration.h"

#include <QDebug>
#include <QProcess>
#include <QStringList>
#include <filesystem>
#include <cmath>

xMovieFileTrack::xMovieFileTrack(double startTime, double endTime, QObject* parent):
    QObject(parent),
    trackStartTime(startTime),
    trackEndTime(endTime),
    trackFileName(),
    extractProcess(nullptr) {
    // required assertion.
    assert(trackStartTime <= trackEndTime);
}

void xMovieFileTrack::attachFile(const QString& fileName) {
    // Verify that we have a regular file.
    if (std::filesystem::is_regular_file(fileName.toStdString())) {
        trackFileName = fileName;
    }
}

void xMovieFileTrack::removeFile() {
    if (!trackFileName.isEmpty()) {
        try {
            std::filesystem::remove(trackFileName.toStdString());
        } catch (std::filesystem::filesystem_error& e)  {
            qCritical() << "Unable to remove track filename: " << trackFileName << ", error: " << e.what();
        }

    }
}

qint64 xMovieFileTrack::getLength() const {
    // Convert from seconds (fractions) to milliseconds.
    return static_cast<qint64>(std::round((trackEndTime-trackStartTime)*1000.0));
}

bool xMovieFileTrack::extract(const QString& fileName, int stream, int bitsPerSample, bool downMix) {
    // Prepare arguments for audio track extraction
    QStringList extractArguments {
            "-v", "error", "-y", "-i", trackFileName, "-map", QString("0:%1").arg(stream),
            "-acodec", QString("pcm_s%1le").arg(bitsPerSample)
    };
    if (downMix) {
        // Set output audio channels if we want to downmix (e.g. from 5.1 to stereo).
        extractArguments.push_back("-ac");
        extractArguments.push_back("2");
    }
    extractArguments.push_back(fileName);
    qDebug() << "xMovieFileTrack::extract: arguments: " << extractArguments.join(" ");
    // Setup extraction process.
    extractProcess = new QProcess();
    extractProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(extractProcess, &QProcess::readyReadStandardOutput, this, &xMovieFileTrack::processExtractOutput);
    extractProcess->start(xRipEncodeConfiguration::configuration()->getFFMpeg(), extractArguments);
    extractProcess->waitForFinished(-1);
    disconnect(extractProcess, &QProcess::readyReadStandardOutput, this, &xMovieFileTrack::processExtractOutput);
    auto exitCode = extractProcess->exitCode();
    delete extractProcess;
    if (exitCode == QProcess::NormalExit) {
        return true;
    } else {
        try {
            std::filesystem::remove(fileName.toStdString());
        } catch (std::filesystem::filesystem_error& e) {
            qWarning() << "Unable to remove corrupt output file: " << fileName << ", error: " << e.what() << ", ignoring.";
        }
        return false;
    }
}

void xMovieFileTrack::processExtractOutput() {
    QString processOutput(extractProcess->readAllStandardOutput());
    emit
    qDebug() << "stdOut: " << processOutput;
}

