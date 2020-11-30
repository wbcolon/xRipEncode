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

#ifndef __XMOVIEFILETRACK_H__
#define __XMOVIEFILETRACK_H__

#include <QObject>
#include <QProcess>

class xMovieFileTrack: public QObject {
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param startTime start of the track within the movie file in seconds.
     * @param endTime end of the track within the movie file in seconds.
     * @param parent pointer to the parent object.
     */
    explicit xMovieFileTrack(double startTime, double endTime, QObject* parent=nullptr);
    /**
     * Destructor
     */
    ~xMovieFileTrack() override = default;
    /**
     * Return the length of the current track.
     *
     * @return length of the track in milliseconds.
     */
    [[nodiscard]] qint64 getLength() const;
    /**
     * Attach a filename to the current track.
     *
     * @param fileName path to the file for the current track.
     */
    void attachFile(const QString& fileName);
    /**
     * Remove the track file.
     */
    void removeFile();
    /**
     * Extract the audio stream out of the track file.
     *
     * @param fileName the output file name.
     * @param stream the index of the audio stream.
     * @param bitsPerSample bits per sample for the audio stream.
     * @param downMix down mix the audio stream to stereo if true.
     */
    bool extract(const QString& fileName, int stream, int bitsPerSample, bool downMix);

private slots:
    void processExtractOutput();

private:
    double trackStartTime;
    double trackEndTime;
    QString trackFileName;
    QProcess* extractProcess;
};

#endif
