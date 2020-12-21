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

#include "xMainEncodingWidget.h"
#include "xRipEncodeConfiguration.h"

#include <QListWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QLabel>

xMainEncodingWidget::xMainEncodingWidget(QWidget *parent, Qt::WindowFlags flags):
        QWidget(parent, flags),
        encoding(nullptr) {

    auto mainLayout = new QGridLayout(this);
    // Create Format Box
    auto formatBox = new QGroupBox(tr("Format"), this);
    auto formatLayout = new QGridLayout();
    auto formatEncodingFormatLabel = new QLabel(tr("Encoding Format"), formatBox);
    formatEncodingFormatInput = new QLineEdit(formatBox);
    formatEncodingFormatInput->setReadOnly(true);
    auto formatFileFormatLabel = new QLabel(tr("File Format"), formatBox);
    formatFileFormatInput = new QLineEdit(formatBox);
    formatFileFormatInput->setReadOnly(true);
    formatLayout->addWidget(formatEncodingFormatLabel, 0, 0, 1, 4);
    formatLayout->addWidget(formatEncodingFormatInput, 1, 0, 1, 4);
    formatLayout->addWidget(formatFileFormatLabel, 2, 0, 1, 4);
    formatLayout->addWidget(formatFileFormatInput, 3, 0, 1, 4);
    formatBox->setLayout(formatLayout);
    // Create smart update box.
    auto updateBox = new QGroupBox(tr("Smart Update"), this);
    auto updateLayout = new QGridLayout();
    updateArtistCheck = new QCheckBox(tr("Artist"), updateBox);
    updateArtistCheck->setChecked(true);
    updateAlbumCheck = new QCheckBox(tr("Album"), updateBox);
    updateAlbumCheck->setChecked(true);
    updateTagCheck = new QCheckBox(tr("Tag"), updateBox);
    updateTagCheck->setChecked(true);
    updateTrackNrCheck = new QCheckBox(tr("Track Number"), updateBox);
    updateTrackNrCheck->setChecked(true);
    updateLayout->addWidget(updateArtistCheck, 0, 0);
    updateLayout->addWidget(updateAlbumCheck, 1, 0);
    updateLayout->addWidget(updateTagCheck, 2, 0);
    updateLayout->addWidget(updateTrackNrCheck, 3, 0);
    updateBox->setLayout(updateLayout);
    // Create encode box.
    auto encodeBox = new QGroupBox(tr("Encode"), this);
    auto encodeLayout = new QGridLayout();
    encodeUseEncodingButton =  new QRadioButton(tr("Use Encoding Format"), encodeBox);
    encodeUseFileButton =  new QRadioButton(tr("Use File Format"), encodeBox);
    encodeBackupButton = new QPushButton(tr("Backup"), encodeBox);
    encodeEncodeButton = new QPushButton(tr("Encode"), encodeBox);
    encodeClearButton = new QPushButton(tr("Clear"), encodeBox);
    encodeLayout->addWidget(encodeUseEncodingButton, 0, 0, 1, 2);
    encodeLayout->addWidget(encodeUseFileButton, 1, 0, 1, 2);
    encodeLayout->setRowMinimumHeight(2, 50);
    encodeLayout->setRowStretch(2, 0);
    encodeLayout->addWidget(encodeEncodeButton, 3, 0, 1, 2);
    encodeLayout->addWidget(encodeBackupButton, 4, 0, 1, 2);
    encodeLayout->setRowMinimumHeight(5, 20);
    encodeLayout->setRowStretch(5, 0);
    encodeLayout->addWidget(encodeClearButton, 6, 0, 1, 2);
    encodeUseEncodingButton->setChecked(true);
    encodeBox->setLayout(encodeLayout);
    // Create encoded tracks box.
    auto encodingBox = new QGroupBox(tr("Encoding Tracks"), this);
    auto encodingLayout = new QGridLayout();
    encodingSelectAllButton = new QPushButton(tr("Select All"), encodingBox);
    encodingDeselectAllButton = new QPushButton(tr("Deselect All"), encodingBox);
    encodingEditAllButton = new QPushButton(tr("Edit All"), encodingBox);
    encodingOutputAllButton = new QPushButton(tr("Output All"), encodingBox);
    // Resize according to tags.
    auto tagInfos = xRipEncodeConfiguration::configuration()->getTagInfos();
    encodingAudioFiles.resize(tagInfos.count());
    encodingTracksWidgets.resize(tagInfos.count());
    encodingTracksTab = new QTabWidget(encodingBox);
    encodingTracksTab->setTabPosition(QTabWidget::West);
    for (auto i = 0; i < tagInfos.count(); ++i) {
        encodingTracksWidgets[i] = new xEncodingTracksWidget(encodingTracksTab);
        // Default setup for smart update.
        encodingTracksWidgets[i]->setUpdateArtist(true);
        encodingTracksWidgets[i]->setUpdateAlbum(true);
        encodingTracksWidgets[i]->setUpdateTag(true);
        encodingTracksWidgets[i]->setUpdateTrackNr(true);
        // Connect check boxes to each of the tracks widgets
        connect(updateArtistCheck, &QCheckBox::clicked, encodingTracksWidgets[i], &xEncodingTracksWidget::setUpdateArtist);
        connect(updateAlbumCheck, &QCheckBox::clicked, encodingTracksWidgets[i], &xEncodingTracksWidget::setUpdateAlbum);
        connect(updateTagCheck, &QCheckBox::clicked, encodingTracksWidgets[i], &xEncodingTracksWidget::setUpdateTag);
        connect(updateTrackNrCheck, &QCheckBox::clicked, encodingTracksWidgets[i], &xEncodingTracksWidget::setUpdateTrackNr);
        // Add tab.
        encodingTracksTab->addTab(encodingTracksWidgets[i], tagInfos[i]);
    }
    encodingLayout->addWidget(encodingSelectAllButton, 0, 0);
    encodingLayout->addWidget(encodingDeselectAllButton, 0, 1);
    encodingLayout->addWidget(encodingTracksTab, 1, 0, 8, 2);
    encodingLayout->addWidget(encodingOutputAllButton, 9, 0);
    encodingLayout->addWidget(encodingEditAllButton, 9, 1);
    encodingBox->setLayout(encodingLayout);
    // Setup main layout
    mainLayout->addWidget(formatBox, 0, 0, 2, 2);
    mainLayout->setRowMinimumHeight(2, 20);
    mainLayout->setRowStretch(2, 0);
    mainLayout->addWidget(updateBox, 3, 0, 2, 2);
    mainLayout->setRowMinimumHeight(5, 20);
    mainLayout->setRowStretch(5, 0);
    mainLayout->addWidget(encodeBox, 6, 0, 3, 2);
    mainLayout->setRowStretch(10, 2);
    mainLayout->addWidget(encodingBox, 0, 3, 11, 7);
    // Fill in configuration.
    formatEncodingFormatInput->setText(xRipEncodeConfiguration::configuration()->getEncodingFormat());
    formatFileFormatInput->setText(xRipEncodeConfiguration::configuration()->getFileNameFormat());
    // Connect buttons.
    connect(encodeEncodeButton, &QPushButton::pressed, this, &xMainEncodingWidget::encode);
    connect(encodeBackupButton, &QPushButton::pressed, this, &xMainEncodingWidget::backup);
    connect(encodeClearButton, &QPushButton::pressed, this, &xMainEncodingWidget::clear);
    connect(encodeUseEncodingButton, &QRadioButton::clicked, this, &xMainEncodingWidget::updateEncodedFileNames);
    connect(encodeUseFileButton, &QRadioButton::clicked, this, &xMainEncodingWidget::updateEncodedFileNames);
    connect(encodingEditAllButton, &QPushButton::pressed, this, &xMainEncodingWidget::editAll);
    connect(encodingOutputAllButton, &QPushButton::pressed, this, &xMainEncodingWidget::outputAll);
    connect(encodingSelectAllButton, &QPushButton::pressed, this, &xMainEncodingWidget::selectAll);
    connect(encodingDeselectAllButton, &QPushButton::pressed, this, &xMainEncodingWidget::deselectAll);
}

xMainEncodingWidget::~xMainEncodingWidget() noexcept {
    for (auto& tagFiles : encodingAudioFiles) {
        for (auto& file : tagFiles) {
            delete file;
        }
    }
}

void xMainEncodingWidget::setAllEnabled(bool enabled) {
    encodeUseEncodingButton->setEnabled(enabled);
    encodeUseFileButton->setEnabled(enabled);
    encodeEncodeButton->setEnabled(enabled);
    encodeBackupButton->setEnabled(enabled);
    encodeClearButton->setEnabled(enabled);
    encodingEditAllButton->setEnabled(enabled);
    encodingOutputAllButton->setEnabled(enabled);
    encodingSelectAllButton->setEnabled(enabled);
    encodingDeselectAllButton->setEnabled(enabled);
}

void xMainEncodingWidget::editAll() {
    auto currentIndex = encodingTracksTab->currentIndex();
    if ((currentIndex >= 0) && (currentIndex < encodingTracksWidgets.count())) {
        encodingTracksWidgets[currentIndex]->viewInput();
    }
}

void xMainEncodingWidget::outputAll() {
    auto currentIndex = encodingTracksTab->currentIndex();
    if ((currentIndex >= 0) && (currentIndex < encodingTracksWidgets.count())) {
        encodingTracksWidgets[currentIndex]->viewOutput();
    }
}

void xMainEncodingWidget::selectAll() {
    auto currentIndex = encodingTracksTab->currentIndex();
    if ((currentIndex >= 0) && (currentIndex < encodingTracksWidgets.count())) {
        encodingTracksWidgets[currentIndex]->selectAll();
    }
}

void xMainEncodingWidget::deselectAll() {
    auto currentIndex = encodingTracksTab->currentIndex();
    if ((currentIndex >= 0) && (currentIndex < encodingTracksWidgets.count())) {
        encodingTracksWidgets[currentIndex]->deselectAll();
    }
}

void xMainEncodingWidget::encode() {
    auto currentIndex = encodingTracksTab->currentIndex();
    if ((currentIndex >= 0) && (currentIndex < encodingTracksWidgets.count())) {
        auto encodingDirectory = xRipEncodeConfiguration::configuration()->getEncodingDirectory();
        QList<std::pair<xAudioFile*,QString>> encodingFiles;
        for (auto& selected : encodingTracksWidgets[currentIndex]->getSelected()) {
            encodingFiles.push_back(std::make_pair(selected->getAudioFile(), encodingDirectory+"/"+selected->getEncodedFileName()+".flac"));
        }
        if (!encodingFiles.isEmpty()) {
            setAllEnabled(false);
            encoding = new xAudioFileEncoding(encodingFiles, true);
            connect(encoding, &xAudioFileEncoding::encodingProgress,encodingTracksWidgets[currentIndex], &xEncodingTracksWidget::ripProgress);
            connect(encoding, &xAudioFileEncoding::finished, this, &xMainEncodingWidget::encodeFinished);
            encoding->start();
        }
    }
}

void xMainEncodingWidget::encodeFinished() {
    qDebug() << "xMainEncodingWidget::encodeFinished";
    delete encoding;
    encoding = nullptr;
    setAllEnabled(true);
}

void xMainEncodingWidget::backup() {
    auto currentIndex = encodingTracksTab->currentIndex();
    if ((currentIndex >= 0) && (currentIndex < encodingTracksWidgets.count())) {
        auto backupDirectory = xRipEncodeConfiguration::configuration()->getBackupDirectory();
        QList<std::pair<xAudioFile*,QString>> encodingFiles;
        for (auto& selected : encodingTracksWidgets[currentIndex]->getSelected()) {
            encodingFiles.push_back(std::make_pair(selected->getAudioFile(), backupDirectory+"/"+selected->getEncodedFileName()+".wv"));
        }
        if (!encodingFiles.isEmpty()) {
            setAllEnabled(false);
            encoding = new xAudioFileEncoding(encodingFiles, false);
            connect(encoding, &xAudioFileEncoding::encodingProgress,encodingTracksWidgets[currentIndex], &xEncodingTracksWidget::ripProgress);
            connect(encoding, &xAudioFileEncoding::finished, this, &xMainEncodingWidget::backupFinished);
            encoding->start();
        }
    }
}

void xMainEncodingWidget::backupFinished() {
    qDebug() << "xMainEncodingWidget::backupFinished";
    delete encoding;
    encoding = nullptr;
    setAllEnabled(true);
}


void xMainEncodingWidget::clear() {
    auto currentIndex = encodingTracksTab->currentIndex();
    if ((currentIndex >= 0) && (currentIndex < encodingTracksWidgets.count())) {
        encodingTracksWidgets[currentIndex]->clear();
        // Delete the corresponding audio file objects before clearing.
        for (auto& audioFile : encodingAudioFiles[currentIndex]) {
            delete audioFile;
        }
        encodingAudioFiles[currentIndex].clear();
        encodingTracksTab->setTabEnabled(currentIndex, false);
    }
}

void xMainEncodingWidget::updateEncodedFileNames() {
    auto encodedFormat = (encodeUseFileButton->isChecked()) ? formatFileFormatInput->text() : formatEncodingFormatInput->text();
    for (const auto& widget : encodingTracksWidgets) {
       widget->setEncodedFormat(encodedFormat);
    }
}

void xMainEncodingWidget::createEncodingTracksWidgets() {
    for (auto i = 0; i < encodingTracksWidgets.count(); ++i) {
        if (encodingAudioFiles[i].count() > 0) {
            encodingTracksTab->setTabEnabled(i, true);
            encodingTracksWidgets[i]->setTracks(encodingAudioFiles[i]);
        } else {
            encodingTracksTab->setTabEnabled(i, false);
        }
    }
}

void xMainEncodingWidget::audioFiles(const QList<xAudioFile*>& files) {
    // Split among the Tag IDs.
    for (const auto& file : files) {
        encodingAudioFiles[file->getTagId()].push_back(file);
    }
    createEncodingTracksWidgets();
    updateEncodedFileNames();
}