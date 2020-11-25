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

#include "xMovieFile.h"
#include <QRegularExpression>
#include <QDebug>

const char* xMovieFile_Program_FFProbe_Path { "/usr/bin/ffprobe" };

xMovieFileProgress::xMovieFileProgress(QObject* parent):
        QProcess(parent) {

}


xMovieFile::xMovieFile(const QString& file, QObject* parent):
        QObject(parent),
        movieFile(file) {

    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    analyze();
}

int xMovieFile::getTracks() {
    return movieChapters.count();
}

void xMovieFile::analyze() {
    // Clear currently stored audio Tracks.
    movieAudioTracks.clear();
    movieChapters.clear();
    // Start ffprobe in order to analyze the movie file.
    process->start(xMovieFile_Program_FFProbe_Path, { movieFile });
    process->waitForFinished();
    // Parse through the output.
    QTextStream stdOutput(process->readAllStandardOutput());
    QString outputLine;
    while (stdOutput.readLineInto(&outputLine)) {
        // Chapters.
        if (outputLine.contains("Chapter #0")) {
            movieChapters.push_back(xMovieFile::parseChapterStream(outputLine));
        }
        // Audio tracks.
        if (outputLine.contains(QRegularExpression("Stream.*Audio:"))) {
            movieAudioTracks.push_back(xMovieFile::parseAudioStream(outputLine));
        }
    }
    for (const auto& track : movieAudioTracks) {
        qDebug() << track.trackNr << "," << track.language << "," << track.type << ","
                 << track.channels << "," << track.samples << "," << track.bitrate;
    }
    qDebug() << "Chapters: " << movieChapters;
}

xMovieFile::xMovieAudioTrack xMovieFile::parseAudioStream(const QString& line) {
    auto lineSplit = line.split(',');
    QVector<QRegularExpression> audioRegExp = {
            QRegularExpression(".*Stream #0:(?<tracknr>.*)\\((?<language>.*)\\): Audio: (?<type>.*)"),
            QRegularExpression(" (?<samples>.*) Hz"),
            QRegularExpression(" (?<channels>.*)(\\(side\\)| channels)"),
            QRegularExpression(" (?<format>.*)"),
            QRegularExpression(" (?<bitrate>.*) kb/s")
    };
    QVector<QRegularExpressionMatch> audioRegExpMatch(audioRegExp.count());
    for (auto i = 0; i < lineSplit.count(); ++i) {
        audioRegExpMatch[i] = audioRegExp.at(i).match(lineSplit.at(i));
    }
    double bitrate = 0.0;
    if (audioRegExpMatch.at(4).hasMatch()) {
        bitrate = audioRegExpMatch.at(4).captured("bitrate").toDouble();
    }
    return xMovieAudioTrack {
            audioRegExpMatch[0].captured("language"),
            audioRegExpMatch[0].captured("type"),
            audioRegExpMatch[2].captured("channels"),
            audioRegExpMatch[0].captured("tracknr").toUInt(),
            audioRegExpMatch[1].captured("samples").toUInt(),
            audioRegExpMatch[3].captured("format"),
            bitrate
    };
}
std::pair<double,double> xMovieFile::parseChapterStream(const QString& line) {
    QRegularExpression chapterRegExp(".*Chapter.*start (?<start>.*), end (?<end>.*)");
    QRegularExpressionMatch chapterRegExpMatch = chapterRegExp.match(line);
    if (chapterRegExpMatch.hasMatch()) {
        return std::make_pair(chapterRegExpMatch.captured("start").toDouble(),
                              chapterRegExpMatch.captured("end").toDouble());
    } else {
        return std::make_pair(-1.0, -1.0);
    }
}
