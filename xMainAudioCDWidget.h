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

#ifndef __XMAINAUDIOCDWIDGET_H__
#define __XMAINAUDIOCDWIDGET_H__

#include "xAudioCD.h"
#include "xAudioTracksWidget.h"
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

class xMainAudioCDWidget: public QWidget {
    Q_OBJECT

public:
    xMainAudioCDWidget(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~xMainAudioCDWidget() = default;

private slots:
    void detect();
    void eject();
    void cddb();
    void cddbFinished();
    void autofill();
    void rip();
    void ripCancel();
    void ripMessage(int track, const QString& message);
    void ripError(int track, const QString& error, bool abort);
    void ripFinished();
    void directory();
    void fileformat();

private:
    QList<std::pair<int,QString>> getTrackNames();

    QLineEdit* audioCDArtistName;
    QLineEdit* audioCDAlbumName;
    QPushButton* audioCDDetectButton;
    QPushButton* audioCDEjectButton;
    QPushButton* audioCDAutoFillButton;
    QPushButton* audioCDCDDBButton;
    QLineEdit* configurationFileFormat;
    QLineEdit* configurationDirectory;
    xAudioTracksWidget* audioTracks;
    QPushButton* audioTracksSelectButton;
    QPushButton* audioTracksRipButton;
    QPushButton* audioTracksRipCancelButton;
    QTextEdit* consoleText;
    xAudioCD* audioCD;
    xAudioCDLookup* audioCDLookup;
};

#endif
