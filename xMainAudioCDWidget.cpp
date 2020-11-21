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
#include <QFileDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QDebug>

xMainAudioCDWidget::xMainAudioCDWidget(QWidget *parent, Qt::WindowFlags flags):
        QWidget(parent, flags) {

    auto mainLayout = new QGridLayout(this);
    // Audio cd box for artist and album input and some control.
    auto audioCDBox = new QGroupBox(tr("Audio CD"), this);
    audioCDDetectButton = new QPushButton(tr("Detect"), audioCDBox);
    audioCDEjectButton = new QPushButton(tr("Eject"), audioCDBox);
    audioCDArtistName = new QLineEdit(audioCDBox);
    audioCDAlbumName = new QLineEdit(audioCDBox);
    auto audioCDLayout = new QGridLayout();
    audioCDAutoFillButton = new QPushButton(tr("Autofill"), audioCDBox);
    audioCDCDDBButton = new QPushButton(tr("CDDB"), audioCDBox);
    audioCDLayout->addWidget(new QLabel(tr("Artist"), audioCDBox), 0, 0, 1, 1);
    audioCDLayout->addWidget(audioCDArtistName, 1, 0, 1, 6);
    audioCDLayout->addWidget(new QLabel(tr("Album"), audioCDBox), 2, 0, 1, 1);
    audioCDLayout->addWidget(audioCDAlbumName, 3, 0, 1, 6);
    audioCDLayout->setRowMinimumHeight(4, 50);
    audioCDLayout->setRowStretch(4, 0);
    audioCDLayout->addWidget(audioCDDetectButton, 5, 0, 1, 3);
    audioCDLayout->addWidget(audioCDEjectButton, 5, 3, 1, 3);
    audioCDLayout->setRowMinimumHeight(6, 20);
    audioCDLayout->setRowStretch(6, 0);
    audioCDLayout->addWidget(audioCDCDDBButton, 7, 0, 1, 3);
    audioCDLayout->addWidget(audioCDAutoFillButton, 7, 3, 1, 3);
    audioCDBox->setLayout(audioCDLayout);
    // Configuration box for ouput file format and output directory.
    auto configurationBox = new QGroupBox(tr("Configuration"), this);
    configurationFileFormat = new QLineEdit(configurationBox);
    configurationFileFormat->setText("(artist)#(album)#(tracknr) (trackname)");
    auto configurationFileFormatButton = new QPushButton("...", configurationBox);
    configurationDirectory = new QLineEdit(configurationBox);
    auto configurationDiretoryButton = new QPushButton("...", configurationBox);
    auto configurationLayout = new QGridLayout();
    configurationLayout->addWidget(new QLabel(tr("File Format"), configurationBox), 0, 0);
    configurationLayout->addWidget(configurationFileFormat, 1, 0, 1, 5);
    configurationLayout->addWidget(configurationFileFormatButton, 1, 5);
    configurationLayout->addWidget(new QLabel(tr("Directory"), configurationBox), 2, 0);
    configurationLayout->addWidget(configurationDirectory, 3, 0, 1, 5);
    configurationLayout->addWidget(configurationDiretoryButton, 3, 5);
    configurationBox->setLayout(configurationLayout);
    // Audio tracks box.
    auto audioTracksBox = new QGroupBox(tr("Audio Tracks"), this);
    audioTracksSelectButton = new QPushButton(tr("Select All"), audioTracksBox);
    audioTracksRipButton = new QPushButton(tr("Rip Selected"), audioTracksBox);
    audioTracksRipCancelButton = new QPushButton(tr("Cancel Rip"), audioTracksBox);
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
    mainLayout->addWidget(configurationBox, 0, 0, 3, 4);
    mainLayout->addWidget(audioCDBox, 3, 0, 4, 4);
    mainLayout->addWidget(audioTracksBox, 0, 4, 7, 4);
    mainLayout->setRowMinimumHeight(7, 20);
    mainLayout->setRowStretch(7, 0);
    mainLayout->addWidget(consoleBox, 8, 0, 2, 8);
    // Connect Buttons
    connect(configurationDiretoryButton, &QPushButton::pressed, this, &xMainAudioCDWidget::directory);
    connect(configurationFileFormatButton, &QPushButton::pressed, this, &xMainAudioCDWidget::fileformat);
    connect(audioCDDetectButton, &QPushButton::pressed, this, &xMainAudioCDWidget::detect);
    connect(audioCDEjectButton, &QPushButton::pressed, this, &xMainAudioCDWidget::eject);
    connect(audioCDCDDBButton, &QPushButton::pressed, this, &xMainAudioCDWidget::cddb);
    connect(audioCDAutoFillButton, &QPushButton::pressed, this, &xMainAudioCDWidget::autofill);
    connect(audioTracksSelectButton, &QPushButton::pressed, audioTracks, &xAudioTracksWidget::selectAll);
    connect(audioTracksRipButton, &QPushButton::pressed, this, &xMainAudioCDWidget::rip);
    connect(audioTracksRipCancelButton, &QPushButton::pressed, this, &xMainAudioCDWidget::ripCancel);
    // Create audio ripper object and connect object.
    audioCD = new xAudioCD(this);
    connect(audioCD, &xAudioCD::ripProgress, audioTracks, &xAudioTracksWidget::ripProgress);
    connect(audioCD, &xAudioCD::ripMessages, this, &xMainAudioCDWidget::ripMessage);
    connect(audioCD, &xAudioCD::ripError, this, &xMainAudioCDWidget::ripError);
    connect(audioCD, &xAudioCD::ripFinished, this, &xMainAudioCDWidget::ripFinished);
}

void xMainAudioCDWidget::cddb() {
    auto id = audioCD->getID();
    if (!id.isEmpty()) {
        // Initiate audio CD lookup. Request lowercase results.
        audioCDLookup = new xAudioCDLookup(id, true, this);
        connect(audioCDLookup, &xAudioCDLookup::finished, this, &xMainAudioCDWidget::cddbFinished);
        audioCDLookup->start();
    }
}

void xMainAudioCDWidget::cddbFinished() {
    auto result = audioCDLookup->result();
    audioCDArtistName->setText(result.at(0).artist);
    audioCDAlbumName->setText(result.at(0).album);
    audioTracks->setTrackNames(result.at(0).tracks);
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
    }
}

void xMainAudioCDWidget::eject() {
    // Clear list and eject device.
    audioTracks->clear();
    audioCDArtistName->clear();
    audioCDAlbumName->clear();
    audioCD->eject();
}

void xMainAudioCDWidget::rip() {
    // Scan.
    auto trackNames = getTrackNames();
    qDebug() << "xMainAudioCDWidget: rip: " << trackNames;
    audioCDAlbumName->setEnabled(false);
    audioCDArtistName->setEnabled(false);
    audioCDDetectButton->setEnabled(false);
    audioCDEjectButton->setEnabled(true);
    audioCDCDDBButton->setEnabled(false);
    audioCDAutoFillButton->setEnabled(false);
    audioTracksSelectButton->setEnabled(false);
    audioTracksRipButton->setEnabled(false);
    audioTracksRipCancelButton->setEnabled(true);
    audioCD->rip(trackNames);
}

void xMainAudioCDWidget::ripCancel() {
    audioCD->ripCancel();
    ripFinished();
}

void xMainAudioCDWidget::ripMessage(int track, const QString& message) {
    if (track > 0) {
        consoleText->append(QString("[Track %1] %2").arg(track).arg(message));
    } else {
        consoleText->append(QString("[All] %1").arg(message));
    }
}

void xMainAudioCDWidget::ripError(int track, const QString& error, bool abort) {
}

void xMainAudioCDWidget::ripFinished() {
    audioCDAlbumName->setEnabled(true);
    audioCDArtistName->setEnabled(true);
    audioCDDetectButton->setEnabled(true);
    audioCDEjectButton->setEnabled(true);
    audioCDCDDBButton->setEnabled(true);
    audioCDAutoFillButton->setEnabled(true);
    audioTracksSelectButton->setEnabled(true);
    audioTracksRipButton->setEnabled(true);
    audioTracksRipCancelButton->setEnabled(false);
}

void xMainAudioCDWidget::directory() {
    QString tempFileDirectory =
            QFileDialog::getExistingDirectory(this, tr("Temp File Directory"), configurationDirectory->text(),
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (!tempFileDirectory.isEmpty()) {
        configurationDirectory->setText(tempFileDirectory);
    }
}

void xMainAudioCDWidget::fileformat() {
    QString fileFormatText = "<p><b>(artist)</b> : artist name for the audio CD</p>"
                             "<p><b>(album)</b> : album name for the audio CD</p>"
                             "<p><b>(tracknr)</b> : number for each audio track </p>"
                             "<p><b>(trackname)</b> : name for each audio track </p><p></p>";
    QMessageBox::information(this, tr("File Format"), fileFormatText);
}

QList<std::pair<int,QString>> xMainAudioCDWidget::getTrackNames() {
    QList<std::pair<int,QString>> trackNames;
    auto selectedTracks = audioTracks->isSelected();
    auto artistName = audioCDArtistName->text();
    auto albumName = audioCDAlbumName->text();
    auto fileFormat = configurationFileFormat->text();
    fileFormat.replace("(artist)", artistName);
    fileFormat.replace("(album)", albumName);
    for (const auto& track : selectedTracks) {
        auto trackFileFormat = fileFormat;
        trackFileFormat.replace("(tracknr)", std::get<1>(track));
        trackFileFormat.replace("(trackname)", std::get<2>(track));
        trackFileFormat.append(".wav");
        trackNames.push_back(std::make_pair(std::get<0>(track), trackFileFormat));
    }
    return trackNames;
}
