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

#ifndef __XMAINMOVIEFILEWIDGET_H__
#define __XMAINMOVIEFILEWIDGET_H__

#include "xMovieFile.h"
#include "xAudioTracksWidget.h"
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QListWidget>
#include <QTextEdit>
#include <QWidget>

class xMainMovieFileWidget: public QWidget {
Q_OBJECT

public:
    explicit xMainMovieFileWidget(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~xMainMovieFileWidget() override = default;

signals:
    void audioFiles(QList<xAudioFile*> files);

private slots:
    void openFile();
    void autofill();
    void rip();
    void ripCancel();
    void ripMessage(int track, const QString& message);
    void ripError(int track, const QString& error, bool abort);
    void ripFinished();

    void audioStreamInfos(const QStringList& infos);
    void audioStreamInfoSelection();
    void trackLengths(const QVector<qint64>& lengths);
    void messages(const QString& msg);

private:
    QList<std::pair<int,QString>> getTrackNames(const QString& tag);
    QList<xAudioFile*> getAudioFiles(const QString& tag, int tagId);

    QLineEdit* movieFileArtistName;
    QLineEdit* movieFileAlbumName;
    QLineEdit* movieFileName;
    QPushButton* movieFileAutofillButton;
    QPushButton* movieFileAnalyzeButton;
    QSpinBox* movieFileTrackOffset;
    QCheckBox* movieFileAudioDownMix;
    QCheckBox* movieFileAudioStreamTag;
    QListWidget* movieFileAudioStreamInfos;
    xAudioTracksWidget* movieAudioTracks;
    QPushButton* movieAudioTracksSelectButton;
    QPushButton* movieAudioTracksRipButton;
    QPushButton* movieAudioTracksRipCancelButton;
    QTextEdit* consoleText;
    xMovieFile* movieFile;
};

#endif
