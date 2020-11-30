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
#include <filesystem>
#include <list>

class xRipEncodeConfiguration:public QObject {
    Q_OBJECT

public:
    /**
     * Return the Application Configuration.
     *
     * @return pointer to a singleton of the configuration.
     */
    static xRipEncodeConfiguration* configuration();

    void setTempDirectory(const QString& directory);
    void setBackupDirectory(const QString& directory);
    void setEncodingDirectory(const QString& directory);
    void setFileNameFormat(const QString& format);
    void setFileNameReplace(const QList<std::pair<QString, QString>>& replace);
    void setFileNameLowerCase(bool lowerCase);
    void setEncodingFormat(const QString& format);
    void setFFMpeg(const QString& path);
    void setFFProbe(const QString& path);
    void setMKVMerge(const QString& path);
    void setMKVExtract(const QString& path);
    void setFlac(const QString& path);
    void setWavPack(const QString& path);
    void setTags(const QStringList& tags);
    [[nodiscard]] QString getTempDirectory() const;
    [[nodiscard]] QString getBackupDirectory() const;
    [[nodiscard]] QString getEncodingDirectory() const;
    [[nodiscard]] QString getFileNameFormat() const;
    [[nodiscard]] QList<std::pair<QString,QString>> getFileNameReplace() const;
    [[nodiscard]] bool getFileNameLowerCase() const;
    [[nodiscard]] QString getEncodingFormat() const;
    [[nodiscard]] QString getFFMpeg() const;
    [[nodiscard]] QString getFFProbe() const;
    [[nodiscard]] QString getMKVMerge() const;
    [[nodiscard]] QString getMKVExtract() const;
    [[nodiscard]] QString getFlac() const;
    [[nodiscard]] QString getWavPack() const;
    [[nodiscard]] QStringList getTags() const;
    /**
     * Trigger all update configuration signals.
     *
     * Useful on application start, when the entire configuration is initially read.
     */
    void updatedConfiguration();

signals:
    void updatedTempDirectory();
    void updatedBackupDirectory();
    void updatedEncodingDirectory();
    void updatedFileNameFormat();
    void updatedFileNameReplace();
    void updatedFileNameLowerCase();
    void updatedEncodingFormat();

private:
    static QList<std::pair<QString,QString>> stringsToList(const QString& replaceFrom, const QString& replaceTo);
    static std::pair<QString,QString> listToString(const QList<std::pair<QString,QString>>& replace);

    xRipEncodeConfiguration();
    ~xRipEncodeConfiguration() override = default;

    const QString ApplicationName = "xRipEncode";
    const QString OrganisationName = "wbcolon";

    static xRipEncodeConfiguration* ripEncodeConfiguration;
    QSettings* settings;
};

#endif
