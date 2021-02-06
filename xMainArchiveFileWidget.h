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

#ifndef __XMAINARCHIVEFILEWIDGET_H__
#define __XMAINARCHIVEFILEWIDGET_H__

#include "xArchiveFile.h"
#include "xAudioTracksWidget.h"
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QListWidget>
#include <QTextEdit>
#include <QWidget>

class xMainArchiveFileWidget: public QWidget {
    Q_OBJECT

public:
    /**
     * Constructor. Create movie file chapter ripper widget.
     *
     * @param parent pointer to the parent widget.
     * @param flags window flags for the widget.
     */
    explicit xMainArchiveFileWidget(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    /**
     * Destructor. Default.
     */
    ~xMainArchiveFileWidget() override = default;

signals:
    /**
     * Signal the list of successfully ripped movie chapter audio tracks.
     *
     * @param tracks list of audio file objects containing the necessary information.
     */
    void audioFiles(const QList<xAudioFile*>& files);

private slots:
    /**
     * Update buttons on changes of input for artist or album.
     *
     * @param text the new input as string (ignored).
     */
    void artistOrAlbumChanged(const QString& text);
    /**
     * Open a file dialog in order to select a movie file (.mkv or .mp4).
     */
    void openFile();
    /**
     * Analzye the currently selected movie file.
     */
    void analyze();
    /**
     * Autofill the artist/album/tracks with generic names.
     */
    void autofill();
    /**
     * Start an extract thread for the currently selected files and update widget.
     */
    void extract();
    /**
     * Cancel a currently running extract thread.
     */
    void extractCancel();
    /**
     * Update widget upon finishing the extract thread.
     */
    void extractFinished();
    /**
     * Set the file names of individual files within the archive file.
     *
     * @param fileNames a vector of file names.
     * @param fileSizes a vector of file sizes in bytes.
     */
    void archivedFiles(const QVector<QString>& fileNames, const QVector<qint64>& fileSizes);
    /**
     * Update the console.
     *
     * @param msg the message appended to the console as string.
     */
    void messages(const QString& msg);

    void lookup();

private:
    /**
     * Create a list of audio file objects based successfully ripped audio tracks.
     *
     * @param tag the HD/multi-channel tag to be used.
     * @param tagId the corresponding tag ID.
     * @param jobId the job ID the files belong to.
     * @return a list of audio file objects containing the necessary information.
     */
    QList<xAudioFile*> getAudioFiles(const QString& tag, int tagId, quint64 jobId);
    /**
     * Determine the state of the extract button.
     *
     * @return true if we can enable the extract button, false otherwise.
     */
    bool isExtractButtonEnabled();

    QLineEdit* archiveFileArtistName;
    QLineEdit* archiveFileAlbumName;
    QLineEdit* archiveFileName;
    QPushButton* archiveFileAutofillButton;
    QPushButton* archiveFileAnalyzeButton;
    QPushButton* archiveFileDialogButton;
    QListWidget* archiveFileTagLookupSelectionList;
    QPushButton* archiveFileTagLookupButton;
    xAudioTracksWidget* archiveAudioTracks;
    QPushButton* archiveAudioTracksSelectButton;
    QPushButton* archiveAudioTracksExtractButton;
    QPushButton* archiveAudioTracksExtractCancelButton;
    QCheckBox* archiveFileTagHDInputCheck;
    QTextEdit* consoleText;
    xArchiveFile* archiveFile;
};

#endif
