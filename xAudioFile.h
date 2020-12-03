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

#ifndef __XAUDIOFILE_H__
#define __XAUDIOFILE_H__

#include <QThread>
#include <QObject>
#include <QProcess>
#include <QList>

class xAudioFile;

class xAudioFileEncoding:public QThread {
    Q_OBJECT

public:
    explicit xAudioFileEncoding(const QList<std::pair<xAudioFile*,QString>>& files, bool flac, QObject* parent=nullptr);
    ~xAudioFileEncoding() override = default;

    void run() override;

signals:
    void encodingProgress(int track, int progress);

private:
    QList<std::pair<xAudioFile*,QString>> encodeFiles;
    bool encodeFlac;
};


class xAudioFile:public QObject {
    Q_OBJECT

public:
    xAudioFile();
    xAudioFile(const QString& fileName, int audioTrackNr, const QString& artist, const QString& album,
               const QString& trackNr, const QString& trackName, const QString& tag, int tagId, QObject* parent=nullptr);
    xAudioFile(const xAudioFile& copy);
    ~xAudioFile() override = default;

    [[nodiscard]] QString getFileName() const;
    [[nodiscard]] int getAudioTrackNr() const;
    [[nodiscard]] QString getArtist() const;
    [[nodiscard]] QString getAlbum() const;
    [[nodiscard]] QString getTrackNr() const;
    [[nodiscard]] QString getTrackName() const;
    [[nodiscard]] QString getTag() const;
    [[nodiscard]] int getTagId() const;

    bool encodeWavPack(const QString& wavPackFileName);
    bool encodeFlac(const QString& flacFileName);

signals:
    void encodingProgress(int track, int progress);

private slots:
    void processEncodeOutput();
    void processBackupOutput();

private:
    QString inputFileName;
    int inputAudioTrackNr;
    QString encodingArtist;
    QString encodingAlbum;
    QString encodingTrackNr;
    QString encodingTrackName;
    QString encodingTag;
    int encodingTagId;
    QProcess* process;
};

Q_DECLARE_METATYPE(xAudioFile)

#endif
