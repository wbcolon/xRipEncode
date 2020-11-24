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

#include "xRipEncodeConfiguration.h"

#include <QList>
#include <QUrl>
#include <QRegularExpression>
#include <QDebug>

// Configuration strings.
const char* xRipEncodeConfiguration_TempFileDirectory { "xRipEncode/TempFileDirectory" };
const char* xRipEncodeConfiguration_FileNameFormat { "xRipEncode/FileNameFormat" };
const char* xRipEncodeConfiguration_FileNameReplaceFrom { "xRipEncode/FileNameReplaceFrom" };
const char* xRipEncodeConfiguration_FileNameReplaceTo { "xRipEncode/FileNameReplaceTo" };
const char* xRipEncodeConfiguration_FileNameLowerCase { "xRipEncode/FileNameLowerCase" };

const char* xRipEncodeConfiguration_TempFileDirectory_Default { "/tmp" };
const char* xRipEncodeConfiguration_FileNameFormat_Default { "(artist)#(album)#(tracknr) (trackname)" };
const char* xRipEncodeConfiguration_FileNameReplaceFrom_Default { "?" };
const char* xRipEncodeConfiguration_FileNameReplaceTo_Default { "" };
const bool xRipEncodeConfiguration_FileNameLower_Default = true;


// singleton object.
xRipEncodeConfiguration* xRipEncodeConfiguration::ripEncodeConfiguration = nullptr;

xRipEncodeConfiguration::xRipEncodeConfiguration():
        QObject() {
    // Settings.
    settings = new QSettings(xRipEncodeConfiguration::OrganisationName, xRipEncodeConfiguration::ApplicationName, this);
}

xRipEncodeConfiguration* xRipEncodeConfiguration::configuration() {
    // Create and return singleton.
    if (ripEncodeConfiguration == nullptr) {
        ripEncodeConfiguration = new xRipEncodeConfiguration();
    }
    return ripEncodeConfiguration;
}

void xRipEncodeConfiguration::setTempFileDirectory(const QString& directory) {
    if (directory != getTempFileDirectory()) {
        settings->setValue(xRipEncodeConfiguration_TempFileDirectory, directory);
        settings->sync();
        emit updatedTempFileDirectory();
    }
}

void xRipEncodeConfiguration::setFileNameFormat(const QString& format) {
    if (format != getFileNameFormat()) {
        settings->setValue(xRipEncodeConfiguration_FileNameFormat, format);
        settings->sync();
        emit updatedFileNameFormat();
    }
}

void xRipEncodeConfiguration::setFileNameReplace(const QList<std::pair<QString, QString>>& replace) {
    // New values
    auto [nReplaceFrom,nReplaceTo] = xRipEncodeConfiguration::listToString(replace);
    // Retrieve current values.
    auto replaceFrom = settings->value(xRipEncodeConfiguration_FileNameReplaceFrom,
                                       xRipEncodeConfiguration_FileNameReplaceFrom_Default).toString();
    auto replaceTo = settings->value(xRipEncodeConfiguration_FileNameReplaceTo,
                                     xRipEncodeConfiguration_FileNameReplaceTo_Default).toString();
    if ((nReplaceFrom != replaceFrom) || (nReplaceTo != replaceTo)) {
        settings->setValue(xRipEncodeConfiguration_FileNameReplaceFrom, nReplaceFrom);
        settings->setValue(xRipEncodeConfiguration_FileNameReplaceTo, nReplaceTo);
        settings->sync();
        emit updatedFileNameReplace();
    }
}

void xRipEncodeConfiguration::setFileNameLowerCase(bool lowerCase) {
    if (lowerCase != getFileNameLowerCase()) {
        settings->setValue(xRipEncodeConfiguration_FileNameLowerCase, lowerCase);
        settings->sync();
        emit updatedFileNameLowerCase();
    }
}

QString xRipEncodeConfiguration::getTempFileDirectory() {
    return settings->value(xRipEncodeConfiguration_TempFileDirectory,
                           xRipEncodeConfiguration_TempFileDirectory_Default).toString();
}

QString xRipEncodeConfiguration::getFileNameFormat() {
    return settings->value(xRipEncodeConfiguration_FileNameFormat,
                           xRipEncodeConfiguration_FileNameFormat_Default).toString();
}

QList<std::pair<QString,QString>> xRipEncodeConfiguration::getFileNameReplace() {
    // Retrieve current values.
    auto replaceFrom = settings->value(xRipEncodeConfiguration_FileNameReplaceFrom,
                                       xRipEncodeConfiguration_FileNameReplaceFrom_Default).toString();
    auto replaceTo = settings->value(xRipEncodeConfiguration_FileNameReplaceTo,
                                     xRipEncodeConfiguration_FileNameReplaceTo_Default).toString();
    return xRipEncodeConfiguration::stringsToList(replaceFrom, replaceTo);
}

bool xRipEncodeConfiguration::getFileNameLowerCase() {
    return settings->value(xRipEncodeConfiguration_FileNameLowerCase,
                           xRipEncodeConfiguration_FileNameLower_Default).toBool();
}

void xRipEncodeConfiguration::updatedConfiguration() {
    // Fire all update signals.
    emit updatedTempFileDirectory();
    emit updatedFileNameFormat();
    emit updatedFileNameReplace();
    emit updatedFileNameLowerCase();
}

QList<std::pair<QString,QString>> xRipEncodeConfiguration::stringsToList(const QString& replaceFrom,
                                                                         const QString& replaceTo) {
    QList<std::pair<QString,QString>> replace;
    if ((replaceFrom.isEmpty() && (replaceTo.isEmpty()))) {
        return replace;
    }
    auto splitReplaceFrom = replaceFrom.split('|');
    auto splitReplaceTo = replaceTo.split('|');
    if (splitReplaceFrom.count() == splitReplaceTo.count()) {
        for (auto index = 0; index < splitReplaceFrom.count(); ++index) {
            replace.push_back(std::make_pair(splitReplaceFrom.at(index), splitReplaceTo.at(index)));
        }
    }
    return replace;
}

std::pair<QString,QString> xRipEncodeConfiguration::listToString(const QList<std::pair<QString, QString>>& replace) {
    QString replaceFrom;
    QString replaceTo;
    if (!replace.isEmpty()) {
        replaceFrom = replace.at(0).first;
        replaceTo = replace.at(0).second;
        for (auto index = 1; index < replace.count(); ++index) {
            replaceFrom += "|" + replace.at(index).first;
            replaceTo += "|" + replace.at(index).second;
        }
    }
    return std::make_pair(replaceFrom, replaceTo);
}

