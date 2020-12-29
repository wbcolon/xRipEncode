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
#include <QDebug>

xEncodingTrackItemWidget::xEncodingTrackItemWidget(xAudioFile* file, QWidget* parent):
        QWidget(parent),
        audioFile(file) {
    auto mainLayout = new QGridLayout(this);
    trackSelect = new QCheckBox("", this);
    trackSelect->setFixedWidth(trackSelect->sizeHint().width());
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
    encodedFileName = new QLineEdit(mainStacked);
    encodedFileName->setAlignment(Qt::AlignLeft);
    encodedFileName->setReadOnly(true);
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
    // Connect changes for line edits for artist, album, tag, and track number.
    // No lambda function because we want to disconnect for the set functions.
    connect(artistName, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedArtist);
    connect(albumName, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedAlbum);
    connect(tagName, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedTag);
    connect(trackNr, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedTrackNr);
}

void xEncodingTrackItemWidget::setArtist(const QString& artist) {
    // Do not trigger trigger an updateArtist signal if we set it directly.
    disconnect(artistName, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedArtist);
    artistName->setText(artist);
    connect(artistName, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedArtist);
    updateEncodedFileName();
}

void xEncodingTrackItemWidget::setAlbum(const QString& album) {
    // Do not trigger trigger an updateAlbum signal if we set it directly.
    disconnect(albumName, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedAlbum);
    albumName->setText(album);
    connect(albumName, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedAlbum);
    updateEncodedFileName();
}

void xEncodingTrackItemWidget::setTag(const QString& tag) {
    // Do not trigger trigger an updateTag signal if we set it directly.
    disconnect(tagName, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedTag);
    tagName->setText(tag);
    connect(tagName, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedTag);
    updateEncodedFileName();
}

void xEncodingTrackItemWidget::setTrackNr(const QString& nr) {
    // Do not trigger trigger an updateTrackNumber signal if we set it directly.
    disconnect(trackNr, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedTrackNr);
    trackNr->setText(nr);
    connect(trackNr, &QLineEdit::textChanged, this, &xEncodingTrackItemWidget::updatedTrackNr);
    updateEncodedFileName();
}

void xEncodingTrackItemWidget::setTrackName(const QString& name) {
    trackName->setText(name);
    updateEncodedFileName();
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

QString xEncodingTrackItemWidget::getEncodedFileName() const {
    return encodedFileName->text();
}

quint64 xEncodingTrackItemWidget::getJobId() const {
    if (audioFile) {
        return audioFile->getJobId();
    } else {
        return 0;
    }
}

xAudioFile* xEncodingTrackItemWidget::getAudioFile() const {
    return audioFile;
}

void xEncodingTrackItemWidget::setSelected(bool select) {
    trackSelect->setChecked(select);
}

bool xEncodingTrackItemWidget::isSelected() const {
    return trackSelect->isChecked();
}

void xEncodingTrackItemWidget::setEncodedFormat(const QString& format) {
   encodedFormat = format;
   updateEncodedFileName();
}

void xEncodingTrackItemWidget::updatedArtist(const QString& text) {
    auto pos = artistName->cursorPosition();
    setArtist(text);
    artistName->setCursorPosition(pos);
    // Signal to tracks widget in order to allow for smart artist update.
    emit updateArtist(this);
}

void xEncodingTrackItemWidget::updatedAlbum(const QString& text) {
    auto pos = albumName->cursorPosition();
    setAlbum(text);
    albumName->setCursorPosition(pos);
    // Signal to tracks widget in order to allow for smart album update.
    emit updateAlbum(this);
}

void xEncodingTrackItemWidget::updatedTag(const QString& text) {
    auto pos = tagName->cursorPosition();
    setTag(text);
    tagName->setCursorPosition(pos);
    // Signal to tracks widget in order to allow for smart tag update.
    emit updateTag(this);
}

void xEncodingTrackItemWidget::updatedTrackNr(const QString& text) {
    // Does the text represents a valid integer.
    if (text.toInt() > 0)  {
        auto pos = trackNr->cursorPosition();
        setTrackNr(text);
        trackNr->setCursorPosition(pos);
        // Signal to tracks widget in order to allow for smart track number update.
        emit updateTrackNr(this);
    }
}

void xEncodingTrackItemWidget::updateEncodedFileName() {
    auto encodedName = encodedFormat;
    encodedName.replace("(artist)", getArtist());
    encodedName.replace("(album)", getAlbum());
    encodedName.replace("(tracknr)", getTrackNr());
    encodedName.replace("(trackname)", getTrackName());
    encodedName.replace("(tag)", getTag());
    encodedFileName->setText(encodedName);
}

void xEncodingTrackItemWidget::viewOutput(bool autofill) {
    mainStacked->setCurrentWidget(encodedFileName);
    if (autofill) {
        artistName->setText(tr("artist"));
        albumName->setText(tr("album"));
        tagName->setText(tr(""));
        trackName->setText(tr("track"));
    }
    editInfo->setEnabled(true);
    editInfo->setText(tr("Edit"));
    updateEncodedFileName();
}

void xEncodingTrackItemWidget::viewInput() {
    mainStacked->setCurrentWidget(trackInfo);
    editInfo->setEnabled(true);
    editInfo->setText(tr("Done"));
    updateEncodedFileName();
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
    } else {
        viewOutput();
    }
}



xEncodingTracksWidget::xEncodingTracksWidget(QWidget* parent):
        QScrollArea(parent),
        audioMain(nullptr),
        audioLayout(nullptr),
        updateArtistEnabled(false),
        updateAlbumEnabled(false),
        updateTagEnabled(false),
        updateTrackNrEnabled(false) {
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setWidgetResizable(true);
}

void xEncodingTracksWidget::viewOutput(bool autofill) {
    for (auto& audioTrack : encodingTracks) {
        audioTrack->viewOutput(autofill);
    }
}

void xEncodingTracksWidget::viewInput() {
    for (auto& audioTrack : encodingTracks) {
        audioTrack->viewInput();
    }
}

void xEncodingTracksWidget::setTracks(const QVector<xAudioFile*>& tracks) {
    // Cleanup.
    for (auto& audioTrack : encodingTracks) {
        audioLayout->removeWidget(audioTrack);
        delete audioTrack;
    }
    delete audioLayout;
    delete audioMain;
    // Recreate.
    encodingTracks.resize(tracks.count());
    audioMain = new QWidget();
    audioLayout = new QVBoxLayout(audioMain);
    audioMain->setLayout(audioLayout);
    quint64 prevJobId = 0;
    for (int track = 0; track < tracks.count(); ++track) {
        auto trackItemWidget = new xEncodingTrackItemWidget(tracks[track]);
        // Add a separator if the job Id changes.
        if ((prevJobId != 0) && (prevJobId != trackItemWidget->getJobId())) {
            audioLayout->addSpacing(50);
        }
        audioLayout->addWidget(trackItemWidget);
        encodingTracks[track] = trackItemWidget;
        // Connect update signals.
        connect(trackItemWidget, &xEncodingTrackItemWidget::updateArtist, this, &xEncodingTracksWidget::updateArtist);
        connect(trackItemWidget, &xEncodingTrackItemWidget::updateAlbum, this, &xEncodingTracksWidget::updateAlbum);
        connect(trackItemWidget, &xEncodingTrackItemWidget::updateTag, this, &xEncodingTracksWidget::updateTag);
        connect(trackItemWidget, &xEncodingTrackItemWidget::updateTrackNr, this, &xEncodingTracksWidget::updateTrackNr);
        // Update job ID.
        prevJobId = trackItemWidget->getJobId();
    }
    updateTabOrder();
    audioLayout->addStretch(10);
    setWidget(audioMain);
}

void xEncodingTracksWidget::updateTabOrder() {
    auto noTracks = encodingTracks.count();
    if (noTracks <= 1) {
        return;
    }
    // Order the track selection check-boxes
    for (auto i = 1; i < noTracks; ++i) {
        setTabOrder(encodingTracks[i-1]->trackSelect, encodingTracks[i]->trackSelect);
    }
    // Transition to artist name and then order artist name.
    setTabOrder(encodingTracks[noTracks-1]->trackSelect, encodingTracks[0]->artistName);
    for (auto i = 1; i < noTracks; ++i) {
        setTabOrder(encodingTracks[i-1]->artistName, encodingTracks[i]->artistName);
    }
    // Transition to album name and then order album name.
    setTabOrder(encodingTracks[noTracks-1]->artistName, encodingTracks[0]->albumName);
    for (auto i = 1; i < noTracks; ++i) {
        setTabOrder(encodingTracks[i-1]->albumName, encodingTracks[i]->albumName);
    }
    // Transition to tag name and then order tag name.
    setTabOrder(encodingTracks[noTracks-1]->albumName, encodingTracks[0]->tagName);
    for (auto i = 1; i < noTracks; ++i) {
        setTabOrder(encodingTracks[i-1]->tagName, encodingTracks[i]->tagName);
    }
    // Transition to nr and then order nr.
    setTabOrder(encodingTracks[noTracks-1]->tagName, encodingTracks[0]->trackNr);
    for (auto i = 1; i < noTracks; ++i) {
        setTabOrder(encodingTracks[i-1]->trackNr, encodingTracks[i]->trackNr);
    }
    // Transition to track name and then order track name.
    setTabOrder(encodingTracks[noTracks-1]->trackNr, encodingTracks[0]->trackName);
    for (auto i = 1; i < noTracks; ++i) {
        setTabOrder(encodingTracks[i-1]->trackName, encodingTracks[i]->trackName);
    }
}

void xEncodingTracksWidget::setEncodedFormat(const QString& format) {
    for (const auto& track : encodingTracks) {
        track->setEncodedFormat(format);
    }
}

QList<xEncodingTrackItemWidget*> xEncodingTracksWidget::getSelected() {
    QList<xEncodingTrackItemWidget*> selected;
    for (auto i = 0; i < encodingTracks.count(); ++i) {
        if (encodingTracks[i]->isSelected()) {
            selected.push_back(encodingTracks[i]);
        }
    }
    return selected;
}

void xEncodingTracksWidget::selectAll() {
    for (auto& audioTrack : encodingTracks) {
        audioTrack->setSelected(true);
    }
}

void xEncodingTracksWidget::deselectAll() {
    for (auto& audioTrack : encodingTracks) {
        audioTrack->setSelected(false);
    }
}

void xEncodingTracksWidget::setUpdateArtist(bool enabled) {
    updateArtistEnabled = enabled;
}

void xEncodingTracksWidget::setUpdateAlbum(bool enabled) {
    updateAlbumEnabled = enabled;
}

void xEncodingTracksWidget::setUpdateTag(bool enabled) {
    updateTagEnabled = enabled;
}

void xEncodingTracksWidget::setUpdateTrackNr(bool enabled) {
    updateTrackNrEnabled = enabled;
}

void xEncodingTracksWidget::clear() {
    // Cleanup.
    setTracks(QVector<xAudioFile*>());
}

void xEncodingTracksWidget::ripProgress(int track, int progress) {
    // Track are numbered from 1..n but encodingTracks index from 0..n-1.
    if ((track > 0) && (track <= encodingTracks.count())) {
        encodingTracks[track-1]->ripProgress(progress);
    }
}

void xEncodingTracksWidget::updateArtist(xEncodingTrackItemWidget* item) {
    if (updateArtistEnabled) {
        auto artist = item->getArtist();
        auto jobId = item->getJobId();
        for (auto& tracks : encodingTracks) {
            // Update only if job ID matches and if item is the one triggering the event.
            if ((tracks->getJobId() == jobId) && (tracks != item)) {
                tracks->setArtist(artist);
            }
        }
    }
}

void xEncodingTracksWidget::updateAlbum(xEncodingTrackItemWidget* item) {
    if (updateAlbumEnabled) {
        auto album = item->getAlbum();
        auto jobId = item->getJobId();
        for (auto& tracks : encodingTracks) {
            // Update only if job ID matches and if item is the one triggering the event.
            if ((tracks->getJobId() == jobId) && (tracks != item)) {
                tracks->setAlbum(album);
            }
        }
    }
}

void xEncodingTracksWidget::updateTag(xEncodingTrackItemWidget* item) {
    if (updateTagEnabled) {
        auto tag = item->getTag();
        auto jobId = item->getJobId();
        for (auto& tracks : encodingTracks) {
            // Update only if job ID matches and if item is the one triggering the event.
            if ((tracks->getJobId() == jobId) && (tracks != item)) {
                tracks->setTag(tag);
            }
        }
    }
}

void xEncodingTracksWidget::updateTrackNr(xEncodingTrackItemWidget* item) {
    if (updateTrackNrEnabled) {
        auto index = encodingTracks.indexOf(item);
        if ((index >= 0) || (index < encodingTracks.count())) {
            auto jobId = item->getJobId();
            auto currentTrackNr = item->getTrackNr().toInt() + 1;
            for (auto i = index+1; i < encodingTracks.count(); ++i) {
                if (encodingTracks[i]->getJobId() == jobId) {
                    encodingTracks[i]->setTrackNr(QString("%1").arg(currentTrackNr++, 2, 10, QChar('0')));
                } else {
                    break;
                }
            }
        }
    }
}
