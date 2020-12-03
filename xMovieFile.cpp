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
#include "xRipEncodeConfiguration.h"
#include <QRegularExpression>
#include <QTemporaryFile>
#include <QDebug>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

const char* xMovieFile_TemporaryFileBase { "riptmpfile" };

xMovieFile::xMovieFile(QObject* parent):
        QThread(parent) {
}

int xMovieFile::getTracks() const {
    return movieFileTracks.count();
}

QVector<qint64> xMovieFile::getTracksLengths() const {
    QVector<qint64> lengths;
    for (const auto& track : movieFileTracks) {
        lengths.push_back(track->getLength());
    }
    return lengths;
}

xMovieFileAudioStream xMovieFile::getAudioStreamInfo(int audio) {
    if ((audio >= 0) && (audio < movieFileAudioStreams.count())) {
        return movieFileAudioStreams[audio];
    } else {
        // Return empty structure
        return xMovieFileAudioStream { "", "", "", "",
                                       0, 0, 0, 0, "" };
    }
}

QString xMovieFile::getFileName() const {
    return movieFile;
}

void xMovieFile::analyze(const QString& file) {
    // Check if we really have a file.
    if (!std::filesystem::is_regular_file(file.toStdString())) {
        qCritical() << "xMovieFile::analyze: illegal file name: " << file;
        return;
    }
    movieFile = file;
    // Clear currently stored audio streams and queued tracks.
    movieFileAudioStreams.clear();
    queue.clear();
    // Start ffprobe in order to analyze the movie file.
    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(xRipEncodeConfiguration::configuration()->getFFProbe(),
                   {{"-v"},{"quiet"},{"-print_format"},{"json"},{"-show_streams"},{"-show_chapters"},movieFile});
    process->waitForFinished(-1);
    // Process standard output (includes stderr) and interpret json.
    std::stringstream processOutput;
    processOutput << process->readAllStandardOutput().toStdString();
    // Delete process.
    delete process;
    boost::property_tree::ptree movieFileInfo;
    try {
        boost::property_tree::read_json(processOutput, movieFileInfo);
    } catch (boost::property_tree::json_parser_error& e) {
        qCritical() << "xMovieFile::analyze: unable to read movie file info.";
        return;
    }
    // Extract infos.
    QStringList movieFileAudioTrackInfos;
    auto childStreams = movieFileInfo.get_child("streams");
    for (boost::property_tree::ptree::value_type& childStream : childStreams) {
        if (childStream.second.get<std::string>("codec_type") != "audio") {
            continue;
        }
        xMovieFileAudioStream movieFileAudioTrack;
        movieFileAudioTrack.codecName = QString::fromStdString(childStream.second.get<std::string>("codec_name", ""));
        movieFileAudioTrack.codecLongName = QString::fromStdString(childStream.second.get<std::string>("codec_long_name", ""));
        movieFileAudioTrack.profile = QString::fromStdString(childStream.second.get<std::string>("profile", ""));
        movieFileAudioTrack.sampleFormat = QString::fromStdString(childStream.second.get<std::string>("sample_fmt", ""));
        movieFileAudioTrack.sampleRate = childStream.second.get<uint32_t >("sample_rate", 44100);
        movieFileAudioTrack.bitsPerSample = childStream.second.get<uint32_t>("bits_per_sample", 0);
        if (!movieFileAudioTrack.bitsPerSample) {
            movieFileAudioTrack.bitsPerSample = childStream.second.get<uint32_t>("bits_per_raw_sample", 16);
        }
        movieFileAudioTrack.bitRate = childStream.second.get<uint32_t>("bit_rate", 0);
        movieFileAudioTrack.channels = childStream.second.get<uint32_t>("channels", 2);
        movieFileAudioTrack.channelLayout = QString::fromStdString(childStream.second.get<std::string>("channelLayout", ""));
        movieFileAudioStreams.push_back(movieFileAudioTrack);
        if (movieFileAudioTrack.profile.isEmpty()) {
            movieFileAudioTrackInfos.push_back(QString("%1, %2Hz, %3-bit, %4 channels").arg(movieFileAudioTrack.codecLongName).
                    arg(movieFileAudioTrack.sampleRate).arg(movieFileAudioTrack.bitsPerSample).arg(movieFileAudioTrack.channels));
        } else {
            movieFileAudioTrackInfos.push_back(QString("%1 (%2), %3Hz, %4-bit, %5 channels").arg(movieFileAudioTrack.codecLongName).
                    arg(movieFileAudioTrack.profile).arg(movieFileAudioTrack.sampleRate).arg(movieFileAudioTrack.bitsPerSample).arg(movieFileAudioTrack.channels));
        }
    }
    emit audioStreamInfos(movieFileAudioTrackInfos);
    clearTracks();
    QVector<qint64> movieFileTrackLengths;
    auto childTracks = movieFileInfo.get_child("chapters");
    for (boost::property_tree::ptree::value_type& childTrack : childTracks) {
        // Convert start and end time to ms.
        auto movieFileTrack = new xMovieFileTrack(childTrack.second.get<double>("start_time", 0.0),
                                                    childTrack.second.get<double>("end_time", 0.0), this);
        movieFileTracks.push_back(movieFileTrack);
        // Update track length.
        movieFileTrackLengths.push_back(movieFileTrack->getLength());
    }
    emit trackLengths(movieFileTrackLengths);
    // Resize the queue. Index start with 0 not with 1 as the track index does.
    queue.resize(getTracks());
}

void xMovieFile::queueRip(const QList<xAudioFile*>& files, int stream, bool downMix) {
    if ((stream < 0) || (stream >= movieFileAudioStreams.count())) {
        qCritical() << "Illegal audio stream: " << stream;
        return;
    }
    auto audioStreamInfo = movieFileAudioStreams[stream];
    for (const auto& file : files) {
        // audio track nr start with index 1.
        if ((file->getAudioTrackNr() < 1) || (file->getAudioTrackNr() > queue.count())) {
            qCritical() << "Illegal track: " << file->getAudioTrackNr();
            continue;
        }
        // queue starts with index 0.
        queue[file->getAudioTrackNr()-1].push_back(xMovieFileQueue{ file, stream, audioStreamInfo.bitsPerSample, downMix });
        qDebug() << "Added to rip queue: " << file->getFileName();
    }
}

void xMovieFile::processOutput() {
    QString ripOutput = process->readAllStandardOutput();
    QRegularExpression regExp(R"(.*The file '.*-(?<track>\d\d\d)' has been opened for writing.*)");
    QRegularExpressionMatch regExpMatch = regExp.match(ripOutput);
    if (regExpMatch.hasMatch()) {
        auto track = regExpMatch.captured("track").toInt();
        emit ripProgress(track, 50);
    }
    emit messages(ripOutput);
}

void xMovieFile::run() {
    // First we need to split the movie file into tracks.
    movieFilePath = xRipEncodeConfiguration::configuration()->getTempDirectory();
    auto movieFileOutput = movieFilePath + "/" + xMovieFile_TemporaryFileBase;
    // Redirect output only if necessary.
    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, &QProcess::readyReadStandardOutput, this, &xMovieFile::processOutput);
    process->start(xRipEncodeConfiguration::configuration()->getMKVMerge(),
                   { {"--split"}, {"chapters:all"}, movieFile, {"-o"}, movieFileOutput });
    process->waitForFinished(-1);
    disconnect(process, &QProcess::readyReadStandardOutput, this, &xMovieFile::processOutput);
    // Attach the temporary file names to the movie tracks.
    for (auto index = 0; index < movieFileTracks.count(); ++index) {
        // File names start with index 1.
        movieFileTracks[index]->attachFile(movieFileOutput+QString("-%1").arg(index+1, 3, 10, QChar('0')));
    }
    // Run through the queue
    QList<xAudioFile*> files;
    for (auto index = 0; index < queue.count(); ++index) {
        auto queueEntry = queue[index];
        if (queueEntry.isEmpty()) {
            continue;
        }
        for (auto i = 0; i < queueEntry.count(); ++i) {
            auto entry = queueEntry.at(i);
            if (movieFileTracks[index]->extract(entry.audioFile->getFileName(), entry.audioStream+1,
                                                entry.bitsPerSample, entry.downMix)) {
                files.push_back(entry.audioFile);
            }
            // Update rip progress. Track index starts with 1.
            emit ripProgress(index+1, 50 + ((i+1)*50/queueEntry.count()));
        }
    }
    // Emit extracted audio file queue.
    emit audioFiles(files);
    // Remove temporary track files.
    for (auto& track : movieFileTracks) {
        track->removeFile();
    }
    // Delete process.
    delete process;
    clearTracks();
}

void xMovieFile::clearTracks() {
    for (auto& track : movieFileTracks) {
        delete track;
    }
    movieFileTracks.clear();
}