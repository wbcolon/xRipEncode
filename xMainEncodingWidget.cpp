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
#include <QHBoxLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>

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
    // Create encode box.
    auto encodeBox = new QGroupBox(tr("Encode"), this);
    auto encodeLayout = new QGridLayout();
    encodeUseEncodingButton =  new QRadioButton(tr("Use Encoding Format"), encodeBox);
    encodeUseFileButton =  new QRadioButton(tr("Use File Format"), encodeBox);
    encodeSelectAllButton = new QPushButton(tr("Select All"), encodeBox);
    encodeDeselectAllButton = new QPushButton(tr("Deselect All"), encodeBox);
    encodeEditAllButton = new QPushButton(tr("Edit All"), encodeBox);
    encodeFinishAllButton = new QPushButton(tr("Finish All"), encodeBox);
    encodeBackupButton = new QPushButton(tr("Backup"), encodeBox);
    encodeEncodeButton = new QPushButton(tr("Encode"), encodeBox);
    encodeClearButton = new QPushButton(tr("Clear"), encodeBox);
    encodeLayout->addWidget(encodeUseEncodingButton, 0, 0, 1, 2);
    encodeLayout->addWidget(encodeUseFileButton, 1, 0, 1, 2);
    encodeLayout->setRowMinimumHeight(2, 50);
    encodeLayout->setRowStretch(2, 0);
    encodeLayout->addWidget(encodeEditAllButton, 3, 0);
    encodeLayout->addWidget(encodeFinishAllButton, 3, 1);
    encodeLayout->setRowMinimumHeight(4, 20);
    encodeLayout->setRowStretch(4, 0);
    encodeLayout->addWidget(encodeSelectAllButton, 5, 0);
    encodeLayout->addWidget(encodeDeselectAllButton, 5, 1);
    encodeLayout->setRowMinimumHeight(6, 50);
    encodeLayout->setRowStretch(6, 0);
    encodeLayout->addWidget(encodeEncodeButton, 7, 0, 1, 2);
    encodeLayout->addWidget(encodeBackupButton, 8, 0, 1, 2);
    encodeLayout->setRowMinimumHeight(9, 20);
    encodeLayout->setRowStretch(9, 0);
    encodeLayout->addWidget(encodeClearButton, 10, 0, 1, 2);
    encodeUseEncodingButton->setChecked(true);
    encodeBox->setLayout(encodeLayout);
    // Resize according to tags.
    auto tagInfos = xRipEncodeConfiguration::configuration()->getTagInfos();
    encodingAudioFiles.resize(tagInfos.count());
    encodingTracksWidgets.resize(tagInfos.count());
    encodingTracksTab = new QTabWidget(this);
    for (auto i = 0; i < tagInfos.count(); ++i) {
        encodingTracksWidgets[i] = new xEncodingTracksWidget(encodingTracksTab);
        encodingTracksTab->addTab(encodingTracksWidgets[i], tagInfos[i]);
    }
    // Setup main layout
    mainLayout->addWidget(formatBox, 0, 0, 2, 2);
    mainLayout->setRowMinimumHeight(2, 20);
    mainLayout->setRowStretch(2, 0);
    mainLayout->addWidget(encodeBox, 3, 0, 6, 2);
    mainLayout->setRowStretch(10, 2);
    mainLayout->setColumnMinimumWidth(3, 20);
    mainLayout->setColumnStretch(3, 0);
    mainLayout->addWidget(encodingTracksTab, 0, 4, 11, 6);
    // Fill in configuration.
    formatEncodingFormatInput->setText(xRipEncodeConfiguration::configuration()->getEncodingFormat());
    formatFileFormatInput->setText(xRipEncodeConfiguration::configuration()->getFileNameFormat());
    // Connect buttons.
    connect(encodeEditAllButton, &QPushButton::pressed, this, &xMainEncodingWidget::editAll);
    connect(encodeFinishAllButton, &QPushButton::pressed, this, &xMainEncodingWidget::finishAll);
    connect(encodeSelectAllButton, &QPushButton::pressed, this, &xMainEncodingWidget::selectAll);
    connect(encodeDeselectAllButton, &QPushButton::pressed, this, &xMainEncodingWidget::deselectAll);
    connect(encodeEncodeButton, &QPushButton::pressed, this, &xMainEncodingWidget::encode);
    connect(encodeBackupButton, &QPushButton::pressed, this, &xMainEncodingWidget::backup);
    connect(encodeClearButton, &QPushButton::pressed, this, &xMainEncodingWidget::clear);
    connect(encodeUseEncodingButton, &QRadioButton::clicked, this, &xMainEncodingWidget::updateEncodedFileNames);
    connect(encodeUseFileButton, &QRadioButton::clicked, this, &xMainEncodingWidget::updateEncodedFileNames);
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
    encodeEditAllButton->setEnabled(enabled);
    encodeFinishAllButton->setEnabled(enabled);
    encodeSelectAllButton->setEnabled(enabled);
    encodeDeselectAllButton->setEnabled(enabled);
    encodeEncodeButton->setEnabled(enabled);
    encodeBackupButton->setEnabled(enabled);
    encodeClearButton->setEnabled(enabled);
}

void xMainEncodingWidget::editAll() {
    auto currentIndex = encodingTracksTab->currentIndex();
    if ((currentIndex >= 0) && (currentIndex < encodingTracksWidgets.count())) {
        encodingTracksWidgets[currentIndex]->viewInput();
    }
}

void xMainEncodingWidget::finishAll() {
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
        QList<std::pair<xAudioFile*,QString>> encodingFiles;
        for (auto& selected : encodingTracksWidgets[currentIndex]->getSelected()) {
            encodingFiles.push_back(std::make_pair(selected->getAudioFile(), selected->getEncodedFileName()+".flac"));
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
        QList<std::pair<xAudioFile*,QString>> encodingFiles;
        for (auto& selected : encodingTracksWidgets[currentIndex]->getSelected()) {
            encodingFiles.push_back(std::make_pair(selected->getAudioFile(), selected->getEncodedFileName()+".wv"));
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