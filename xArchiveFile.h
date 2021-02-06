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

#ifndef __XRIPENCODE_XARCHIVEFILE_H__
#define __XRIPENCODE_XARCHIVEFILE_H__

#include "xAudioFile.h"
#include <QThread>
#include <QVector>

struct xArchiveFileTags {
    QString artist;
    QString album;
    QVector<QString> trackName;
    QVector<qint64> trackSize;
    int bitsPerSample;
};

class xArchiveFile:public QThread {
    Q_OBJECT

public:
    // Currently supported lookup schemes.
    static const QStringList TagLookupSchemes;

    explicit xArchiveFile(QObject* parent=nullptr);
    ~xArchiveFile() override = default;
    /**
     * Determine the number of relevant files in the archive file.
     *
     * @return the number of files.
     */
    [[nodiscard]] int getFiles() const;
    /**
     * Return the file name of the movie file.
     *
     * @return the absolute path as string.
     */
    [[nodiscard]] const QString& getFileName() const;
    /**
     * Analyze the given archive.
     *
     * @param file path to the archive file as string.
     */
    void analyze(const QString& file);
    /**
     * Extract tags (album, artist, track name, quality) out of archived file names.
     *
     * @param scheme the scheme used to extract tags as string.
     * @return tuple of artist, album and vector of track names.
     */
    xArchiveFileTags extractTags(const QString& scheme);
    /**
     * Queue the given to tracks
     *
     * @param files list of audio files containing all information.
     */
    void queueExtract(const QList<xAudioFile*>& files);
    /**
     * Extract all tracks queued.
     */
    void run() override;

signals:
    /**
     * Signal emitted after the archived file have been extracted.
     *
     * @param files a list of audio file objects.
     */
    void audioFiles(const QList<xAudioFile*>& files);
    /**
     * Signal emitted after the archive file has been analyzed.
     *
     * @param fileNames a vector of file names.
     * @param fileSizes a vector of file sizes in bytes.
     */
    void archivedFiles(const QVector<QString>& fileNames, const QVector<qint64>& fileSizes);
    /**
     * Signal the extract progress for the archived file.
     *
     * @param track the index of the archived file currently extracted.
     * @param progress the extract progress for the current archived file.
     */
    void extractProgress(int track, int progress);
    /**
     * Signal the output of the rip process.
     *
     * @param msg the current output as string.
     */
    void messages(const QString& msg);

private:
    /**
     * Extract album, artist and track name out of archived file names for Qobuz archives.
     *
     * @return tuple of artist, album and vector of track names.
     */
    xArchiveFileTags extractTagsFromQobuz();
    /**
     * Extract album, artist and track name out of archived file names for 7Digital archives.
     *
     * @return tuple of artist, album and vector of track names.
     */
    xArchiveFileTags extractTagsFrom7Digital();
    /**
     * Extract album, artist and track name out of archived file names for Bandcamp archives.
     *
     * @return tuple of artist, album and vector of track names.
     */
    xArchiveFileTags extractTagsFromBandcamp();
    /**
     * Find the queue entry for the corresponding archive entry.
     *
     * @param entry the archive entry currently parsed.
     * @return the audio file structure with the matching original name.
     */
    xAudioFile* findQueueEntry(struct archive_entry* entry);
    /**
     * Check if the file is a supported audio file.
     *
     * @param fileName the file name of the archive file.
     * @return true if the file name ends with .flac or .wav, false otherwise.
     */
    static bool validOutputFile(const QString& fileName);
    /**
     * Actually extract the given archive file.
     *
     * @param archiveFile pointer to the structure for the archive (input).
     * @param outputFile pointer to the structure for the output file.
     * @return true if the file was successfully extracted, false otherwise.
     */
    static bool extractOutputFile(struct archive* archiveFile, struct archive* outputFile);

    typedef struct {
        xAudioFile* audioFile;
        QString archiveFileName;
    } xArchiveFileQueue;

    QString archiveFileName;
    QVector<QString> archiveFileNames;
    QVector<int> archiveFileTrackNrs;
    QVector<qint64> archiveFileSizes;
    QList<xArchiveFileQueue> queue;
};


#endif
