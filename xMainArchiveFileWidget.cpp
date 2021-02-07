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

#include "xMainArchiveFileWidget.h"
#include "xRipEncodeConfiguration.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QDir>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QRandomGenerator>
#include <QDebug>

xMainArchiveFileWidget::xMainArchiveFileWidget(QWidget *parent, Qt::WindowFlags flags):
        QWidget(parent, flags) {

    auto mainLayout = new QGridLayout(this);
    // Audio CD -  artist and album input and some control.
    auto archiveFileBox = new QGroupBox(tr("Archive File"), this);
    archiveFileBox->setFlat(xRipEncodeUseFlatGroupBox);
    archiveFileArtistName = new QLineEdit(archiveFileBox);
    archiveFileAlbumName = new QLineEdit(archiveFileBox);
    archiveFileName = new QLineEdit(archiveFileBox);
    archiveFileDialogButton = new QPushButton(tr("..."), archiveFileBox);
    archiveFileAutofillButton = new QPushButton(tr("Autofill"), archiveFileBox);
    archiveFileAnalyzeButton = new QPushButton(tr("Analyze"), archiveFileBox);
    archiveFileTagLookupSelectionList = new QListWidget(archiveFileBox);
    archiveFileTagLookupSelectionList->addItems(xArchiveFile::TagLookupSchemes);
    archiveFileTagLookupButton = new QPushButton(tr("Lookup"), archiveFileBox);
    archiveFileTagHDInputCheck = new QCheckBox(tr("HD input"), archiveFileBox);
    auto archiveFileLayout = new QGridLayout();
    archiveFileLayout->addWidget(new QLabel(tr("Artist"), archiveFileBox), 0, 0, 1, 1);
    archiveFileLayout->addWidget(archiveFileArtistName, 1, 0, 1, 6);
    archiveFileLayout->addWidget(new QLabel(tr("Album"), archiveFileBox), 2, 0, 1, 1);
    archiveFileLayout->addWidget(archiveFileAlbumName, 3, 0, 1, 6);
    archiveFileLayout->setRowMinimumHeight(4, 50);
    archiveFileLayout->setRowStretch(4, 0);
    archiveFileLayout->addWidget(new QLabel(tr("Filename"), archiveFileBox), 5, 0, 1, 6);
    archiveFileLayout->addWidget(archiveFileName, 6, 0, 1, 5);
    archiveFileLayout->addWidget(archiveFileDialogButton, 6, 5, 1, 1);
    archiveFileLayout->setRowMinimumHeight(7, 50);
    archiveFileLayout->setRowStretch(7, 0);
    archiveFileLayout->addWidget(archiveFileAnalyzeButton, 8, 0, 1, 3);
    archiveFileLayout->addWidget(archiveFileAutofillButton, 8, 3, 1, 3);
    archiveFileLayout->setRowMinimumHeight(9, 20);
    archiveFileLayout->setRowStretch(9, 0);
    archiveFileLayout->addWidget(new QLabel(tr("Tag Scheme"), archiveFileBox), 10, 0, 1, 1);
    archiveFileLayout->addWidget(archiveFileTagLookupSelectionList, 11, 0, 3, 6);
    archiveFileLayout->addWidget(archiveFileTagLookupButton, 14, 0, 1, 3);
    archiveFileLayout->addWidget(archiveFileTagHDInputCheck, 14, 5, 1, 1);
    archiveFileLayout->setRowMinimumHeight(15, 50);
    archiveFileLayout->setRowStretch(15, 0);
    archiveFileBox->setLayout(archiveFileLayout);
    // Audio tracks box.
    auto archiveAudioTracksBox = new QGroupBox(tr("Audio Tracks"), this);
    archiveAudioTracksBox->setFlat(xRipEncodeUseFlatGroupBox);
    archiveAudioTracksSelectButton = new QPushButton(tr("Select All"), archiveAudioTracksBox);
    archiveAudioTracksExtractButton = new QPushButton(tr("Extract Selected"), archiveAudioTracksBox);
    archiveAudioTracksExtractButton->setEnabled(false);
    archiveAudioTracksExtractCancelButton = new QPushButton(tr("Cancel Extract"), archiveAudioTracksBox);
    archiveAudioTracksExtractCancelButton->setEnabled(false);
    archiveAudioTracks = new xAudioTracksWidget(archiveAudioTracksBox);
    auto archiveAudioTracksLayout = new QGridLayout();
    archiveAudioTracksLayout->addWidget(archiveAudioTracksSelectButton, 0, 0, 1, 2);
    archiveAudioTracksLayout->setRowMinimumHeight(1, 20);
    archiveAudioTracksLayout->setRowStretch(1, 0);
    archiveAudioTracksLayout->addWidget(archiveAudioTracks, 2, 0, 6, 2);
    archiveAudioTracksLayout->setRowMinimumHeight(8, 20);
    archiveAudioTracksLayout->setRowStretch(8, 0);
    archiveAudioTracksLayout->addWidget(archiveAudioTracksExtractButton, 9, 0, 1, 1);
    archiveAudioTracksLayout->addWidget(archiveAudioTracksExtractCancelButton, 9, 1, 1, 1);
    archiveAudioTracksBox->setLayout(archiveAudioTracksLayout);
    // Console box.
    auto consoleBox = new QGroupBox(tr("Console"), this);
    consoleBox->setFlat(xRipEncodeUseFlatGroupBox);
    consoleText = new QTextEdit(consoleBox);
    consoleText->setAcceptRichText(true);
    consoleText->setReadOnly(true);
    auto consoleLayout = new QGridLayout();
    consoleLayout->addWidget(consoleText, 0, 0, 2, 6);
    consoleBox->setLayout(consoleLayout);
    // Setup main layout.
    mainLayout->addWidget(archiveFileBox, 0, 0, 7, 4);
    mainLayout->addWidget(consoleBox, 7, 0, 3, 4);
    mainLayout->addWidget(archiveAudioTracksBox, 0, 4, 10, 4);
    // Create audio ripper object and connect object.
    archiveFile = new xArchiveFile(this);
    connect(archiveFile, &xArchiveFile::archivedFiles, this, &xMainArchiveFileWidget::archivedFiles);
    connect(archiveFile, &xArchiveFile::messages, this, &xMainArchiveFileWidget::messages);
    connect(archiveFile, &xArchiveFile::extractProgress, archiveAudioTracks, &xAudioTracksWidget::ripProgress);
    connect(archiveFile, &xArchiveFile::finished, this, &xMainArchiveFileWidget::extractFinished);
    // Connect signals for audio files.
    connect(archiveFile, &xArchiveFile::audioFiles, this, &xMainArchiveFileWidget::audioFiles);
    // Connect buttons.
    connect(archiveFileDialogButton, &QPushButton::pressed, this, &xMainArchiveFileWidget::openFile);
    connect(archiveFileAnalyzeButton, &QPushButton::pressed, this, &xMainArchiveFileWidget::analyze);
    connect(archiveFileAutofillButton, &QPushButton::pressed, this, &xMainArchiveFileWidget::autofill);
    connect(archiveFileTagLookupButton, &QPushButton::pressed, this, &xMainArchiveFileWidget::lookup);
    connect(archiveAudioTracksSelectButton, &QPushButton::pressed, archiveAudioTracks, &xAudioTracksWidget::selectAll);
    connect(archiveAudioTracksExtractButton, &QPushButton::pressed, this, &xMainArchiveFileWidget::extract);
    connect(archiveAudioTracksExtractCancelButton, &QPushButton::pressed, this, &xMainArchiveFileWidget::extractCancel);
    // Connect album and artist LineEdit.
    connect(archiveFileArtistName, &QLineEdit::textChanged, this, &xMainArchiveFileWidget::artistOrAlbumChanged);
    connect(archiveFileAlbumName, &QLineEdit::textChanged, this, &xMainArchiveFileWidget::artistOrAlbumChanged);
    // Connect audio track selection updates to rip button.
    connect(archiveAudioTracks, &xAudioTracksWidget::isSelectedUpdate, [=]() { archiveAudioTracksExtractButton->setEnabled(isExtractButtonEnabled()); });
}

void xMainArchiveFileWidget::artistOrAlbumChanged(const QString& text) {
    Q_UNUSED(text)
    if (archiveFileArtistName->text().isEmpty() || archiveFileAlbumName->text().isEmpty()) {
        archiveAudioTracksExtractButton->setEnabled(false);
    } else {
        archiveAudioTracksExtractButton->setEnabled(isExtractButtonEnabled());
    }
}

void xMainArchiveFileWidget::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Archive File"),
                                                    QDir::cleanPath(archiveFile->getFileName()),
                                                    tr("Archives (*.tar *.zip)"));
    if (!fileName.isEmpty()) {
        archiveFileName->setText(fileName);
    }
}

void xMainArchiveFileWidget::analyze() {
    // Reset artist, album and track offset on analyzing the file.
    archiveFileArtistName->clear();
    archiveFileAlbumName->clear();
    archiveFile->analyze(archiveFileName->text());
}

void xMainArchiveFileWidget::autofill() {
    archiveFileArtistName->setText(tr("artist"));
    archiveFileAlbumName->setText(tr("album"));
    archiveAudioTracks->autofill();
}

void xMainArchiveFileWidget::archivedFiles(const QVector<QString>& fileNames, const QVector<qint64>& fileSizes) {
    archiveAudioTracks->setEnabled(true);
    archiveAudioTracksSelectButton->setEnabled(true);
    archiveAudioTracksExtractButton->setEnabled(isExtractButtonEnabled());
    archiveAudioTracksExtractCancelButton->setEnabled(false);
    archiveAudioTracks->setTracks(fileSizes.count());
    archiveAudioTracks->setTrackSizes(fileSizes);
    archiveAudioTracks->setTrackNames(fileNames);
}

void xMainArchiveFileWidget::extract() {
    auto jobId = QRandomGenerator::global()->generate64();
    auto tags = xRipEncodeConfiguration::configuration()->getTags();
    auto tagId = static_cast<int>(archiveFileTagHDInputCheck->isChecked());
    qDebug() << "xMainArchiveFileWidget::extract: tags: " << tags;
    archiveFile->queueExtract(getAudioFiles(tags[tagId], tagId, jobId));
    // Disable elements.
    archiveFileArtistName->setEnabled(false);
    archiveFileAlbumName->setEnabled(false);
    archiveFileName->setEnabled(false);
    archiveFileDialogButton->setEnabled(false);
    archiveFileAutofillButton->setEnabled(false);
    archiveFileAnalyzeButton->setEnabled(false);
    archiveFileTagLookupButton->setEnabled(false);
    archiveAudioTracksSelectButton->setEnabled(false);
    archiveAudioTracksExtractButton->setEnabled(false);
    // Only enable "cancel extract" button.
    archiveAudioTracksExtractCancelButton->setEnabled(true);
    // Start rip thread.
    archiveFile->start();
}

void xMainArchiveFileWidget::extractCancel() {
    extractFinished();
}

void xMainArchiveFileWidget::extractFinished() {
    // Enable elements.
    archiveFileArtistName->setEnabled(true);
    archiveFileAlbumName->setEnabled(true);
    archiveFileName->setEnabled(true);
    archiveFileDialogButton->setEnabled(true);
    archiveFileAutofillButton->setEnabled(true);
    archiveFileAnalyzeButton->setEnabled(true);
    archiveFileTagLookupButton->setEnabled(true);
    // Disable audio tracks widget and extract cancel button.
    archiveAudioTracks->setEnabled(false);
    archiveAudioTracksExtractCancelButton->setEnabled(false);
}

void xMainArchiveFileWidget::messages(const QString& msg) {
    consoleText->append(msg);
}

void xMainArchiveFileWidget::lookup() {
    auto selected = archiveFileTagLookupSelectionList->selectedItems();
    if (selected.count() > 0) {
        auto result = archiveFile->extractTags(selected[0]->text());
        archiveFileArtistName->setText(result.artist);
        archiveFileAlbumName->setText(result.album);
        archiveAudioTracks->setTrackNames(result.trackName);
        archiveAudioTracks->setTrackSizes(result.trackSize);
        archiveFileTagHDInputCheck->setChecked(result.bitsPerSample == 24);
    }
}

QList<xAudioFile*> xMainArchiveFileWidget::getAudioFiles(const QString& tag, int tagId, quint64 jobId) {
    auto selectedTracks = archiveAudioTracks->getSelected();
    auto artistName = archiveFileArtistName->text();
    auto albumName = archiveFileAlbumName->text();
    auto fileFormat = xRipEncodeConfiguration::configuration()->getFileNameFormat();
    auto tempDirectory = xRipEncodeConfiguration::configuration()->getTempDirectory();
    fileFormat.replace("(artist)", artistName);
    fileFormat.replace("(album)", albumName);
    // The tag should contain any separators such as a space.
    fileFormat.replace("(tag)", tag);
    QList<xAudioFile*> files;
    for (const auto& track : selectedTracks) {
        auto trackFileFormat = fileFormat;
        trackFileFormat.replace("(tracknr)", std::get<1>(track));
        trackFileFormat.replace("(trackname)", std::get<2>(track));
        if (!trackFileFormat.endsWith(".flac")) {
            trackFileFormat.append(".flac");
        }
        files.push_back(new xAudioFileFlac(tempDirectory+"/"+trackFileFormat, std::get<0>(track), artistName,
                                           albumName, std::get<1>(track), std::get<2>(track), tag, tagId, jobId));
    }
    return files;
}

bool xMainArchiveFileWidget::isExtractButtonEnabled() {
    // Only enable extract button if artist and album is non-empty
    if ((archiveFileArtistName->text().isEmpty()) || (archiveFileAlbumName->text().isEmpty())) {
        return false;
    }
    // And the tracks are selected and the names are valid.
    auto selectedTracks = archiveAudioTracks->getSelected();
    if (selectedTracks.count() > 0) {
        // The track names are not allowed to have any "/".
        for (const auto& selectedTrack : selectedTracks) {
            if (std::get<2>(selectedTrack).contains("/")) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

