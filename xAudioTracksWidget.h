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

#ifndef __XAUDIOTRACKSWIDGET_H__
#define __XAUDIOTRACKSWIDGET_H__

#include <QScrollArea>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QProgressBar>
#include <QList>
#include <QWidget>

class xAudioTrackItemWidget:public QWidget {
    Q_OBJECT

public:
    xAudioTrackItemWidget(int track, int offset=0, QWidget* parent=nullptr);
    ~xAudioTrackItemWidget() = default;

    int getAudioTrackNr();
    void setAudioTrackOffset(int offset);
    int getAudioTrackOffset();

    QString getTrackNr();
    void setTrackName(const QString& name);
    QString getTrackName();

    void setSelected(bool select);
    bool isSelected() const;

    void trackInput(bool autofill=false);
    void ripProgress(int progress);

private:
    int audioTrackNr;
    int audioTrackOffset;
    QCheckBox* trackSelect;
    QLineEdit* trackNr;
    QLineEdit* trackName;
    QProgressBar* trackProgress;
    QStackedWidget* trackStacked;
};

class xAudioTracksWidget:public QScrollArea {
    Q_OBJECT

public:
    xAudioTracksWidget(QWidget* parent=nullptr);
    ~xAudioTracksWidget() = default;

    void autofill();
    void setTracks(int tracks);
    void setTrackNames(const QVector<QString>& names);
    void setTrackOffset(int offset);
    void clear();

    QList<std::tuple<int,QString,QString>> isSelected();

public slots:
    void selectAll();
    void ripProgress(int track, int progress);

private:
    int audioTracksOffset;
    QWidget* audioMain;
    QVBoxLayout* audioLayout;
    QVector<xAudioTrackItemWidget*> audioTracks;
};

#endif
