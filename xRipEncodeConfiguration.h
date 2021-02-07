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

#ifndef __XRIPENCODECONFIGURATION_H__
#define __XRIPENCODECONFIGURATION_H__

#include <QSettings>
#include <QString>
#include <QList>
#include <QUrl>
#include <QDebug>
#include <filesystem>
#include <list>

const bool xRipEncodeUseFlatGroupBox = true;

class xRipEncodeConfiguration:public QObject {
    Q_OBJECT

public:
    /**
     * Return the Application Configuration.
     *
     * @return pointer to a singleton of the configuration.
     */
    static xRipEncodeConfiguration* configuration();
    /**
     * Set the temp directory for audio CD and movie file rip output.
     *
     * @param directory the absolute path as string.
     */
    void setTempDirectory(const QString& directory);
    /**
     * Set the backup directory for the wavpack encoded files.
     *
     * @param directory the absolute path as string.
     */
    void setBackupDirectory(const QString& directory);
    /**
     * Set the encoding directory for the flac encoded files.
     *
     * @param directory the absolute path as string.
     */
    void setEncodingDirectory(const QString& directory);
    /**
     * Set the format for ripped audio CD and movie file names.
     *
     * @param format the format of the file name as string.
     */
    void setFileNameFormat(const QString& format);
    /**
     * Set the replacement from/to strings.
     *
     * @param replace list of pairs of from/to strings used.
     */
    void setFileNameReplace(const QList<std::pair<QString, QString>>& replace);
    /**
     * Set the lowercase mode flag.
     *
     * @param lowerCase set file names to lowercase if true.
     */
    void setFileNameLowerCase(bool lowerCase);
    /**
     * Set the format for the encoded files.
     *
     * @param format the format of the file name as string.
     */
    void setEncodingFormat(const QString& format);
    /**
     * Set path to ffmpeg.
     *
     * @param path the absolute path as string.
     */
    void setFFMpeg(const QString& path);
    /**
     * Set path to ffprobe.
     *
     * @param path the absolute path as string.
     */
    void setFFProbe(const QString& path);
    /**
     * Set path to mkvmerge.
     *
     * @param path the absolute path as string.
     */
    void setMKVMerge(const QString& path);
    /**
     * Set path to mkvextract.
     *
     * @param path the absolute path as string.
     */
    void setMKVExtract(const QString& path);
    /**
     * Set path to flac.
     *
     * @param path the absolute path as string.
     */
    void setFlac(const QString& path);
    /**
     * Set path to wavpack.
     *
     * @param path the absolute path as string.
     */
    void setWavPack(const QString& path);
    /**
     * Set path to lltag.
     *
     * @param path the absolute path as string.
     */
    void setLLTag(const QString& path);
    /**
     * Set the tags for HD and multi-channel.
     *
     * @param tags list of strings with tags.
     */
    void setTags(const QStringList& tags);
    /**
     * Get the temp directory for audio CD and movie file rip output.
     *
     * @return the temp directory as string.
     */
    [[nodiscard]] QString getTempDirectory() const;
    /**
     * Get the backup directory for the wavpack encoded files.
     *
     * @return the backup directory as string.
     */
    [[nodiscard]] QString getBackupDirectory() const;
    /**
     * Get the encoding directory for the flac encoded files.
     *
     * @return the encoding directory as string.
     */
    [[nodiscard]] QString getEncodingDirectory() const;
    /**
     * Get the format for ripped audio CD and movie file names.
     *
     * @return the format of the file name as string.
     */
    [[nodiscard]] QString getFileNameFormat() const;
    /**
     * Get the replacement from/to strings.
     *
     * @return list of pairs of from/to strings.
     */
    [[nodiscard]] QList<std::pair<QString,QString>> getFileNameReplace() const;
    /**
     * Get the lowercase mode flag.
     *
     * @return true, if file names are set to lowercase, false otherwise.
     */
    [[nodiscard]] bool getFileNameLowerCase() const;
    /**
     * Get the format for the encoded files.
     *
     * @return the format of the file name as string.
     */
    [[nodiscard]] QString getEncodingFormat() const;
    /**
     * Get the path to ffmpeg.
     *
     * @return the absolute path as string.
     */
    [[nodiscard]] QString getFFMpeg() const;
    /**
     * Get the path to ffprobe.
     *
     * @return the absolute path as string.
     */
    [[nodiscard]] QString getFFProbe() const;
    /**
     * Get the path to mkvmerge.
     *
     * @return the absolute path as string.
     */
    [[nodiscard]] QString getMKVMerge() const;
    /**
     * Get the path to mkvextract.
     *
     * @return the absolute path as string.
     */
    [[nodiscard]] QString getMKVExtract() const;
    /**
     * Get the path to flac.
     *
     * @return the absolute path as string.
     */
    [[nodiscard]] QString getFlac() const;
    /**
     * Get the path to wavpack.
     *
     * @return the absolute path as string.
     */
    [[nodiscard]] QString getWavPack() const;
    /**
     * Get the path to lltag.
     *
     * @return the absolute path as string.
     */
    [[nodiscard]] QString getLLTag() const;
    /**
     * Get the tags for HD and multi-channel.
     *
     * @return the list of strings with tags.
     */
    [[nodiscard]] QStringList getTags() const;
    /**
     * Get infos for the corresponding tags for HD and multi-channel.
     *
     * @return the list of info strings.
     */
    [[nodiscard]] QStringList getTagInfos() const;
    /**
     * Trigger all update configuration signals.
     *
     * Useful on application start, when the entire configuration is initially read.
     */
    void updatedConfiguration();

signals:
    /**
     * Signal an update of the temp directory.
     */
    void updatedTempDirectory();
    /**
     * Signal an update of the backup directory.
     */
    void updatedBackupDirectory();
    /**
     * Signal and update of the encoding directory.
     */
    void updatedEncodingDirectory();
    /**
     * Signal an update of the file name format.
     */
    void updatedFileNameFormat();
    /**
     * Signal an update of the file name replace strings.
     */
    void updatedFileNameReplace();
    /**
     * Signal an update of the lowercase flag.
     */
    void updatedFileNameLowerCase();
    /**
     * Signal an update of the encoding format.
     */
    void updatedEncodingFormat();

private:
    /**
     * Convert configuration strings to list of pairs.
     *
     * @param replaceFrom serialized list of from strings.
     * @param replaceTo serialized list of to strings.
     * @return list of pairs of strings from/to.
     */
    static QList<std::pair<QString,QString>> stringsToList(const QString& replaceFrom, const QString& replaceTo);
    /**
     * Convert list of pairs of strings to configuration strings.
     *
     * @param replace list of pairs of strings from/to.
     * @return pair of serialized from/to strings.
     */
    static std::pair<QString,QString> listToString(const QList<std::pair<QString,QString>>& replace);

    xRipEncodeConfiguration();
    ~xRipEncodeConfiguration() override = default;

    const QString ApplicationName = "xRipEncode";
    const QString OrganisationName = "wbcolon";

    static xRipEncodeConfiguration* ripEncodeConfiguration;
    QSettings* settings;
};

#endif
