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

#ifndef __XMAINMOVIEFILEWIDGET_H__
#define __XMAINMOVIEFILEWIDGET_H__

#include "xMovieFile.h"
#include "xAudioTracksWidget.h"
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QListWidget>
#include <QTextEdit>
#include <QWidget>

class xMainMovieFileWidget: public QWidget {
Q_OBJECT

public:
    /**
     * Constructor. Create movie file chapter ripper widget.
     *
     * @param parent pointer to the parent widget.
     * @param flags window flags for the widget.
     */
    explicit xMainMovieFileWidget(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    /**
     * Destructor. Default.
     */
    ~xMainMovieFileWidget() override = default;

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
     * Start a rip thread with the currently selected movie tracks (chapters) and update widget.
     */
    void rip();
    /**
     * Cancel a currently running rip thread.
     */
    void ripCancel();
    /**
     * Output messages for the rip thread.
     *
     * @param track the ripped track (chapter) this message belongs to.
     * @param message the message for the ripped track (chapter).
     */
    void ripMessage(int track, const QString& message);
    /**
     * Output error messages for the rip thread.
     *
     * @param track the ripped track (chapter) this error message belongs to.
     * @param error the error message for the ripped track (chapter).
     * @param abort indicated if rip for current track (chapter) was aborted.
     */
    void ripError(int track, const QString& error, bool abort);
    /**
     * Update widget upon finishing the rip thread.
     */
    void ripFinished();
    /**
     * Update the combo box with infos of the audio streams of the movie file.
     *
     * @param infos the audio stream infos as list of strings.
     */
    void audioStreamInfos(const QStringList& infos);
    /**
     * Update widget if selection of audio streams is changed.
     */
    void audioStreamInfoSelection();
    /**
     * Set the track lengths of individual tracks (chapters) of the movie file.
     *
     * @param lengths the lengths of the tracks (chapters) in milliseconds.
     */
    void trackLengths(const QVector<qint64>& lengths);
    /**
     * Update the console.
     *
     * @param msg the message appended to the console as string.
     */
    void messages(const QString& msg);

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

    bool isRipButtonEnabled();

    QLineEdit* movieFileArtistName;
    QLineEdit* movieFileAlbumName;
    QLineEdit* movieFileName;
    QPushButton* movieFileAutofillButton;
    QPushButton* movieFileAnalyzeButton;
    QPushButton* movieFileDialogButton;
    QSpinBox* movieFileTrackOffset;
    QCheckBox* movieFileAudioDownMix;
    QCheckBox* movieFileAudioStreamTag;
    QListWidget* movieFileAudioStreamInfos;
    xAudioTracksWidget* movieAudioTracks;
    QPushButton* movieAudioTracksSelectButton;
    QPushButton* movieAudioTracksRipButton;
    QPushButton* movieAudioTracksRipCancelButton;
    QTextEdit* consoleText;
    xMovieFile* movieFile;
};

#endif
