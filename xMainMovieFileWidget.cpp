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

#include "xMainMovieFileWidget.h"
#include "xRipEncodeConfiguration.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QDir>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QRandomGenerator>
#include <QDebug>

xMainMovieFileWidget::xMainMovieFileWidget(QWidget *parent, Qt::WindowFlags flags):
        QWidget(parent, flags) {

    auto mainLayout = new QGridLayout(this);
    // Audio CD -  artist and album input and some control.
    auto movieFileBox = new QGroupBox(tr("Movie File"), this);
    movieFileBox->setFlat(xRipEncodeUseFlatGroupBox);
    movieFileArtistName = new QLineEdit(movieFileBox);
    movieFileAlbumName = new QLineEdit(movieFileBox);
    movieFileName = new QLineEdit(movieFileBox);
    movieFileDialogButton = new QPushButton(tr("..."), movieFileBox);
    auto movieFileLayout = new QGridLayout();
    movieFileLayout->addWidget(new QLabel(tr("Artist"), movieFileBox), 0, 0, 1, 1);
    movieFileLayout->addWidget(movieFileArtistName, 1, 0, 1, 6);
    movieFileLayout->addWidget(new QLabel(tr("Album"), movieFileBox), 2, 0, 1, 1);
    movieFileLayout->addWidget(movieFileAlbumName, 3, 0, 1, 6);
    movieFileLayout->setRowMinimumHeight(4, 50);
    movieFileLayout->setRowStretch(4, 0);
    movieFileLayout->addWidget(new QLabel(tr("Filename"), movieFileBox), 5, 0, 1, 6);
    movieFileLayout->addWidget(movieFileName, 6, 0, 1, 5);
    movieFileLayout->addWidget(movieFileDialogButton, 6, 5, 1, 1);
    movieFileLayout->setRowMinimumHeight(7, 50);
    movieFileLayout->setRowStretch(7, 0);
    // Movie file - analyze section.
    auto movieFileAudioTracksLabel = new QLabel(tr("Audio Tracks"), movieFileBox);
    movieFileAudioStreamInfos = new QListWidget(movieFileBox);
    movieFileAudioStreamInfos->setSelectionMode(QListWidget::MultiSelection);
    movieFileAutofillButton = new QPushButton(tr("Autofill"), movieFileBox);
    movieFileAnalyzeButton = new QPushButton(tr("Analyze"), movieFileBox);
    movieFileTrackOffset = new QSpinBox(movieFileBox);
    auto movieFileTrackOffsetLabel = new QLabel(tr("Offset"), movieFileBox);
    movieFileTrackOffsetLabel->setAlignment(Qt::AlignCenter);
    movieFileAudioStreamTag = new QCheckBox(tr("Stream Tag"), movieFileBox);
    movieFileAudioStreamTag->setChecked(false);
    movieFileAudioDownMix = new QCheckBox(tr("Down Mix"), movieFileBox);
    movieFileAudioDownMix->setChecked(false);
    movieFileAudioDownMix->setEnabled(false);
    movieFileLayout->addWidget(movieFileAudioTracksLabel, 8, 0, 1, 6);
    movieFileLayout->addWidget(movieFileAudioStreamInfos, 9, 0, 3, 6);
    movieFileLayout->setRowMinimumHeight(12, 20);
    movieFileLayout->setRowStretch(12, 0);
    movieFileLayout->addWidget(movieFileAudioStreamTag, 13, 0, 1, 2);
    movieFileLayout->addWidget(movieFileAudioDownMix, 13, 2, 1, 2);
    movieFileLayout->addWidget(movieFileTrackOffsetLabel, 13, 4, 1, 1);
    movieFileLayout->addWidget(movieFileTrackOffset, 13, 5, 1, 1);
    movieFileLayout->setRowMinimumHeight(14, 50);
    movieFileLayout->setRowStretch(14, 0);
    movieFileLayout->addWidget(movieFileAnalyzeButton, 15, 0, 1, 3);
    movieFileLayout->addWidget(movieFileAutofillButton, 15, 3, 1, 3);
    movieFileLayout->setRowMinimumHeight(16, 20);
    movieFileLayout->setRowStretch(16, 0);
    movieFileBox->setLayout(movieFileLayout);
    // Audio tracks box.
    auto movieAudioTracksBox = new QGroupBox(tr("Audio Tracks"), this);
    movieAudioTracksBox->setFlat(xRipEncodeUseFlatGroupBox);
    movieAudioTracksSelectButton = new QPushButton(tr("Select All"), movieAudioTracksBox);
    movieAudioTracksRipButton = new QPushButton(tr("Rip Selected"), movieAudioTracksBox);
    movieAudioTracksRipButton->setEnabled(false);
    movieAudioTracksRipCancelButton = new QPushButton(tr("Cancel Rip"), movieAudioTracksBox);
    movieAudioTracksRipCancelButton->setEnabled(false);
    movieAudioTracks = new xAudioTracksWidget(movieAudioTracksBox);
    auto movieAudioTracksLayout = new QGridLayout();
    movieAudioTracksLayout->addWidget(movieAudioTracksSelectButton, 0, 0, 1, 2);
    movieAudioTracksLayout->setRowMinimumHeight(1, 20);
    movieAudioTracksLayout->setRowStretch(1, 0);
    movieAudioTracksLayout->addWidget(movieAudioTracks, 2, 0, 6, 2);
    movieAudioTracksLayout->setRowMinimumHeight(8, 20);
    movieAudioTracksLayout->setRowStretch(8, 0);
    movieAudioTracksLayout->addWidget(movieAudioTracksRipButton, 9, 0, 1, 1);
    movieAudioTracksLayout->addWidget(movieAudioTracksRipCancelButton, 9, 1, 1, 1);
    movieAudioTracksBox->setLayout(movieAudioTracksLayout);
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
    mainLayout->addWidget(movieFileBox, 0, 0, 7, 4);
    mainLayout->addWidget(consoleBox, 7, 0, 3, 4);
    mainLayout->addWidget(movieAudioTracksBox, 0, 4, 10, 4);
    // Create audio ripper object and connect object.
    movieFile = new xMovieFile(this);
    connect(movieFile, &xMovieFile::audioStreamInfos, this, &xMainMovieFileWidget::audioStreamInfos);
    connect(movieFile, &xMovieFile::trackLengths, this, &xMainMovieFileWidget::trackLengths);
    connect(movieFile, &xMovieFile::messages, this, &xMainMovieFileWidget::messages);
    connect(movieFile, &xMovieFile::ripProgress, movieAudioTracks, &xAudioTracksWidget::ripProgress);
    connect(movieFile, &xMovieFile::finished, this, &xMainMovieFileWidget::ripFinished);
    // Connect signals for audio files.
    connect(movieFile, &xMovieFile::audioFiles, this, &xMainMovieFileWidget::audioFiles);
    // Connect buttons.
    connect(movieFileDialogButton, &QPushButton::pressed, this, &xMainMovieFileWidget::openFile);
    connect(movieFileAnalyzeButton, &QPushButton::pressed, this, &xMainMovieFileWidget::analyze);
    connect(movieFileAutofillButton, &QPushButton::pressed, this, &xMainMovieFileWidget::autofill);
    connect(movieAudioTracksSelectButton, &QPushButton::pressed, movieAudioTracks, &xAudioTracksWidget::selectAll);
    connect(movieAudioTracksRipButton, &QPushButton::pressed, this, &xMainMovieFileWidget::rip);
    connect(movieAudioTracksRipCancelButton, &QPushButton::pressed, this, &xMainMovieFileWidget::ripCancel);
    // Connect album and artist LineEdit.
    connect(movieFileArtistName, &QLineEdit::textChanged, this, &xMainMovieFileWidget::artistOrAlbumChanged);
    connect(movieFileAlbumName, &QLineEdit::textChanged, this, &xMainMovieFileWidget::artistOrAlbumChanged);
    // Connect audio stream infos.
    connect(movieFileAudioStreamInfos, &QListWidget::itemSelectionChanged, this, &xMainMovieFileWidget::audioStreamInfoSelection);
    // Update track offset.
    connect(movieFileTrackOffset, SIGNAL(valueChanged(int)), movieAudioTracks, SLOT(setTrackOffset(int)));
    // Connect audio track selection updates to rip button.
    connect(movieAudioTracks, &xAudioTracksWidget::isSelectedUpdate, [=]() { movieAudioTracksRipButton->setEnabled(isRipButtonEnabled()); });
}

void xMainMovieFileWidget::artistOrAlbumChanged(const QString& text) {
    Q_UNUSED(text)
    if (movieFileArtistName->text().isEmpty() || movieFileAlbumName->text().isEmpty()) {
        movieAudioTracksRipButton->setEnabled(false);
    } else {
        movieAudioTracksRipButton->setEnabled(isRipButtonEnabled());
    }
}

void xMainMovieFileWidget::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie File"),
                                                    QDir::cleanPath(movieFile->getFileName()),
                                                    tr("Movies (*.mkv *.mp4)"));
    if (!fileName.isEmpty()) {
        movieFileName->setText(fileName);
    }
}

void xMainMovieFileWidget::analyze() {
    // Reset artist, album and track offset on analyzing the file.
    movieFileArtistName->clear();
    movieFileAlbumName->clear();
    movieFileTrackOffset->setValue(0);
    movieFile->analyze(movieFileName->text());
}

void xMainMovieFileWidget::autofill() {
    movieFileArtistName->setText(tr("artist"));
    movieFileAlbumName->setText(tr("album"));
    movieAudioTracks->autofill();
}

void xMainMovieFileWidget::audioStreamInfos(const QStringList& infos) {
    movieFileAudioStreamInfos->clear();
    movieFileAudioStreamInfos->addItems(infos);
}

void xMainMovieFileWidget::audioStreamInfoSelection() {
    auto items = movieFileAudioStreamInfos->selectedItems();
    if (items.count() > 1) {
        movieFileAudioStreamTag->setChecked(true);
        movieFileAudioStreamTag->setEnabled(false);
    } else {
        movieFileAudioStreamTag->setEnabled(true);
    }
    movieAudioTracksRipButton->setEnabled(isRipButtonEnabled());
    for (const auto& item : items) {
        auto streamInfo = movieFile->getAudioStreamInfo(movieFileAudioStreamInfos->row(item));
        if (streamInfo.channels > 2) {
            movieFileAudioDownMix->setEnabled(true);
            return;
        }
    }
    // Reset down mix.
    movieFileAudioDownMix->setChecked(false);
    movieFileAudioDownMix->setEnabled(false);
}

void xMainMovieFileWidget::trackLengths(const QVector<qint64>& lengths) {
    movieAudioTracks->setEnabled(true);
    movieAudioTracksSelectButton->setEnabled(true);
    movieAudioTracks->setTracks(lengths.count());
    movieAudioTracks->setTrackLengths(lengths);
    movieAudioTracksRipButton->setEnabled(isRipButtonEnabled());
}

void xMainMovieFileWidget::rip() {
    auto jobId = QRandomGenerator::global()->generate64();
    auto audioStreams = movieFileAudioStreamInfos->selectedItems();
    auto downMix = movieFileAudioDownMix->isChecked();
    auto tags = xRipEncodeConfiguration::configuration()->getTags();
    qDebug() << "xMainMovieFileWidget::rip: tags: " << tags;
    for (const auto& audioStream : audioStreams) {
        auto audioStreamIndex = movieFileAudioStreamInfos->row(audioStream);
        auto audioStreamInfo = movieFile->getAudioStreamInfo(audioStreamIndex);
        if (audioStreamInfo.channels > 2) {
            if (audioStreamInfo.bitsPerSample > 16) {
                movieFile->queueRip(getAudioFiles(tags[3].arg(audioStreamInfo.channels-1), 3, jobId), audioStreamIndex, false);
                if (downMix) {
                    movieFile->queueRip(getAudioFiles(tags[1], 1, jobId), audioStreamIndex, true);
                }
            } else {
                movieFile->queueRip(getAudioFiles(tags[2].arg(audioStreamInfo.channels-1), 2, jobId), audioStreamIndex, false);
                if (downMix) {
                    movieFile->queueRip(getAudioFiles(tags[0], 0, jobId), audioStreamIndex, true);
                }
            }
        } else {
            if (audioStreamInfo.bitsPerSample > 16) {
                movieFile->queueRip(getAudioFiles(tags[1], 1, jobId), audioStreamIndex, false);
            } else {
                movieFile->queueRip(getAudioFiles(tags[0], 0, jobId), audioStreamIndex, false);
            }
        }
    }
    // Disable elements.
    movieFileArtistName->setEnabled(false);
    movieFileAlbumName->setEnabled(false);
    movieFileName->setEnabled(false);
    movieFileDialogButton->setEnabled(false);
    movieFileAutofillButton->setEnabled(false);
    movieFileAnalyzeButton->setEnabled(false);
    movieFileAudioStreamInfos->setEnabled(false);
    movieAudioTracksSelectButton->setEnabled(false);
    movieAudioTracksRipButton->setEnabled(false);
    // Only enable "cancel rip" button.
    movieAudioTracksRipCancelButton->setEnabled(true);
    // Start rip thread.
    movieFile->start();
}

void xMainMovieFileWidget::ripCancel() {
    ripFinished();
}

void xMainMovieFileWidget::ripMessage(int track, const QString& message) {
    if (track > 0) {
        consoleText->append(QString("(track %1) %2").arg(track).arg(message));
    } else {
        consoleText->append(message);
    }
}

void xMainMovieFileWidget::ripError(int track, const QString& error, bool abort) {
    QString errorMessage;
    if (track > 0) {
        errorMessage = QString("(track %1) %2").arg(track).arg(error);
    } else {
        errorMessage = error;
    }
    consoleText->append(QString("[%1] %2").arg((abort)?"abort":"error").arg(errorMessage));
}

void xMainMovieFileWidget::ripFinished() {
    // Enable elements.
    movieFileArtistName->setEnabled(true);
    movieFileAlbumName->setEnabled(true);
    movieFileName->setEnabled(true);
    movieFileDialogButton->setEnabled(true);
    movieFileAutofillButton->setEnabled(true);
    movieFileAnalyzeButton->setEnabled(true);
    movieFileAudioStreamInfos->setEnabled(true);
    movieAudioTracksSelectButton->setEnabled(true);
    // Disable the entire audio tracks view. Need to analyze before enabling.
    movieAudioTracksSelectButton->setEnabled(false);
    movieAudioTracksRipCancelButton->setEnabled(false);
    movieAudioTracksRipButton->setEnabled(false);
    movieAudioTracks->setEnabled(false);
}

void xMainMovieFileWidget::messages(const QString& msg) {
    consoleText->append(msg);
}

QList<xAudioFile*> xMainMovieFileWidget::getAudioFiles(const QString& tag, int tagId, quint64 jobId) {
    auto selectedTracks = movieAudioTracks->getSelected();
    auto artistName = movieFileArtistName->text();
    auto albumName = movieFileAlbumName->text();
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
        trackFileFormat.append(".wav");
        files.push_back(new xAudioFileWav(tempDirectory+"/"+trackFileFormat, std::get<0>(track), artistName,
                                          albumName, std::get<1>(track), std::get<2>(track), tag, tagId, jobId));
    }
    return files;
}

bool xMainMovieFileWidget::isRipButtonEnabled() {
    // Only enable rip button if artist and album is non-empty
    // and the audio streams and tracks are selected.
    return (movieFileAudioStreamInfos->selectedItems().count() != 0) &&
           (movieAudioTracks->isSelected()) &&
           (!movieFileArtistName->text().isEmpty()) &&
           (!movieFileAlbumName->text().isEmpty()) &&
           (movieFile->getTracks() > 0);
}

