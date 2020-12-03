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
#include "xAudioFile.h"

#include <QThread>
#include <QProcess>
#include <QVector>

#ifndef __XMOVIEFILE_H__
#define __XMOVIEFILE_H__

struct xMovieFileAudioStream {
    QString codecName;
    QString codecLongName;
    QString profile;
    QString sampleFormat;
    int sampleRate;
    int bitsPerSample;
    int bitRate;
    int channels;
    QString channelLayout;
};

class xMovieFile:public QThread {
    Q_OBJECT

public:
    explicit xMovieFile(QObject* parent=nullptr);
    ~xMovieFile() override = default;
    /**
     * Analyze the given file.
     *
     * @param file path to the movie file as string.
     */
    void analyze(const QString& file);
    /**
     * Determine the number of tracks in the movie file.
     *
     * @return the number of tracks.
     */
    [[nodiscard]] int getTracks() const;
    /**
     * Return the file name of the movie file.
     *
     * @return the absolute path as string.
     */
    [[nodiscard]] QString getFileName() const;
    /**
     * Compute the times for each track of the audio CD.
     *
     * @return a vector of track length in milliseconds.
     */
    [[nodiscard]] QVector<qint64> getTracksLengths() const;
    /**
     * Retrieve the extracted infos for the requested audio stream.
     *
     * @param audio number of the audio stream as integer (starting with 0).
     * @return the audio stream infos as structure.
     */
    [[nodiscard]] xMovieFileAudioStream getAudioStreamInfo(int audio);
    /**
     * Queue the given to tracks
     *
     * @param files list of audio files containing all information.
     * @param audio index of the audio stream (starting at 0).
     * @param downMix down mix to stereo if true, false otherwise.
     */
    void queueRip(const QList<xAudioFile*>& files, int stream, bool downMix);
    /**
     * Rip all tracks queued.
     */
    void run() override;

signals:
    /**
     * Signal emitted after the movie file chapters have been ripped.
     *
     * @param files a list of audio file objects.
     */
    void audioFiles(const QList<xAudioFile*>& files);
    /**
     * Signal emitted after the movie file has been analyzed.
     *
     * @param infos a list of strings of stream infos.
     */
    void audioStreamInfos(const QStringList& infos);
    /**
     * Signal emitted after the movie file has been analyzed.
     *
     * @param lengths a vector of chapter length in milliseconds.
     */
    void trackLengths(const QVector<qint64>& lengths);
    /**
     * Signal the rip progress for movie file chapters.
     *
     * @param track the track currently ripped.
     * @param progress the rip progress for the current track.
     */
    void ripProgress(int track, int progress);
    /**
     * Signal the output of the rip process.
     *
     * @param msg the current output as string.
     */
    void messages(const QString& msg);


private slots:
    /**
     * Process the output of the rip process.
     */
    void processOutput();

private:
    /**
     * Clear all tracks for the movie file.
     */
    void clearTracks();

    typedef struct {
        xAudioFile* audioFile;
        int audioStream;
        int bitsPerSample;
        bool downMix;
    } xMovieFileQueue;

    QString movieFile;
    QString movieFilePath;
    QVector<xMovieFileAudioStream> movieFileAudioStreams;
    QVector<xMovieFileTrack*> movieFileTracks;
    QProcess* process;
    QVector<QList<xMovieFileQueue>> queue;
};

#endif
