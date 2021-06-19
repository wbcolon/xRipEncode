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
    /**
     * Constructor.
     *
     * @param files list of pairs of audio file objects and file names.
     * @param flac encode as flac if true, wavpack otherwise.
     * @param parent pointer to the parent object.
     */
    explicit xAudioFileEncoding(const QList<std::pair<xAudioFile*,QString>>& files, bool flac, QObject* parent=nullptr);
    ~xAudioFileEncoding() override = default;
    /**
     * Encode all files in the queue in a separate thread.
     */
    void run() override;

signals:
    /**
     * Signal emitted to indicate progress of encoding process.
     *
     * @param track the number of the track currently encoded.
     * @param progress the encoding progress for the current track.
     */
    void encodingProgress(int track, int progress);

private:
    QList<std::pair<xAudioFile*,QString>> encodeFiles;
    bool encodeFlac;
};


class xAudioFile:public QObject {
    Q_OBJECT

public:
    /**
     * Constructor. Create empty audio file object.
     */
    xAudioFile();
    /**
     * Constructor. Create an audio file object connected to a file.
     *
     * @param fileName the absolute path to the file as string.
     * @param audioTrackNr the track of the audio CD, chapter in the movie file or file in archive.
     * @param artist the artist name as string.
     * @param album the album name as string.
     * @param trackNr the track number as string
     * @param trackName the track name as string.
     * @param tag the additional tag as string.
     * @param tagId the corresponding tag ID.
     * @param id the job Id the audio file belongs to.
     * @param parent pointer to the parent object.
     */
    xAudioFile(const QString& fileName, int audioTrackNr, const QString& artist, const QString& album,
               const QString& trackNr, const QString& trackName, const QString& tag, int tagId,
               quint64 id, QObject* parent=nullptr);
    /**
     * Copy constructor.
     *
     * @param copy a reference to the object to make a copy of.
     */
    xAudioFile(const xAudioFile& copy);
    /**
     * Destructor. Remove the file attached to the object.
     */
    ~xAudioFile() noexcept override;
    /**
     * Get the file name of the attached object.
     *
     * @return the path to the file as string.
     */
    [[nodiscard]] const QString& getFileName() const;
    /**
     * Get the track number/chapter.
     *
     * @return the track number or chapter as integer.
     */
    [[nodiscard]] int getAudioTrackNr() const;
    /**
     * Set the artist.
     *
     * @param artist the artist name as string.
     */
    void setArtist(const QString& artist);
    /**
     * Get the artist used as tag.
     *
     * @return the artist name as string.
     */
    [[nodiscard]] const QString& getArtist() const;
    /**
     * Set the album.
     *
     * @param album the album name as string.
     */
    void setAlbum(const QString& album);
    /**
     * Get the album used as tag.
     *
     * @return the album name as string.
     */
    [[nodiscard]] const QString& getAlbum() const;
    /**
     * Set the track number.
     *
     * @param trackNr the track number as string.
     */
    void setTrackNr(const QString& trackNr);
    /**
     * Get the track number used as tag.
     *
     * @return the track number as string.
     */
    [[nodiscard]] const QString& getTrackNr() const;
    /**
     * Set the track name.
     *
     * @param trackName the track name as string.
     */
    void setTrackName(const QString& trackName);
    /**
     * Get the track name used as tag.
     *
     * @return the track name as string.
     */
    [[nodiscard]] const QString& getTrackName() const;
    /**
     * Set the tag.
     *
     * @param tag the tag as string.
     */
    void setTag(const QString& tag);
    /**
     * Get the hd/multi-channel tag used.
     *
     * @return the tag as string.
     */
    [[nodiscard]] const QString& getTag() const;
    /**
     * Get the hd/multi-channel tag ID.
     *
     * @return the tag ID as integer.
     */
    [[nodiscard]] int getTagId() const;
    /**
     * Get the job ID the audio file belongs to.
     *
     * @return the job ID as integer.
     */
    [[nodiscard]] quint64 getJobId() const;
    /**
     * Encode the audio file into a wavpack file. No tags.
     *
     * @param wavPackFileName the name of the wavpack output file.
     * @return true, if the encoding process was successful, false otherwise.
     */
    virtual bool encodeWavPack(const QString& wavPackFileName) = 0;
    /**
     * Encode the audio file into a flac file. Artist, album, etc. tags are set.
     *
     * @param flacFileName the name of the flac output file.
     * @return true, if the encoding process was successful, false otherwise.
     */
    virtual bool encodeFlac(const QString& flacFileName) = 0;
    /**
     * Remove the file attached to the object.
     */
    void remove();

signals:
    /**
     * Signal emitted to indicate progress of encoding process.
     *
     * @param track the number of the track currently encoded.
     * @param progress the encoding progress for the current track.
     */
    void encodingProgress(int track, int progress);

protected:
    QString inputFileName;
    int inputAudioTrackNr;
    QString encodingArtist;
    QString encodingAlbum;
    QString encodingTrackNr;
    QString encodingTrackName;
    QString encodingTag;
    int encodingTagId;
    quint64 jobId;
};

class xAudioFileWav:public xAudioFile {
    Q_OBJECT

public:
    /**
     * Constructor. Create empty audio file object.
     */
    xAudioFileWav();
    /**
     * Constructor. Create an audio file object connected to a file.
     * @param fileName the absolute path to the file as string.
     * @param audioTrackNr the track of the audio CD, chapter in the movie file or file in archive.
     * @param artist the artist name as string.
     * @param album the album name as string.
     * @param trackNr the track number as string
     * @param trackName the track name as string.
     * @param tag the additional tag as string.
     * @param tagId the corresponding tag ID.
     * @param id the job Id the audio file belongs to.
     * @param parent pointer to the parent object.
     */
    xAudioFileWav(const QString& fileName, int audioTrackNr, const QString& artist, const QString& album,
                  const QString& trackNr, const QString& trackName, const QString& tag, int tagId,
                  quint64 id, QObject* parent=nullptr);
    /**
     * Copy constructor.
     *
     * @param copy a reference to the object to make a copy of.
     */
    xAudioFileWav(const xAudioFileWav& copy);
    /**
     * Encode the audio file into a wavpack file. No tags.
     *
     * @param wavPackFileName the name of the wavpack output file.
     * @return true, if the encoding process was successful, false otherwise.
     */
    bool encodeWavPack(const QString& wavPackFileName) override;
    /**
     * Encode the audio file into a flac file. Artist, album, etc. tags are set.
     *
     * @param flacFileName the name of the flac output file.
     * @return true, if the encoding process was successful, false otherwise.
     */
    bool encodeFlac(const QString& flacFileName) override;

private:
    QProcess* process;
};

class xAudioFileFlac:public xAudioFile {
    Q_OBJECT

public:
    /**
     * Constructor. Create empty audio file object.
     */
    xAudioFileFlac();
    /**
     * Constructor. Create an audio file object connected to a file.
     *
     * @param fileName the absolute path to the file as string.
     * @param audioTrackNr the track of the audio CD, chapter in the movie file or file in archive.
     * @param artist the artist name as string.
     * @param album the album name as string.
     * @param trackNr the track number as string
     * @param trackName the track name as string.
     * @param tag the additional tag as string.
     * @param tagId the corresponding tag ID.
     * @param id the job Id the audio file belongs to.
     * @param parent pointer to the parent object.
     */
    xAudioFileFlac(const QString& fileName, int audioTrackNr, const QString& artist, const QString& album,
                   const QString& trackNr, const QString& trackName, const QString& tag, int tagId,
                   quint64 id, QObject* parent=nullptr);
    /**
     * Copy constructor.
     *
     * @param copy a reference to the object to make a copy of.
     */
    xAudioFileFlac(const xAudioFileFlac& copy);
    /**
     * Encode the audio file into a wavpack file. No tags.
     *
     * @param wavPackFileName the name of the wavpack output file.
     * @return true, if the encoding process was successful, false otherwise.
     */
    bool encodeWavPack(const QString& wavPackFileName) override;
    /**
     * Encode the audio file into a flac file. Artist, album, etc. tags are set.
     *
     * @param flacFileName the name of the flac output file.
     * @return true, if the encoding process was successful, false otherwise.
     */
    bool encodeFlac(const QString& flacFileName) override;

private:
    QProcess* process;
};

Q_DECLARE_METATYPE(xAudioFileWav)
Q_DECLARE_METATYPE(xAudioFileFlac)

#endif
