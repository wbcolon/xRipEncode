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
    explicit xMainEncodingWidget(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~xMainEncodingWidget() noexcept override;

public slots:
    void audioFiles(const QList<xAudioFile*>& files);

private slots:
    void setAllEnabled(bool enabled);
    void editAll();
    void finishAll();
    void selectAll();
    void deselectAll();
    void encode();
    void encodeFinished();
    void backup();
    void backupFinished();
    void clear();

private:
    void updateEncodedFileNames();
    void createEncodingTracksWidgets();

    QVector<QVector<xAudioFile*>> encodingAudioFiles;
    QVector<xEncodingTracksWidget*> encodingTracksWidgets;
    QTabWidget* encodingTracksTab;
    QLineEdit* formatEncodingFormatInput;
    QLineEdit* formatFileFormatInput;
    QPushButton* encodeSelectAllButton;
    QPushButton* encodeDeselectAllButton;
    QPushButton* encodeEditAllButton;
    QPushButton* encodeFinishAllButton;
    QPushButton* encodeBackupButton;
    QPushButton* encodeEncodeButton;
    QPushButton* encodeClearButton;
    QRadioButton* encodeUseEncodingButton;
    QRadioButton* encodeUseFileButton;
    xAudioFileEncoding* encoding;
};

#endif
