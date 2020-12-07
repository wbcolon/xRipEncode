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

#ifndef __XMAINENCODINGWIDGET_H__
#define __XMAINENCODINGWIDGET_H__

#include "xAudioFile.h"
#include "xEncodingTracksWidget.h"
#include <QTabWidget>
#include <QRadioButton>
#include <QWidget>

class xMainEncodingWidget:public QWidget {
    Q_OBJECT

public:
    /**
     * Constructor. Create the encoding widget.
     *
     * @param parent pointer to the parent widget.
     * @param flags window flags for the widget.
     */
    explicit xMainEncodingWidget(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    /**
     * Destructor. Cleanup remaining audio file objects.
     */
    ~xMainEncodingWidget() noexcept override;

public slots:
    /**
     * Add the audio file objects to the currently encoded files. Split by tag ID.
     *
     * @param files the list of audio file objects to be added.
     */
    void audioFiles(const QList<xAudioFile*>& files);

private slots:
    void setAllEnabled(bool enabled);
    /**
     * Switch to the input view for all encoding track items.
     */
    void editAll();
    /**
     * Switch to the output view for all encoding track items.
     */
    void outputAll();
    /**
     * Select all encoding track items.
     */
    void selectAll();
    /**
     * Deselect all encoding track items.
     */
    void deselectAll();
    /**
     * Start encoding thread (flac mode) with the currently selected encoding track items.
     * Disable buttons during encoding.
     */
    void encode();
    /**
     * Enable buttons after the encoding thread is finished.
     */
    void encodeFinished();
    /**
     * Start encoding thread (wavpack mode) with the currently selected encoding track items.
     * Disable buttons during encoding.
     */
    void backup();
    /**
     * Enable buttons after the encoding thread is finished.
     */
    void backupFinished();
    /**
     * Remove all items of the current encoding tab. Delete the corresponding audio file objects.
     */
    void clear();

private:
    /**
     * Force an update of the encoded output file name for all items.
     */
    void updateEncodedFileNames();
    /**
     * Create encoding tab widget with encoding track items.
     */
    void createEncodingTracksWidgets();

    QVector<QVector<xAudioFile*>> encodingAudioFiles;
    QVector<xEncodingTracksWidget*> encodingTracksWidgets;
    QTabWidget* encodingTracksTab;
    QLineEdit* formatEncodingFormatInput;
    QLineEdit* formatFileFormatInput;
    QPushButton* encodeSelectAllButton;
    QPushButton* encodeDeselectAllButton;
    QPushButton* encodeEditAllButton;
    QPushButton* encodeOutputAllButton;
    QPushButton* encodeBackupButton;
    QPushButton* encodeEncodeButton;
    QPushButton* encodeClearButton;
    QRadioButton* encodeUseEncodingButton;
    QRadioButton* encodeUseFileButton;
    xAudioFileEncoding* encoding;
};

#endif
