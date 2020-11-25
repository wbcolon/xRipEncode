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

#ifndef __XAUDIOCD_H__
#define __XAUDIOCD_H__

#include <QThread>
#include <QList>
#include <QString>
#include <cdio/paranoia/paranoia.h>
#include <cdio/cd_types.h>
#include <cdio/device.h>

class xAudioCDLookup:public QThread {
    Q_OBJECT

public:
    /**
     * Result from the MusicBrainz query.
     */
    typedef struct {
        QString artist;
        QString album;
        QVector<QString> tracks;
    } xAudioCDLookupResult;
    /**
     * Constructor
     *
     * @param id the disc ID for the MusicBrainz query.
     * @param lowerCase convert the query results to lower case if true.
     * @param parent pointer to the parent widget.
     */
    explicit xAudioCDLookup(const QString& id, QObject* parent=nullptr);
    /**
     * Destructor (default)
     */
    ~xAudioCDLookup() override = default;
    /**
     * Run the query to MusicBrainz.
     *
     * Query is run in separate thread. After the query is finished, the signal
     * finished is emitted. Only then the results can be requested.
     */
    void run() override;
    /**
     * Return the results of the MusicBrainz query.
     *
     * @return a list of structs that contains artist, album and track list.
     */
    [[nodiscard]] QList<xAudioCDLookupResult> result() const;

private:
    QString musicBrainzID;
    QList<xAudioCDLookupResult> musicBrainzResult;
};

class xAudioCDRipper:public QThread {
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param drive pointer to the structure required by libcdio-paranoia.
     * @param tracks list of pairs of track number and names to be ripped.
     * @param parent pointer to the parent widget.
     */
    xAudioCDRipper(cdrom_drive_t* drive, const QList<std::pair<int,QString>>& tracks, QObject* parent=nullptr);
    /**
     * Destructor (default)
     */
    ~xAudioCDRipper() override = default;
    /**
     * Start the rip process.
     *
     * All tracks in the given list are ripped and the output is written
     * into wav file of the given file name.
     */
    void run() override;

signals:
    /**
     * Signal emitted to show the rip progress.
     *
     * @param track number of the current track that is ripped.
     * @param percent the percentage for the current track.
     */
    void progress(int track, int percent);
    /**
     * Signal emitted if an error occurs during the rip process.
     *
     * @param track number of the current track that is ripped.
     * @param error the error message as string.
     * @param abort the rip process is aborted if true, false otherwise.
     */
    void error(int track, const QString& error, bool abort);
    /**
     * Signal emitted if a message occurs during the rip process.
     *
     * @param track number of the current track that is ripped.
     * @param message the rip message as string.
     */
    void messages(int track, const QString& message);

private:
    /**
     * Write wav file header.
     *
     * @param dataStream the stream to write the header to.
     * @param byteCount the size of the wav file.
     */
    static void writeWaveHeader(QDataStream& dataStream, int byteCount);

    cdrom_drive_t* audioDrive;
    QList<std::pair<int,QString>> audioTracks;
};


class xAudioCD: public QObject {
    Q_OBJECT

public:
    explicit xAudioCD(QObject* parent=nullptr);
    ~xAudioCD() override;
    /**
     * Autodetect audio CD.
     *
     * @return true if an audio CD is detected, false otherwise.
     */
    [[nodiscard]] bool detect();
    /**
     * Eject the audio CD if previously detected.
     */
    void eject();
    /**
     * Close the audio CD if preciously detected.
     */
    void close();
    /**
     * Determine the number of tracks of the audio CD.
     *
     * @return number of tracks if detected, -1 otherwise.
     */
    [[nodiscard]] int getTracks();
    /**
     * Compute the times for each track of the audio CD.
     *
     * @return a vector of track length in milliseconds.
     */
    [[nodiscard]] QVector<qint64> getTrackLengths();
    /**
     * Compute the ID of the audio CD for MusicBrainz
     *
     * @return the ID as string. Return empty string on error.
     */
    [[nodiscard]] QString getID();
    /**
     * Rip the tracks from the audio CD.
     *
     * @param tracks a list of pairs of track number and corresponding file names.
     */
    void rip(const QList<std::pair<int, QString>>& tracks);
    /**
     * Cancel the current rip process.
     */
    void ripCancel();

signals:
    /**
     * Signal emitted (forwarded) to show the rip progress.
     *
     * @param track number of the current track that is ripped.
     * @param percent the percentage for the current track.
     */
    void ripProgress(int track, int percent);
    /**
     * Signal emitted (forwarded) if an error occurs during the rip process.
     *
     * @param track number of the current track that is ripped.
     * @param error the error message as string.
     * @param abort the rip process is aborted if true, false otherwise.
     */
    void ripError(int track, const QString& error, bool abort);
    /**
     * Signal emitted (forwarded) if a message occurs during the rip process.
     *
     * @param track number of the current track that is ripped.
     * @param message the rip message as string.
     */
    void ripMessages(int track, const QString& message);
    /**
     * Signal emitted if the scan process is finished.
     */
    void ripFinished();

private slots:
    /**
     * Called if rip thread is finished.
     */
    void ripThreadFinished();

private:
    cdrom_drive_t* audioDrive;
    xAudioCDRipper* audioRipper;
};

#endif
