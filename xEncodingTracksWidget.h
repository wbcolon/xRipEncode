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
     * Set the offset to actual track or chapter.
     *
     * @param offset the offset as integer.
     */
    void setTrackNrOffset(int offset);
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

private slots:
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
     * Set the track offset for all tracks.
     *
     * @param offset the new offset as integer.
     */
    void setTrackOffset(int offset);
    /**
     * Select all tracks.
     */
    void selectAll();
    /**
     * Deselect all tracks.
     */
    void deselectAll();
    /**
     * Update the progress of the rip process.
     *
     * @param track number of the track currently ripped.
     * @param progress the progress for the current track.
     */
    void ripProgress(int track, int progress);

private:
    /**
     * Change focus order for cycling through with Tab and Shift+Tab.
     */
    void updateTabOrder();

    int audioTracksOffset;
    QWidget* audioMain;
    QVBoxLayout* audioLayout;
    QVector<xEncodingTrackItemWidget*> encodingTracks;
};

#endif
