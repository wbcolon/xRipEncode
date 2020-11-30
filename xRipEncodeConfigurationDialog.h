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

#ifndef __XRIPENCODECONFIGURATIONDIALOG_H__
#define __XRIPENCODECONFIGURATIONDIALOG_H__

#include "xRipEncodeConfiguration.h"

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QListWidget>
#include <QCheckBox>
#include <QSettings>

class xRipEncodeConfigurationDialog:public QDialog {
    Q_OBJECT

public:
    explicit xRipEncodeConfigurationDialog(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~xRipEncodeConfigurationDialog() override = default;

public slots:
    /**
     * Load the current configuration settings.
     */
    void loadSettings();

private slots:
    /**
     * Save any updates into the configuration.
     */
    void saveSettings();
    void selectReplaceEntry(QListWidgetItem* item);
    void replaceEntryAdd();
    void replaceEntryRemove();

private:
    void openDirectory(const QString& title, QLineEdit* lineEdit);
    void openFile(const QString& title, QLineEdit* lineEdit);
    static std::pair<QString,QString> splitReplaceEntries(const QString& entry);

    QLineEdit* fileTempDirectoryInput;
    QLineEdit* fileBackupDirectoryInput;
    QLineEdit* fileEncodingDirectoryInput;
    QLineEdit* fileFFMpegInput;
    QLineEdit* fileFFProbeInput;
    QLineEdit* fileMKVMergeInput;
    QLineEdit* fileMKVExtractInput;
    QLineEdit* fileFlacInput;
    QLineEdit* fileWavPackInput;
    QLineEdit* formatEncodingFormatInput;
    QLineEdit* formatFileNameFormatInput;
    QCheckBox* formatFileNameLowerCase;
    QLineEdit* replaceFromInput;
    QLineEdit* replaceToInput;
    QListWidget* replaceList;

};

#endif
