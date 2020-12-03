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

#include "xMainAudioCDWidget.h"
#include "xRipEncodeConfiguration.h"
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QDebug>

xMainAudioCDWidget::xMainAudioCDWidget(QWidget *parent, Qt::WindowFlags flags):
        QWidget(parent, flags) {

    auto mainLayout = new QGridLayout(this);
    // Audio CD -  artist and album input and some control.
    auto audioCDBox = new QGroupBox(tr("Audio CD"), this);
    audioCDDetectButton = new QPushButton(tr("Detect"), audioCDBox);
    audioCDEjectButton = new QPushButton(tr("Eject"), audioCDBox);
    audioCDArtistName = new QLineEdit(audioCDBox);
    audioCDAlbumName = new QLineEdit(audioCDBox);
    auto audioCDLayout = new QGridLayout();
    audioCDLayout->addWidget(new QLabel(tr("Artist"), audioCDBox), 0, 0, 1, 1);
    audioCDLayout->addWidget(audioCDArtistName, 1, 0, 1, 6);
    audioCDLayout->addWidget(new QLabel(tr("Album"), audioCDBox), 2, 0, 1, 1);
    audioCDLayout->addWidget(audioCDAlbumName, 3, 0, 1, 6);
    audioCDLayout->setRowMinimumHeight(4, 50);
    audioCDLayout->setRowStretch(4, 0);
    audioCDLayout->addWidget(audioCDDetectButton, 5, 0, 1, 3);
    audioCDLayout->addWidget(audioCDEjectButton, 5, 3, 1, 3);
    audioCDLayout->setRowMinimumHeight(6, 50);
    audioCDLayout->setRowStretch(6, 0);
    // Audio CD - lookup section.
    auto audioCDLookupResultsLabel = new QLabel(tr("Lookup Results"), audioCDBox);
    audioCDLookupResults = new QComboBox(audioCDBox);
    audioCDAutofillButton = new QPushButton(tr("Autofill"), audioCDBox);
    audioCDLookupButton = new QPushButton(tr("Lookup"), audioCDBox);
    audioCDTrackOffset = new QSpinBox(audioCDBox);
    auto audioCDTrackOffsetLabel = new QLabel(tr("Offset"), audioCDBox);
    audioCDTrackOffsetLabel->setAlignment(Qt::AlignLeft);
    audioCDLowerCase = new QCheckBox(tr("Lowercase"), audioCDBox);
    audioCDLowerCase->setChecked(true);
    audioCDReplace = new QCheckBox(tr("Replace"), audioCDBox);
    audioCDReplace->setChecked(false);
    audioCDReplaceView = new QListWidget(audioCDBox);
    // Fill replace view.
    auto replace = xRipEncodeConfiguration::configuration()->getFileNameReplace();
    for (const auto& entry : replace) {
        audioCDReplaceView->addItem(QString(R"(replace "%1" with "%2")").arg(entry.first).arg(entry.second));
    }
    audioCDLayout->addWidget(audioCDLookupResultsLabel, 7, 0, 1, 5);
    audioCDLayout->addWidget(audioCDTrackOffsetLabel, 7, 5, 1, 1);
    audioCDLayout->addWidget(audioCDLookupResults, 8, 0, 1, 5);
    audioCDLayout->addWidget(audioCDTrackOffset, 8, 5, 1, 1);
    audioCDLayout->setRowMinimumHeight(9, 20);
    audioCDLayout->setRowStretch(9, 0);
    audioCDLayout->addWidget(audioCDLowerCase, 10, 0, 1, 1);
    audioCDLayout->addWidget(audioCDReplace, 10, 1, 1, 1);
    audioCDLayout->addWidget(audioCDReplaceView, 10, 2, 2, 4);
    audioCDLayout->setRowMinimumHeight(12, 50);
    audioCDLayout->setRowStretch(12, 0);
    audioCDLayout->addWidget(audioCDLookupButton, 13, 0, 1, 3);
    audioCDLayout->addWidget(audioCDAutofillButton, 13, 3, 1, 3);
    audioCDLayout->setRowMinimumHeight(14, 20);
    audioCDLayout->setRowStretch(14, 0);
    audioCDBox->setLayout(audioCDLayout);
    // Audio tracks box.
    auto audioTracksBox = new QGroupBox(tr("Audio Tracks"), this);
    audioTracksSelectButton = new QPushButton(tr("Select All"), audioTracksBox);
    audioTracksRipButton = new QPushButton(tr("Rip Selected"), audioTracksBox);
    audioTracksRipButton->setEnabled(false);
    audioTracksRipCancelButton = new QPushButton(tr("Cancel Rip"), audioTracksBox);
    audioTracksRipCancelButton->setEnabled(false);
    audioTracks = new xAudioTracksWidget(audioTracksBox);
    auto audioTracksLayout = new QGridLayout();
    audioTracksLayout->addWidget(audioTracksSelectButton, 0, 0, 1, 2);
    audioTracksLayout->setRowMinimumHeight(1, 20);
    audioTracksLayout->setRowStretch(1, 0);
    audioTracksLayout->addWidget(audioTracks, 2, 0, 6, 2);
    audioTracksLayout->setRowMinimumHeight(8, 20);
    audioTracksLayout->setRowStretch(8, 0);
    audioTracksLayout->addWidget(audioTracksRipButton, 9, 0, 1, 1);
    audioTracksLayout->addWidget(audioTracksRipCancelButton, 9, 1, 1, 1);
    audioTracksBox->setLayout(audioTracksLayout);
    // Console box.
    auto consoleBox = new QGroupBox(tr("Console"), this);
    consoleText = new QTextEdit(consoleBox);
    consoleText->setAcceptRichText(true);
    consoleText->setReadOnly(true);
    auto consoleLayout = new QGridLayout();
    consoleLayout->addWidget(consoleText, 0, 0, 2, 6);
    consoleBox->setLayout(consoleLayout);
    // Setup main layout.
    mainLayout->addWidget(audioCDBox, 0, 0, 7, 4);
    mainLayout->addWidget(consoleBox, 7, 0, 3, 4);
    mainLayout->addWidget(audioTracksBox, 0, 4, 10, 4);
    // Connect Buttons.
    connect(audioCDDetectButton, &QPushButton::pressed, this, &xMainAudioCDWidget::detect);
    connect(audioCDEjectButton, &QPushButton::pressed, this, &xMainAudioCDWidget::eject);
    connect(audioCDAutofillButton, &QPushButton::pressed, this, &xMainAudioCDWidget::autofill);
    connect(audioCDLookupButton, &QPushButton::pressed, this, &xMainAudioCDWidget::musicBrainz);
    connect(audioTracksSelectButton, &QPushButton::pressed, audioTracks, &xAudioTracksWidget::selectAll);
    connect(audioTracksRipButton, &QPushButton::pressed, this, &xMainAudioCDWidget::rip);
    connect(audioTracksRipCancelButton, &QPushButton::pressed, this, &xMainAudioCDWidget::ripCancel);
    connect(audioCDLookupResults, SIGNAL(currentIndexChanged(int)), this, SLOT(musicBrainzUpdate(int)));
    // Connect LineEdits for Artist and Album.
    connect(audioCDArtistName, &QLineEdit::textChanged, this, &xMainAudioCDWidget::artistOrAlbumChanged);
    connect(audioCDAlbumName, &QLineEdit::textChanged, this, &xMainAudioCDWidget::artistOrAlbumChanged);
    // Update names if checkbox "Replace" or "Lowercase" is clicked.
    connect(audioCDLowerCase, &QCheckBox::clicked, [=](bool) { musicBrainzUpdate(audioCDLookupResults->currentIndex()); });
    connect(audioCDReplace, &QCheckBox::clicked, [=](bool) { musicBrainzUpdate(audioCDLookupResults->currentIndex()); });
    // Update track offset.
    connect(audioCDTrackOffset, SIGNAL(valueChanged(int)), audioTracks, SLOT(setTrackOffset(int)));
    // Create audio ripper object and connect object.
    audioCD = new xAudioCD(this);
    connect(audioCD, &xAudioCD::ripProgress, audioTracks, &xAudioTracksWidget::ripProgress);
    connect(audioCD, &xAudioCD::ripMessages, this, &xMainAudioCDWidget::ripMessage);
    connect(audioCD, &xAudioCD::ripError, this, &xMainAudioCDWidget::ripError);
    connect(audioCD, &xAudioCD::ripFinished, this, &xMainAudioCDWidget::ripFinished);
    connect(audioCD, &xAudioCD::audioFiles, this, &xMainAudioCDWidget::audioFiles);
}

void xMainAudioCDWidget::musicBrainz() {
    auto id = audioCD->getID();
    if (!id.isEmpty()) {
        // Initiate audio CD lookup. Request lowercase results.
        audioCDLookup = new xAudioCDLookup(id, this);
        connect(audioCDLookup, &xAudioCDLookup::finished, this, &xMainAudioCDWidget::musicBrainzFinished);
        audioCDLookup->start();
    }
}

void xMainAudioCDWidget::musicBrainzFinished() {
    lookupResults = audioCDLookup->result();
    audioCDLookupResults->clear();
    for (const auto& result : lookupResults) {
        audioCDLookupResults->addItem(QString("%1 - %2").arg(result.artist).arg(result.album));
    }
    audioCDLookupResults->setCurrentIndex(0);
    musicBrainzUpdate(0);
}

void xMainAudioCDWidget::musicBrainzUpdate(int index) {
    if ((index >= 0) && (index < lookupResults.count())) {
        auto result = lookupResults.at(index);
        result.artist = updateString(result.artist);
        result.album = updateString(result.album);
        for (auto& track : result.tracks) {
            track = updateString(track);
        }
        audioCDArtistName->setText(result.artist);
        audioCDAlbumName->setText(result.album);
        audioTracks->setTrackNames(result.tracks);
    }
}

void xMainAudioCDWidget::artistOrAlbumChanged(const QString& text) {
    Q_UNUSED(text)
    if (audioCDArtistName->text().isEmpty() || audioCDAlbumName->text().isEmpty()) {
        audioTracksRipButton->setEnabled(false);
    } else {
        audioTracksRipButton->setEnabled(true);
    }
}

void xMainAudioCDWidget::autofill() {
    audioCDArtistName->setText(tr("artist"));
    audioCDAlbumName->setText(tr("album"));
    audioTracks->autofill();
}

void xMainAudioCDWidget::detect() {
    // Check for audio CD.
    if (audioCD->detect()) {
        // Fill in an item for each track on the audio CD.
        audioTracks->setTracks(audioCD->getTracks());
        // Fill in the times.
        audioTracks->setTrackLengths(audioCD->getTrackLengths());
    }
}

void xMainAudioCDWidget::eject() {
    // Clear list and eject device.
    audioTracks->clear();
    audioCDArtistName->clear();
    audioCDAlbumName->clear();
    audioCDLookupResults->clear();
    audioCD->eject();
}

void xMainAudioCDWidget::rip() {
    // Retrieve selected tracks.
    auto tracks = getTracks();
    if (tracks.isEmpty()) {
        consoleText->append("No tracks selected.");
        return;
    }
    audioCDAlbumName->setEnabled(false);
    audioCDArtistName->setEnabled(false);
    audioCDDetectButton->setEnabled(false);
    audioCDEjectButton->setEnabled(false);
    audioCDAutofillButton->setEnabled(false);
    audioCDLookupButton->setEnabled(false);
    audioCDLookupResults->setEnabled(false);
    audioCDTrackOffset->setEnabled(false);
    audioCDLowerCase->setEnabled(false);
    audioCDReplace->setEnabled(false);
    audioCDReplaceView->setEnabled(false);
    audioTracksSelectButton->setEnabled(false);
    audioTracksRipButton->setEnabled(false);
    audioTracksRipCancelButton->setEnabled(true);
    audioCD->rip(tracks);
}

void xMainAudioCDWidget::ripCancel() {
    audioCD->ripCancel();
    ripFinished();
}

void xMainAudioCDWidget::ripMessage(int track, const QString& message) {
    if (track > 0) {
        consoleText->append(QString("(track %1) %2").arg(track).arg(message));
    } else {
        consoleText->append(message);
    }
}

void xMainAudioCDWidget::ripError(int track, const QString& error, bool abort) {
    QString errorMessage;
    if (track > 0) {
        errorMessage = QString("(track %1) %2").arg(track).arg(error);
    } else {
        errorMessage = error;
    }
    consoleText->append(QString("[%1] %2").arg((abort)?"abort":"error").arg(errorMessage));
}

void xMainAudioCDWidget::ripFinished() {
    audioCDAlbumName->setEnabled(true);
    audioCDArtistName->setEnabled(true);
    audioCDDetectButton->setEnabled(true);
    audioCDEjectButton->setEnabled(true);
    audioCDAutofillButton->setEnabled(true);
    audioCDLookupButton->setEnabled(true);
    audioCDLookupResults->setEnabled(true);
    audioCDTrackOffset->setEnabled(true);
    audioCDLowerCase->setEnabled(true);
    audioCDReplace->setEnabled(true);
    audioCDReplaceView->setEnabled(true);
    audioTracksSelectButton->setEnabled(true);
    audioTracksRipButton->setEnabled(true);
    audioTracksRipCancelButton->setEnabled(false);
}

QList<xAudioFile*> xMainAudioCDWidget::getTracks() {
    QList<xAudioFile*> tracks;
    auto selectedTracks = audioTracks->isSelected();
    auto artistName = audioCDArtistName->text();
    auto albumName = audioCDAlbumName->text();
    auto tagName = xRipEncodeConfiguration::configuration()->getTags().at(0);
    auto fileFormat = xRipEncodeConfiguration::configuration()->getFileNameFormat();
    auto tempDirectory = xRipEncodeConfiguration::configuration()->getTempDirectory();
    fileFormat.replace("(artist)", artistName);
    fileFormat.replace("(album)", albumName);
    fileFormat.replace("(tag)", tagName);
    for (const auto& track : selectedTracks) {
        auto trackFileFormat = fileFormat;
        trackFileFormat.replace("(tracknr)", std::get<1>(track));
        trackFileFormat.replace("(trackname)", std::get<2>(track));
        trackFileFormat.append(".wav");
        tracks.push_back(new xAudioFile(trackFileFormat, std::get<0>(track), artistName, albumName,
                                        std::get<1>(track), std::get<2>(track), tagName, 0));
    }
    return tracks;
}

QString xMainAudioCDWidget::updateString(const QString& text) {
    QString updatedText;
    if (audioCDLowerCase->isChecked()) {
        updatedText = text.toLower();
    } else {
        updatedText = text;
    }
    auto replace = xRipEncodeConfiguration::configuration()->getFileNameReplace();
    // Replace all characters if from and to strings are the same length
    for (const auto& entry : replace) {
        updatedText.replace(entry.first, entry.second);
    }
    return updatedText;
}