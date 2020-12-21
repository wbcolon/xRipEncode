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

#ifndef __XENCODINGTRACKSWIDGET_H__
#define __XENCODINGTRACKSWIDGET_H__

#include "xAudioFile.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QStackedWidget>
#include <QProgressBar>
#include <QList>
#include <QLabel>
#include <QWidget>

class xEncodingTracksWidget;

class xEncodingTrackItemWidget:public QWidget {
    Q_OBJECT

public:
    explicit xEncodingTrackItemWidget(xAudioFile* file, QWidget* parent=nullptr);
    /**
     * Destructor.
     */
    ~xEncodingTrackItemWidget() override = default;
    /**
     * Set the artist used in the file name.
     *
     * @param artist the new artist name as string.
     */
    void setArtist(const QString& artist);
    /**
     * Set the album used in the file name.
     *
     * @param album the new album name as string.
     */
    void setAlbum(const QString& album);
    /**
     * Set the HD/multi-channel tag used in the file name.
     *
     * @param tag the HD/multi-channel tag as string.
     */
    void setTag(const QString& tag);
    /**
     * Set the track number used in the file name.
     *
     * @param nr the new track number as string.
     */
    void setTrackNr(const QString& nr);
    /**
     * Set the track name used in the file name.
     *
     * @param name the new track name as string.
     */
    void setTrackName(const QString& name);
    /**
     * Set the format string used to determine the file name.
     *
     * @param format the new format as string.
     */
    void setEncodedFormat(const QString& format);
    /**
     * Retrieve the artist used in the file name.
     *
     * @return the artist as string.
     */
    [[nodiscard]] QString getArtist() const;
    /**
     * Retrieve the album used in the file name.
     *
     * @return the album as string.
     */
    [[nodiscard]] QString getAlbum() const;
    /**
     * Retrieve the HD/multi-channel tag used in the file name.
     *
     * @return the HD/multi-channel tag as string.
     */
    [[nodiscard]] QString getTag() const;
    /**
     * Retrieve the track number used in the file name.
     *
     * @return the track number as string.
     */
    [[nodiscard]] QString getTrackNr() const;
    /**
     * Retrieve the track name used in the file name.
     *
     * @return the current track name as string.
     */
    [[nodiscard]] QString getTrackName() const;
    /**
     * Retrieve the file name for the encoded file.
     *
     * @return the current file name as string.
     */
    [[nodiscard]] QString getEncodedFileName() const;
    /**
     * Retrieve the job ID the connected audio file belongs to.
     *
     * @return the job ID as integer.
     */
    [[nodiscard]] quint64 getJobId() const;
    /**
     * Select/deselect the current track.
     *
     * @param select select if true, deselect otherwise.
     */
    void setSelected(bool select);
    /**
     * Retrieve the selected status of the current track.
     *
     * @return true if the current track is selected, false otherwise.
     */
    [[nodiscard]] bool isSelected() const;
    /**
     * Retrieve the audio file object attached to the item.
     *
     * @return pointer to the audio file object.
     */
    [[nodiscard]] xAudioFile* getAudioFile() const;
    /**
     * Switch to the output file name view of the widget.
     *
     * @param autofill set a generic name if true.
     */
    void viewOutput(bool autofill=false);
    /**
     * Switch to the track name input view of the widget.
     */
    void viewInput();
    /**
     * Switch to the progress view of the widget.
     *
     * @param progress the progress in percent.
     */
    void ripProgress(int progress);

signals:
    /**
     * Signal triggered if artist is updated.
     *
     * @param item pointer to the encoding track item.
     */
    void updateArtist(xEncodingTrackItemWidget* item);
    /**
     * Signal triggered if album is updated.
     *
     * @param item pointer to the encoding track item.
     */
    void updateAlbum(xEncodingTrackItemWidget* item);
    /**
     * Signal triggered if tag is updated.
     *
     * @param item pointer to the encoding track item.
     */
    void updateTag(xEncodingTrackItemWidget* item);
    /**
     * Signal triggered if track number is updated.
     *
     * @param item pointer to the encoding track item.
     */
    void updateTrackNr(xEncodingTrackItemWidget* item);

private slots:
    /**
     * Triggered upon changes in the artist input and emits signal updateArtist.
     *
     * @param text the new content of the artist input as string.
     */
    void updatedArtist(const QString& text);
    /**
     * Triggered upon changes in the album input and emits signal updateAlbum.
     *
     * @param text the new content of the album input as string.
     */
    void updatedAlbum(const QString& text);
    /**
     * Triggered upon changes in the tag input and emits signal updateTag.
     *
     * @param text the new content of the tag input as string.
     */
    void updatedTag(const QString& text);
    /**
     * Triggered upon changes in the track number input and emits signal updateTrackNr.
     *
     * @param text the new content of the tag input as string.
     */
    void updatedTrackNr(const QString& text);
    /**
     * Update the file name based on the format string and the widgets current input.
     */
    void updateEncodedFileName();
    /**
     * Toggle from in between input and output view.
     */
    void toggleViews();

private:
    // Grant access in order to allow for changing the focus order.
    friend xEncodingTracksWidget;

    xAudioFile* audioFile;
    QString encodedFormat;
    QPushButton* editInfo;
    QLineEdit* encodedFileName;
    QProgressBar* encodedProgress;
    QWidget* trackInfo;
    QLineEdit* artistName;
    QLineEdit* albumName;
    QLineEdit* tagName;
    QLineEdit* trackNr;
    QLineEdit* trackName;
    QCheckBox* trackSelect;
    QStackedWidget* mainStacked;
};

/**
 * @class xEncodingTrackWidget
 *
 * @note An audio track represents either a dedicated track on an audio
 * CD or a section of an audio stream of a movie file. The section in a
 * movie file (named track) is usually accessed through chapters in the
 * movie file.
 */
class xEncodingTracksWidget:public QScrollArea {
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param parent pointer to the parent object.
     */
    explicit xEncodingTracksWidget(QWidget* parent=nullptr);
    /**
     * Destructor. Default
     */
    ~xEncodingTracksWidget() override = default;
    /**
     * Select the input view for all items.
     */
    void viewInput();
    /**
     * Select the output view for all items.
     *
     * @param autofill use generic names for all items if true.
     */
    void viewOutput(bool autofill=false);
    /**
     * Create all items with attached audio file objects.
     *
     * @param names the new track names as vector of strings.
     */
    void setTracks(const QVector<xAudioFile*>& files);
    /**
     * Set the format string for all items.
     *
     * @param format the new format as string.
     */
    void setEncodedFormat(const QString& format);
    /**
     * Retrieve the currently selected items.
     *
     * @return a list of pointer to the item widgets.
     */
    [[nodiscard]] QList<xEncodingTrackItemWidget*> getSelected();
    /**
     * Clear the widget. Remove all items.
     */
    void clear();

public slots:
    /**
     * Select all tracks.
     */
    void selectAll();
    /**
     * Deselect all tracks.
     */
    void deselectAll();
    /**
     * Set the mode for the updating the artist within a job ID.
     *
     * @param enabled update the artist if true, do not update otherwise.
     */
    void setUpdateArtist(bool enabled);
    /**
     * Set the mode for the updating the album within a job ID.
     *
     * @param enabled update the album if true, do not update otherwise.
     */
    void setUpdateAlbum(bool enabled);
    /**
     * Set the mode for the updating the tag within a job ID.
     *
     * @param enabled update the tag if true, do not update otherwise.
     */
    void setUpdateTag(bool enabled);
    /**
     * Set the mode for the updating the track number within a job ID.
     *
     * @param enabled update the track number if true, do not update otherwise.
     */
    void setUpdateTrackNr(bool enabled);
    /**
     * Update the progress of the rip process.
     *
     * @param track number of the track currently ripped.
     * @param progress the progress for the current track.
     */
    void ripProgress(int track, int progress);

private slots:
    /**
     * Perform smart update of the artist for all entries that match its job ID.
     *
     * @param item pointer to the encoding track item object updated.
     */
    void updateArtist(xEncodingTrackItemWidget* item);
    /**
     * Perform smart update of the album for all entries that match its job ID.
     *
     * @param item pointer to the encoding track item object updated.
     */
    void updateAlbum(xEncodingTrackItemWidget* item);
    /**
     * Perform smart update of the tag for all entries that match its job ID.
     *
     * @param item pointer to the encoding track item object updated.
     */
    void updateTag(xEncodingTrackItemWidget* item);
    /**
     * Perform smart update of the track number for all entries that match its job ID.
     *
     * @param item pointer to the encoding track item object updated.
     */
    void updateTrackNr(xEncodingTrackItemWidget* item);

private:
    /**
     * Change focus order for cycling through with Tab and Shift+Tab.
     */
    void updateTabOrder();

    QWidget* audioMain;
    QVBoxLayout* audioLayout;
    QVector<xEncodingTrackItemWidget*> encodingTracks;
    bool updateArtistEnabled;
    bool updateAlbumEnabled;
    bool updateTagEnabled;
    bool updateTrackNrEnabled;
};

#endif
