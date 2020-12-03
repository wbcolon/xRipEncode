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

#include "xRipEncodeConfiguration.h"

#include <filesystem>
#include <QList>
#include <QUrl>
#include <QRegularExpression>
#include <QDebug>

// Configuration strings.
const char* xRipEncodeConfiguration_TempDirectory { "xRipEncode/TempDirectory" };
const char* xRipEncodeConfiguration_BackupDirectory { "xRipEncode/BackupDirectory" };
const char* xRipEncodeConfiguration_EncodingDirectory { "xRipEncode/EncodingDirectory" };
const char* xRipEncodeConfiguration_FileNameFormat { "xRipEncode/FileNameFormat" };
const char* xRipEncodeConfiguration_FileNameReplaceFrom { "xRipEncode/FileNameReplaceFrom" };
const char* xRipEncodeConfiguration_FileNameReplaceTo { "xRipEncode/FileNameReplaceTo" };
const char* xRipEncodeConfiguration_FileNameLowerCase { "xRipEncode/FileNameLowerCase" };
const char* xRipEncodeConfiguration_EncodingFormat { "xRipEncode/EncodingFormat" };
const char* xRipEncodeConfiguration_FFMpeg { "xRipEncode/FFMpeg" };
const char* xRipEncodeConfiguration_FFProbe { "xRipEncode/FFProbe" };
const char* xRipEncodeConfiguration_MKVMerge { "xRipEncode/MKVMerge" };
const char* xRipEncodeConfiguration_MKVExtract { "xRipEncode/MKVExtract" };
const char* xRipEncodeConfiguration_Flac { "xRipEncode/Flac" };
const char* xRipEncodeConfiguration_WavPack { "xRipEncode/WavPack" };
const char* xRipEncodeConfiguration_Tags { "xRipEncode/Tags" };
const char* xRipEncodeConfiguration_TagInfos { "xRipEncode/TagInfos" };
// Default values.
const char* xRipEncodeConfiguration_TempDirectory_Default { "/tmp" };
const char* xRipEncodeConfiguration_BackupDirectory_Default { "/tmp" };
const char* xRipEncodeConfiguration_EncodingDirectory_Default { "/tmp" };
const char* xRipEncodeConfiguration_FileNameFormat_Default { "(artist)#(album)(tag)#(tracknr) (trackname)" };
const char* xRipEncodeConfiguration_FileNameReplaceFrom_Default { "?" };
const char* xRipEncodeConfiguration_FileNameReplaceTo_Default { "" };
const bool xRipEncodeConfiguration_FileNameLower_Default = true;
const char* xRipEncodeConfiguration_EncodingFormat_Default { "(artist)/(album)(tag)/(tracknr) (trackname)" };
const char* xRipEncodeConfiguration_FFMpeg_Default { "/usr/bin/ffmpeg" };
const char* xRipEncodeConfiguration_FFProbe_Default { "/usr/bin/ffprobe" };
const char* xRipEncodeConfiguration_MKVMerge_Default { "/usr/bin/mkvmerge" };
const char* xRipEncodeConfiguration_MKVExtract_Default { "/usr/bin/mkvextract" };
const char* xRipEncodeConfiguration_Flac_Default { "/usr/bin/flac" };
const char* xRipEncodeConfiguration_WavPack_Default { "/usr/bin/wavpack" };
const char* xRipEncodeConfiguration_Tags_Default { "| [hd]| [%1.1]| [hd-%1.1]" };
const char* xRipEncodeConfiguration_TagInfos_Default { "CD/Stereo|HD/Stereo|CD/MultiChannel|HD/MultiChannel" };

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

void xRipEncodeConfiguration::setTempDirectory(const QString& directory) {
    if ((directory != getTempDirectory()) && (std::filesystem::is_directory(directory.toStdString()))) {
        settings->setValue(xRipEncodeConfiguration_TempDirectory, directory);
        settings->sync();
        emit updatedTempDirectory();
    }
}

void xRipEncodeConfiguration::setBackupDirectory(const QString& directory) {
    if ((directory != getBackupDirectory()) && (std::filesystem::is_directory(directory.toStdString()))) {
        settings->setValue(xRipEncodeConfiguration_BackupDirectory, directory);
        settings->sync();
        emit updatedBackupDirectory();
    }
}

void xRipEncodeConfiguration::setEncodingDirectory(const QString& directory) {
    if ((directory != getEncodingDirectory()) && (std::filesystem::is_directory(directory.toStdString()))) {
        settings->setValue(xRipEncodeConfiguration_EncodingDirectory, directory);
        settings->sync();
        emit updatedEncodingDirectory();
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

void xRipEncodeConfiguration::setEncodingFormat(const QString& format) {
    if (format != getEncodingFormat()) {
        settings->setValue(xRipEncodeConfiguration_EncodingFormat, format);
        settings->sync();
        emit updatedEncodingFormat();
    }
}

void xRipEncodeConfiguration::setFFMpeg(const QString& path) {
    if ((path != getFFMpeg()) && (std::filesystem::is_regular_file(path.toStdString()))) {
        settings->setValue(xRipEncodeConfiguration_FFMpeg, path);
        settings->sync();
    }
}

void xRipEncodeConfiguration::setFFProbe(const QString& path) {
    if ((path != getFFProbe()) && (std::filesystem::is_regular_file(path.toStdString()))) {
        settings->setValue(xRipEncodeConfiguration_FFProbe, path);
        settings->sync();
    }
}

void xRipEncodeConfiguration::setMKVMerge(const QString& path) {
    if ((path != getMKVMerge()) && (std::filesystem::is_regular_file(path.toStdString()))) {
        settings->setValue(xRipEncodeConfiguration_MKVMerge, path);
        settings->sync();
    }
}

void xRipEncodeConfiguration::setMKVExtract(const QString& path) {
    if ((path != getMKVExtract()) && (std::filesystem::is_regular_file(path.toStdString()))) {
        settings->setValue(xRipEncodeConfiguration_MKVExtract, path);
        settings->sync();
    }
}

void xRipEncodeConfiguration::setFlac(const QString& path) {
    if ((path != getFlac()) && (std::filesystem::is_regular_file(path.toStdString()))) {
        settings->setValue(xRipEncodeConfiguration_Flac, path);
        settings->sync();
    }
}

void xRipEncodeConfiguration::setWavPack(const QString& path) {
    if ((path != getWavPack()) && (std::filesystem::is_regular_file(path.toStdString()))) {
        settings->setValue(xRipEncodeConfiguration_WavPack, path);
        settings->sync();
    }
}

void xRipEncodeConfiguration::setTags(const QStringList& tags) {
    if (tags != getTags()) {
        settings->setValue(xRipEncodeConfiguration_Tags, tags.join('|'));
        settings->sync();
    }
}

QString xRipEncodeConfiguration::getTempDirectory() const {
    return settings->value(xRipEncodeConfiguration_TempDirectory,
                           xRipEncodeConfiguration_TempDirectory_Default).toString();
}

QString xRipEncodeConfiguration::getBackupDirectory() const {
    return settings->value(xRipEncodeConfiguration_BackupDirectory,
                           xRipEncodeConfiguration_BackupDirectory_Default).toString();
}

QString xRipEncodeConfiguration::getEncodingDirectory() const {
    return settings->value(xRipEncodeConfiguration_EncodingDirectory,
                           xRipEncodeConfiguration_EncodingDirectory_Default).toString();
}

QString xRipEncodeConfiguration::getFileNameFormat() const {
    return settings->value(xRipEncodeConfiguration_FileNameFormat,
                           xRipEncodeConfiguration_FileNameFormat_Default).toString();
}

QList<std::pair<QString,QString>> xRipEncodeConfiguration::getFileNameReplace() const {
    // Retrieve current values.
    auto replaceFrom = settings->value(xRipEncodeConfiguration_FileNameReplaceFrom,
                                       xRipEncodeConfiguration_FileNameReplaceFrom_Default).toString();
    auto replaceTo = settings->value(xRipEncodeConfiguration_FileNameReplaceTo,
                                     xRipEncodeConfiguration_FileNameReplaceTo_Default).toString();
    return xRipEncodeConfiguration::stringsToList(replaceFrom, replaceTo);
}

bool xRipEncodeConfiguration::getFileNameLowerCase() const {
    return settings->value(xRipEncodeConfiguration_FileNameLowerCase,
                           xRipEncodeConfiguration_FileNameLower_Default).toBool();
}

QString xRipEncodeConfiguration::getEncodingFormat() const {
    return settings->value(xRipEncodeConfiguration_EncodingFormat,
                           xRipEncodeConfiguration_EncodingFormat_Default).toString();
}

QString xRipEncodeConfiguration::getFFMpeg() const {
    return settings->value(xRipEncodeConfiguration_FFMpeg, xRipEncodeConfiguration_FFMpeg_Default).toString();
}

QString xRipEncodeConfiguration::getFFProbe() const {
    return settings->value(xRipEncodeConfiguration_FFProbe, xRipEncodeConfiguration_FFProbe_Default).toString();
}

QString xRipEncodeConfiguration::getMKVMerge() const {
    return settings->value(xRipEncodeConfiguration_MKVMerge, xRipEncodeConfiguration_MKVMerge_Default).toString();
}

QString xRipEncodeConfiguration::getMKVExtract() const {
    return settings->value(xRipEncodeConfiguration_MKVExtract, xRipEncodeConfiguration_MKVExtract_Default).toString();
}

QString xRipEncodeConfiguration::getFlac() const {
    return settings->value(xRipEncodeConfiguration_Flac, xRipEncodeConfiguration_Flac_Default).toString();
}

QString xRipEncodeConfiguration::getWavPack() const {
    return settings->value(xRipEncodeConfiguration_WavPack, xRipEncodeConfiguration_WavPack_Default).toString();
}

QStringList xRipEncodeConfiguration::getTags() const {
    auto tags = settings->value(xRipEncodeConfiguration_Tags, xRipEncodeConfiguration_Tags_Default).toString();
    return tags.split('|');
}

QStringList xRipEncodeConfiguration::getTagInfos() const {
    auto tagInfos = settings->value(xRipEncodeConfiguration_TagInfos, xRipEncodeConfiguration_TagInfos_Default).toString();
    return tagInfos.split('|');
}

void xRipEncodeConfiguration::updatedConfiguration() {
    // Fire all update signals.
    emit updatedTempDirectory();
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

