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
    static std::pair<QString,QString> splitReplaceEntries(const QString& entry);

    QLineEdit* fileTempDirectoryInput;
    QLineEdit* fileFileNameFormatInput;
    QCheckBox* fileFileNameLowerCase;
    QLineEdit* replaceFromInput;
    QLineEdit* replaceToInput;
    QListWidget* replaceList;

};

#endif
