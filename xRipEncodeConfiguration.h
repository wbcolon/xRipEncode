/*
 * This file is part of xPlay.
 *
 * xPlay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * xPlay is distributed in the hope that it will be useful,
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

    void setTempFileDirectory(const QString& directory);
    void setFileNameFormat(const QString& format);
    void setFileNameReplace(const QList<std::pair<QString, QString>>& replace);
    void setFileNameLowerCase(bool lowerCase);


    [[nodiscard]] QString getTempFileDirectory();
    [[nodiscard]] QString getFileNameFormat();
    [[nodiscard]] QList<std::pair<QString,QString>> getFileNameReplace();
    [[nodiscard]] bool getFileNameLowerCase();

    /**
     * Trigger all update configuration signals.
     *
     * Useful on application start, when the entire configuration is initially read.
     */
    void updatedConfiguration();

signals:
    void updatedTempFileDirectory();
    void updatedFileNameFormat();
    void updatedFileNameReplace();
    void updatedFileNameLowerCase();

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
