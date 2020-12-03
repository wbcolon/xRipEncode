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

#include "xAudioTracksWidget.h"
#include <QGridLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QProgressBar>
#include <QStackedWidget>
#include <QLabel>
#include <QDebug>

xAudioTrackItemWidget::xAudioTrackItemWidget(int track, int offset, QWidget* parent):
        QWidget(parent),
        audioTrackNr(track),
        audioTrackOffset(offset) {
    auto trackLayout = new QGridLayout(this);
    trackSelect = new QCheckBox("", this);
    trackNr = new QLineEdit(this);
    trackStacked = new QStackedWidget(this);
    trackProgress = new QProgressBar(trackStacked);
    trackProgress->setRange(0, 100);
    trackName = new QLineEdit(trackStacked);
    trackStacked->addWidget(trackName);
    trackStacked->addWidget(trackProgress);
    trackStacked->setCurrentWidget(trackName);
    trackNr->setAlignment(Qt::AlignCenter);
    trackNr->setFixedWidth(40);
    trackNr->setText(QString("%1").arg(audioTrackNr+audioTrackOffset, 2, 10, QChar('0')));
    trackLength = new QLabel(this);
    trackLength->setFixedWidth(trackLength->fontMetrics().boundingRect("01:23:45.678").width());
    trackLength->setAlignment(Qt::AlignCenter);
    trackLayout->addWidget(trackSelect, 0, 0);
    trackLayout->addWidget(trackNr, 0, 1);
    trackLayout->addWidget(trackStacked, 0, 2, 1, 10);
    trackLayout->addWidget(trackLength, 0, 12, 1, 1);
    trackLayout->setColumnStretch(1, 0);
    trackLayout->setColumnStretch(2, 2);
    trackLayout->setColumnStretch(1, 0);
    setLayout(trackLayout);
    setFixedHeight(sizeHint().height());
}

int xAudioTrackItemWidget::getAudioTrackNr() const {
    return audioTrackNr;
}

void xAudioTrackItemWidget::setAudioTrackOffset(int offset) {
    audioTrackOffset = offset;
    trackNr->setText(QString("%1").arg(audioTrackNr+audioTrackOffset, 2, 10, QChar('0')));
}

int xAudioTrackItemWidget::getAudioTrackOffset() const {
    return audioTrackOffset;
}

QString xAudioTrackItemWidget::getTrackNr() const {
    return trackNr->text();
}

void xAudioTrackItemWidget::setTrackName(const QString& name) {
    trackName->setText(name);
}

QString xAudioTrackItemWidget::getTrackName() const {
    return trackName->text();
}

void xAudioTrackItemWidget::setTrackLength(const QString& length) {
    trackLength->setText(length);
}

void xAudioTrackItemWidget::setSelected(bool select) {
    trackSelect->setChecked(select);
}

bool xAudioTrackItemWidget::isSelected() const {
    return trackSelect->isChecked();
}

void xAudioTrackItemWidget::trackInput(bool autofill) {
    trackStacked->setCurrentWidget(trackName);
    if (autofill) {
        trackName->setText(tr("track"));
    }
}

void xAudioTrackItemWidget::ripProgress(int progress) {
    if (isSelected()) {
        trackProgress->setFormat(trackName->text()+" - %p%");
        trackStacked->setCurrentWidget(trackProgress);
        trackProgress->setValue(progress);
    }
}

xAudioTracksWidget::xAudioTracksWidget(QWidget* parent):
        QScrollArea(parent),
        audioTracksOffset(0),
        audioMain(nullptr),
        audioLayout(nullptr) {
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setWidgetResizable(true);
}

void xAudioTracksWidget::autofill() {
    for (auto& audioTrack : audioTracks) {
        audioTrack->trackInput(true);
    }
}

void xAudioTracksWidget::setTrackOffset(int offset) {
    if (offset >= 0) {
        audioTracksOffset = offset;
        for (const auto& track : audioTracks) {
            track->setAudioTrackOffset(offset);
        }
    }
}

void xAudioTracksWidget::setTrackNames(const QVector<QString>& names) {
    if (names.count() == audioTracks.count()) {
        for (int track = 0; track < audioTracks.count(); ++track) {
            audioTracks[track]->setTrackName(names[track]);
        }
    }
}

QString xAudioTracksWidget::millisecondsToLabel(qint64 ms) {
    return QString("%1:%2.%3").arg(ms/60000).
            arg((ms/1000)%60, 2, 10, QChar('0')).
            arg(((ms%1000)+5)/10, 2, 10, QChar('0'));
}

void xAudioTracksWidget::setTrackLengths(const QVector<qint64>& times) {
    if (times.count() == audioTracks.count()) {
        for (int track = 0; track < audioTracks.count(); ++track) {
            audioTracks[track]->setTrackLength(xAudioTracksWidget::millisecondsToLabel(times[track]));
        }
    }
}

void xAudioTracksWidget::setTracks(int tracks) {
    // Cleanup.
    for (auto& audioTrack : audioTracks) {
        audioLayout->removeWidget(audioTrack);
        delete audioTrack;
    }
    delete audioLayout;
    delete audioMain;
    // Recreate.
    audioTracks.resize(tracks);
    audioMain = new QWidget();
    audioLayout = new QVBoxLayout(audioMain);
    audioMain->setLayout(audioLayout);
    for (int track = 0; track < tracks; ++track) {
        auto trackItemWidget = new xAudioTrackItemWidget(track+1, audioTracksOffset, audioMain);
        audioLayout->addWidget(trackItemWidget);
        audioTracks[track] = trackItemWidget;
    }
    audioLayout->addStretch(10);
    setWidget(audioMain);
}

void xAudioTracksWidget::selectAll() {
    for (auto& audioTrack : audioTracks) {
        audioTrack->setSelected(true);
    }
}

void xAudioTracksWidget::clear() {
    setTracks(0);
}

QList<std::tuple<int,QString,QString>> xAudioTracksWidget::isSelected() {
    QList<std::tuple<int,QString,QString>> selectedTracks;
    for (const auto& audioTrack : audioTracks) {
        if (audioTrack->isSelected()) {
            selectedTracks.push_back(std::make_tuple(audioTrack->getAudioTrackNr(),
                                                     audioTrack->getTrackNr(),
                                                     audioTrack->getTrackName()));

        }
    }
    return selectedTracks;
}

void xAudioTracksWidget::ripProgress(int track, int progress) {
    // Track are numbered from 1..n but audioTracks index from 0..n-1.
    if ((track > 0) && (track <= audioTracks.count())) {
        audioTracks[track-1]->ripProgress(progress);
    }
}

