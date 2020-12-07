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

#ifndef __XMAINAUDIOCDWIDGET_H__
#define __XMAINAUDIOCDWIDGET_H__

#include "xAudioCD.h"
#include "xAudioFile.h"
#include "xAudioTracksWidget.h"
#include "xReplaceWidget.h"
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QListWidget>
#include <QTextEdit>
#include <QWidget>

class xMainAudioCDWidget: public QWidget {
    Q_OBJECT

public:
    /**
     * Constructor. Create audio CD ripper widget.
     *
     * @param parent pointer to the parent widget.
     * @param flags window flags for the widget.
     */
    explicit xMainAudioCDWidget(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    /**
     * Destructor. Default.
     */
    ~xMainAudioCDWidget() override = default;

private slots:
    /**
     * Update buttons on changes of input for artist or album.
     *
     * @param text the new input as string (ignored).
     */
    void artistOrAlbumChanged(const QString& text);
    /**
     * Try to detect an audio CD in the system. Widget is updated if successful.
     */
    void detect();
    /**
     * Eject the audio CD if present. Update widget if necessary.
     */
    void eject();
    /**
     * Start a music brainz lookup thread for the current audio CD.
     */
    void musicBrainz();
    /**
     * Update the widget based upon the results of the music brainz lookup thread.
     */
    void musicBrainzFinished();
    /**
     * Update the artist/album/tracks based upon the music brainz results.
     *
     * @param index the index of the music brainz results to be used.
     */
    void musicBrainzUpdate(int index);
    /**
     * Autofill the artist/album/tracks with generic names.
     */
    void autofill();
    /**
     * Start a rip thread with the currently selected audio CD tracks and update widget.
     */
    void rip();
    /**
     * Cancel a currently running rip thread.
     */
    void ripCancel();
    /**
     * Output messages for the rip thread.
     *
     * @param track the ripped track this message belongs to.
     * @param message the message for the ripped track.
     */
    void ripMessage(int track, const QString& message);
    /**
     * Output error messages for the rip thread.
     *
     * @param track the ripped track this error message belongs to.
     * @param error the error message for the ripped track.
     * @param abort indicated if rip for current track was aborted.
     */
    void ripError(int track, const QString& error, bool abort);
    /**
     * Update widget upon finishing the rip thread.
     */
    void ripFinished();

signals:
    /**
     * Signal the list of successfully ripped audio tracks.
     *
     * @param tracks list of audio file objects containing the necessary information.
     */
    void audioFiles(const QList<xAudioFile*>& tracks);

private:
    /**
     * Create a list of audio file objects based successfully ripped audio tracks.
     *
     * @return a list of audio file objects containing the necessary information.
     */
    QList<xAudioFile*> getTracks();
    /**
     * Update strings based on replace strings and lowercase mode.
     *
     * @param text the input as string.
     * @return an updated string based on the widgets settings.
     */
    QString updateString(const QString& text);

    QLineEdit* audioCDArtistName;
    QLineEdit* audioCDAlbumName;
    QPushButton* audioCDDetectButton;
    QPushButton* audioCDEjectButton;
    QPushButton* audioCDAutofillButton;
    QPushButton* audioCDLookupButton;
    QComboBox* audioCDLookupResults;
    QSpinBox* audioCDTrackOffset;
    QCheckBox* audioCDLowerCase;
    QCheckBox* audioCDReplace;
    xReplaceWidget* audioCDReplaceView;
    xAudioTracksWidget* audioTracks;
    QPushButton* audioTracksSelectButton;
    QPushButton* audioTracksRipButton;
    QPushButton* audioTracksRipCancelButton;
    QTextEdit* consoleText;
    xAudioCD* audioCD;
    xAudioCDLookup* audioCDLookup;
    QList<xAudioCDLookup::xAudioCDLookupResult> lookupResults;
};

#endif
