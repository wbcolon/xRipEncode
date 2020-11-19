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

#include "xMainAudioRipperWidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QTableWidget>
#include <QScrollArea>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QCheckBox>
#include <QDebug>

class xAudioTrackItem:public QGroupBox {
public:
    xAudioTrackItem(int track, QWidget* parent=nullptr);
    ~xAudioTrackItem() = default;

};

xAudioTrackItem::xAudioTrackItem(int track, QWidget* parent):
        QGroupBox(QString("Track %1").arg(track, 2, 10, QChar('0')), parent) {
    auto trackLayout = new QGridLayout(this);
    auto trackSelect = new QCheckBox("", this);
    auto trackName = new QLineEdit(this);
    trackLayout->addWidget(trackSelect, 0, 0);
    trackLayout->addWidget(trackName, 0, 1, 1, 10);
    setLayout(trackLayout);
    qDebug() << "XAUDIO SIZE: " << sizeHint();
}




xMainAudioRipperWidget::xMainAudioRipperWidget(QWidget *parent, Qt::WindowFlags flags):
        QWidget(parent, flags) {

    auto mainLayout = new QGridLayout(this);
    // Audio CD Box .....
    auto audioCdBox = new QGroupBox(tr("Audio CD"), this);
    auto audioCdDetectButton = new QPushButton(tr("Detect"), audioCdBox);
    auto audioCdEjectButton = new QPushButton(tr("Eject"), audioCdBox);
    auto audioCdCddbButton = new QPushButton(tr("CDDB"), audioCdBox);
    auto audioCdArtistName = new QLineEdit(audioCdBox);
    auto audioCdAlbumName = new QLineEdit(audioCdBox);
    auto audioCdLayout = new QGridLayout();

    audioCdLayout->setRowMinimumHeight(1, 20);
    audioCdLayout->addWidget(audioCdDetectButton, 0, 0, 1, 2);
    audioCdLayout->addWidget(audioCdEjectButton, 0, 2, 1, 2);
    audioCdLayout->addWidget(audioCdCddbButton, 0, 4, 1, 2);
    audioCdLayout->setRowStretch(1, 0);
    audioCdLayout->addWidget(new QLabel(tr("Artist"), audioCdBox), 2, 0, 1, 1);
    audioCdLayout->addWidget(audioCdArtistName, 3, 0, 1, 6);
    audioCdLayout->addWidget(new QLabel(tr("Album"), audioCdBox), 4, 0, 1, 1);
    audioCdLayout->addWidget(audioCdAlbumName, 5, 0, 1, 6);
    audioCdBox->setLayout(audioCdLayout);

    auto configurationBox = new QGroupBox(tr("Configuration"), this);
    auto configurationFileFormat = new QLineEdit(configurationBox);
    auto configurationDirectory = new QLineEdit(configurationBox);
    auto configurationLayout = new QGridLayout();
    configurationLayout->addWidget(new QLabel(tr("File Format"), configurationBox), 0, 0);
    configurationLayout->addWidget(configurationFileFormat, 1, 0, 1, 6);
    configurationLayout->addWidget(new QLabel(tr("Directory"), configurationBox), 2, 0);
    configurationLayout->addWidget(configurationDirectory, 3, 0, 1, 6);
    configurationBox->setLayout(configurationLayout);

    // Audio tracks box.
    auto audioTracksBox = new QGroupBox(tr("Audio Tracks"), this);
    auto audioTracksSelectButton = new QPushButton(tr("Select all"), audioTracksBox);
    audioTracksList = new QListWidget(audioTracksBox);
    auto audioTracksLayout = new QGridLayout();
    audioTracksLayout->addWidget(audioTracksSelectButton, 0, 0, 1, 2);
    audioTracksLayout->setRowMinimumHeight(1, 20);
    audioTracksLayout->setRowStretch(1, 0);
    audioTracksLayout->addWidget(audioTracksList, 2, 0, 6, 2);
    audioTracksBox->setLayout(audioTracksLayout);

    mainLayout->addWidget(configurationBox, 0, 0, 3, 4);
    mainLayout->addWidget(audioCdBox, 3, 0, 4, 4);
    mainLayout->addWidget(audioTracksBox, 0, 4, 7, 4);

    // Create audio ripper object.
    audioRipper = new xAudioRipper(this);
    // Connect Buttons
    connect(audioCdDetectButton, &QPushButton::pressed, this, &xMainAudioRipperWidget::detectAudioCD);
    connect(audioCdEjectButton, &QPushButton::pressed, this, &xMainAudioRipperWidget::ejectAudioCD);
}


void xMainAudioRipperWidget::detectAudioCD() {
    // Clear list.
    audioTracksList->clear();
    // Check for audio CD.
    if (audioRipper->detect()) {
        // Fill in an item for each track on the audio CD.
        auto noTracks = audioRipper->getTracks();
        qDebug() << "xMainAudioRipperWidget: " << noTracks;
        for (int track = 1; track <= noTracks; ++track) {
            auto trackItem = new QListWidgetItem(audioTracksList);
            auto trackItemWidget = new xAudioTrackItem(track, audioTracksList);
            audioTracksList->addItem(trackItem);
            audioTracksList->setItemWidget(trackItem, trackItemWidget);
            trackItem->setSizeHint(trackItemWidget->sizeHint());
        }
    }
}

void xMainAudioRipperWidget::ejectAudioCD() {
    // Clear list and eject device.
    audioTracksList->clear();
    audioRipper->eject();
}