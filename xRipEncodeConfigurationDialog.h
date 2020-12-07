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
#include "xReplaceWidget.h"

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QListWidget>
#include <QCheckBox>
#include <QSettings>

class xRipEncodeConfigurationDialog:public QDialog {
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param parent pointer to the parent object.
     * @param flags window flags.
     */
    explicit xRipEncodeConfigurationDialog(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    /**
     * Destructor.
     */
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
    /**
     * Select a from/to entry from the replace list.
     *
     * @param item pointer to the selected item.
     */
    void selectReplaceEntry(QListWidgetItem* item);
    /**
     * Add a from/to to the replace list.
     */
    void replaceEntryAdd();
    /**
     * Remove the selected from/to from the replace list.
     */
    void replaceEntryRemove();

private:
    /**
     * Open a dialog in order to select a directory.
     *
     * @param title the title for the directory dialog.
     * @param lineEdit pointer to the line edit object that is updated.
     */
    void openDirectory(const QString& title, QLineEdit* lineEdit);
    /**
     * Open a dialog in order to select a file.
     *
     * @param title the title for the directory dialog.
     * @param lineEdit pointer to the line edit object that is updated.
     */
    void openFile(const QString& title, QLineEdit* lineEdit);
    /**
     * Split an entry into a pair of from/to.
     *
     * @param entry the entry as string.
     * @return a pair of from/to strings.
     */
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
    xReplaceWidget* replaceList;
};

#endif
