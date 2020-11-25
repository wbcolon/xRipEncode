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

#include <QObject>
#include <QProcess>
#include <QVector>

#ifndef __XMOVIEFILE_H__
#define __XMOVIEFILE_H__

class xMovieFileProgress:public QProcess {
    Q_OBJECT
public:
    xMovieFileProgress(QObject* parent=nullptr);
    ~xMovieFileProgress() override = default;
};

class xMovieFile:public QObject {
    Q_OBJECT

public:
    typedef struct {
        QString language;
        QString type;
        QString channels;
        uint32_t trackNr;
        uint32_t samples;
        QString format;
        double bitrate;
    } xMovieAudioTrack;

    explicit xMovieFile(const QString& file, QObject* parent=nullptr);
    ~xMovieFile() override = default;

    int getTracks();

signals:
    void audioTracks(const QVector<double>& tracks);

private:
    void analyze();
    static xMovieAudioTrack parseAudioStream(const QString& line);
    static std::pair<double,double> parseChapterStream(const QString& line);

    QString movieFile;
    QVector<xMovieAudioTrack> movieAudioTracks;
    QVector<std::pair<double,double>> movieChapters;
    QProcess* process;
};

#endif
