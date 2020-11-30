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

#include "xEncodingTracksWidget.h"
#include <QGridLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QProgressBar>
#include <QStackedWidget>
#include <QLabel>
#include <QDebug>

xEncodingTrackItemWidget::xEncodingTrackItemWidget(xAudioFile* file, QWidget* parent):
        QWidget(parent),
        audioFile(file) {
    auto mainLayout = new QGridLayout(this);
    trackSelect = new QCheckBox("", this);
    editInfo = new QPushButton(tr("Edit"), this);
    editInfo->setContentsMargins(0,0,0,0);
    // Stacked widget for different views.
    mainStacked = new QStackedWidget(this);
    // Setup track info. Compact one row format.
    trackInfo = new QWidget(mainStacked);
    trackInfo->setContentsMargins(0,0,0,0);
    artistName = new QLineEdit(trackInfo);
    albumName = new QLineEdit(trackInfo);
    tagName = new QLineEdit(trackInfo);
    trackNr = new QLineEdit(trackInfo);
    trackNr->setAlignment(Qt::AlignCenter);
    trackNr->setFixedWidth(40);
    trackName = new QLineEdit(trackInfo);
    auto trackInfoLayout = new QGridLayout(trackInfo);
    trackInfoLayout->setContentsMargins(0,0,0,0);
    trackInfoLayout->addWidget(artistName, 0, 0, 1, 3);
    trackInfoLayout->addWidget(albumName, 0, 3, 1, 3);
    trackInfoLayout->addWidget(tagName, 0, 6);
    trackInfoLayout->addWidget(trackNr, 0, 7);
    trackInfoLayout->addWidget(trackName, 0, 8, 1, 8);
    trackInfoLayout->setColumnStretch(0, 2);
    trackInfoLayout->setColumnStretch(3, 2);
    trackInfoLayout->setColumnStretch(6, 0);
    trackInfoLayout->setColumnStretch(7, 0);
    trackInfoLayout->setColumnStretch(8, 2);
    trackInfo->setLayout(trackInfoLayout);
    // Setup encoded (output) file name view.
    encodedFileName = new QLabel(mainStacked);
    encodedFileName->setAlignment(Qt::AlignLeft);
    encodedFileName->setWordWrap(true);
    // Setup progress bar for encoding.
    encodedProgress = new QProgressBar(mainStacked);
    encodedProgress->setRange(0, 100);
    // Fill stacked widget.
    mainStacked->addWidget(trackInfo);
    mainStacked->addWidget(encodedFileName);
    mainStacked->addWidget(encodedProgress);
    mainStacked->setCurrentWidget(encodedFileName);
    mainLayout->addWidget(trackSelect, 0, 0, 1, 1);
    mainLayout->addWidget(mainStacked, 0, 1, 1, 12);
    mainLayout->setColumnMinimumWidth(13, 20);
    mainLayout->setColumnStretch(13, 0);
    mainLayout->addWidget(editInfo, 0, 14, 1, 1);
    setLayout(mainLayout);
    setFixedHeight(sizeHint().height());
    // Connect button to toggle view.
    connect(editInfo, &QPushButton::pressed, this, &xEncodingTrackItemWidget::toggleViews);
    // Populate the widget with the track info.
    if (audioFile) {
        setArtist(audioFile->getArtist());
        setAlbum(audioFile->getAlbum());
        setTag(audioFile->getTag());
        setTrackNr(audioFile->getTrackNr());
        setTrackName(audioFile->getTrackName());
    }
}

void xEncodingTrackItemWidget::setArtist(const QString& artist) {
    artistName->setText(artist);
}

void xEncodingTrackItemWidget::setAlbum(const QString& album) {
    albumName->setText(album);
}

void xEncodingTrackItemWidget::setTag(const QString& tag) {
    tagName->setText(tag);
}

void xEncodingTrackItemWidget::setTrackNrOffset(int offset) {
    trackNr->setText(QString("%1").arg(audioFile->getTrackNr().toInt()+offset, 2, 10, QChar('0')));
}

void xEncodingTrackItemWidget::setTrackNr(const QString& nr) {
    trackNr->setText(nr);
}

void xEncodingTrackItemWidget::setTrackName(const QString& name) {
    trackName->setText(name);
}


QString xEncodingTrackItemWidget::getArtist() const {
    return artistName->text();
}

QString xEncodingTrackItemWidget::getAlbum() const {
    return albumName->text();
}

QString xEncodingTrackItemWidget::getTag() const {
    return tagName->text();
}

QString xEncodingTrackItemWidget::getTrackNr() const {
    return trackNr->text();
}

QString xEncodingTrackItemWidget::getTrackName() const {
    return trackName->text();
}

void xEncodingTrackItemWidget::setSelected(bool select) {
    trackSelect->setChecked(select);
}

bool xEncodingTrackItemWidget::isSelected() const {
    return trackSelect->isChecked();
}

void xEncodingTrackItemWidget::viewOutput(bool autofill) {
    mainStacked->setCurrentWidget(encodedFileName);
    if (autofill) {
        artistName->setText(tr("artist"));
        albumName->setText(tr("album"));
        tagName->setText(tr(""));
        trackName->setText(tr("track"));
        encodedFileName->setText("artist/album/nr track");
    }
}

void xEncodingTrackItemWidget::viewInput() {
    mainStacked->setCurrentWidget(trackInfo);
}

void xEncodingTrackItemWidget::ripProgress(int progress) {
    if (isSelected()) {
        encodedProgress->setFormat(encodedFileName->text()+" - %p%");
        mainStacked->setCurrentWidget(encodedProgress);
        encodedProgress->setValue(progress);
        editInfo->setEnabled(false);
    }
}

void xEncodingTrackItemWidget::toggleViews() {
    // Use button to determine what to show.
    if (editInfo->text() == tr("Edit")) {
        viewInput();
        editInfo->setText(tr("Done"));
    } else {
        viewOutput();
        editInfo->setText(tr("Edit"));
    }
}

xEncodingTracksWidget::xEncodingTracksWidget(QWidget* parent):
        QScrollArea(parent),
        audioMain(nullptr),
        audioLayout(nullptr) {
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setWidgetResizable(true);
    setTracks(20);
    autofill();
    encodingTracks[1]->setSelected(true);
    encodingTracks[2]->setSelected(true);
    ripProgress(2, 10);
    ripProgress(3, 30);
}

void xEncodingTracksWidget::autofill() {
    for (auto& audioTrack : encodingTracks) {
        audioTrack->viewOutput(true);
    }
}

void xEncodingTracksWidget::setTrackOffset(int offset) {
    if (offset >= 0) {
        for (const auto& track : encodingTracks) {
            track->setTrackNrOffset(offset);
        }
    }
}

void xEncodingTracksWidget::setTracks(const QVector<xAudioFile*>& files) {
    if (files.count() == encodingTracks.count()) {
        for (int track = 0; track < encodingTracks.count(); ++track) {
            //encodingTracks[track]->setTrackName(names[track]);
        }
    }
}

void xEncodingTracksWidget::setTracks(int tracks) {
    // Cleanup.
    for (auto& audioTrack : encodingTracks) {
        audioLayout->removeWidget(audioTrack);
        delete audioTrack;
    }
    delete audioLayout;
    delete audioMain;
    // Recreate.
    encodingTracks.resize(tracks);
    audioMain = new QWidget();
    audioLayout = new QVBoxLayout(audioMain);
    audioMain->setLayout(audioLayout);
    for (int track = 0; track < tracks; ++track) {
        auto trackItemWidget = new xEncodingTrackItemWidget(nullptr);
        audioLayout->addWidget(trackItemWidget);
        encodingTracks[track] = trackItemWidget;
    }
    setWidget(audioMain);
}

void xEncodingTracksWidget::selectAll() {
    for (auto& audioTrack : encodingTracks) {
        audioTrack->setSelected(true);
    }
}

void xEncodingTracksWidget::clear() {
    setTracks(0);
}

#if 0
QList<std::tuple<int,QString,QString>> xEncodingTracksWidget::isSelected() {
    QList<std::tuple<int,QString,QString>> selectedTracks;
    for (const auto& audioTrack : encodingTracks) {
        if (audioTrack->isSelected()) {
            selectedTracks.push_back(std::make_tuple(audioTrack->getAudioTrackNr(),
                                                     audioTrack->getTrackNr(),
                                                     audioTrack->getTrackName()));

        }
    }
    return selectedTracks;
}
#endif

void xEncodingTracksWidget::ripProgress(int track, int progress) {
    // Track are numbered from 1..n but encodingTracks index from 0..n-1.
    if ((track > 0) && (track <= encodingTracks.count())) {
        encodingTracks[track-1]->ripProgress(progress);
    }
}

