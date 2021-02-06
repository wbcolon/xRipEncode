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

#ifndef __XAUDIOTRACKSWIDGET_H__
#define __XAUDIOTRACKSWIDGET_H__

#include <QScrollArea>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QProgressBar>
#include <QList>
#include <QLabel>
#include <QWidget>

class xAudioTracksWidget;

class xAudioTrackItemWidget:public QWidget {
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * The numbering for the track number of the audio CD or the
     * chapter (track) of a movie file starts with index 1.
     *
     * @param track the track number of the audio CD or movie file.
     * @param offset the offset in the file name.
     * @param parent pointer to the parent object.
     */
    explicit xAudioTrackItemWidget(int track, int offset=0, QWidget* parent=nullptr);
    /**
     * Destructor.
     */
    ~xAudioTrackItemWidget() override = default;
    /**
     * Retrieve the track number for the audio CD or movie file.
     *
     * @return the track number as integer.
     */
    [[nodiscard]] int getAudioTrackNr() const;
    /**
     * Set the offset for the file name.
     *
     * @param offset the offset to the track number as integer.
     */
    void setAudioTrackOffset(int offset);
    /**
     * Retrieve the current offset used in the file name.
     *
     * @return the offset of the track number as integer.
     */
    [[nodiscard]] int getAudioTrackOffset() const;
    /**
     * Retrieve the track number used in the file name.
     *
     * @return the track number as string.
     */
    [[nodiscard]] QString getTrackNr() const;
    /**
     * Set the track name used in the file name.
     *
     * @param name the new track name as string.
     */
    void setTrackName(const QString& name);
    /**
     * Retrieve the track name used in the file name.
     *
     * @return the current track name as string.
     */
    [[nodiscard]] QString getTrackName() const;
    /**
     * Set the track length for file name.
     *
     * @param length the track length as formatted as string.
     */
    void setTrackLength(const QString& length);
    /**
     * Set the track size for file name.
     *
     * @param size the track size as string.
     */
    void setTrackSize(const QString& size);
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
     * Switch to the track name view of the widget.
     *
     * @param autofill set a generic name if true.
     */
    void trackInput(bool autofill=false);
    /**
     * Switch to the progress view of the widget.
     *
     * @param progress the progress in percent.
     */
    void ripProgress(int progress);

signals:
    /**
     * Emitted if this tracks selection changed.
     */
    void isSelectedUpdate();

private:
    // we grant access in order to change behavior on keys Tab and Shift+Tab.
    friend xAudioTracksWidget;

    int audioTrackNr;
    int audioTrackOffset;
    QCheckBox* trackSelect;
    QLineEdit* trackNr;
    QLineEdit* trackName;
    QLabel* trackLength;
    QProgressBar* trackProgress;
    QStackedWidget* trackStacked;
};

/**
 * @class xAudioTrackWidget
 *
 * @note An audio track represents either a dedicated track on an audio
 * CD or a section of an audio stream of a movie file. The section in a
 * movie file (named track) is usually accessed through chapters in the
 * movie file.
 */
class xAudioTracksWidget:public QScrollArea {
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param parent pointer to the parent object.
     */
    explicit xAudioTracksWidget(QWidget* parent=nullptr);
    /**
     * Destructor
     */
    ~xAudioTracksWidget() override = default;
    /**
     * Set all track names using the generic name "track".
     */
    void autofill();
    /**
     * Set the number of tracks.
     *
     * @param tracks the new number of tracks as integer.
     */
    void setTracks(int tracks);
    /**
     * Set all track names.
     *
     * @param names the new track names as vector of strings.
     */
    void setTrackNames(const QVector<QString>& names);
    /**
     * Set the lengths for each track.
     *
     * @param lengths a vector of track lengths in ms.
     */
    void setTrackLengths(const QVector<qint64>& lengths);
    /**
     * Set the size for each track.
     *
     * @param sizes a vector of track sizes in byte.
     */
    void setTrackSizes(const QVector<qint64>& sizes);
    /**
     * Clear the widget. Remove all tracks.
     */
    void clear();
    /**
     * Return if any of the tracks are selected.
     *
     * @return true if one or more tracks are selected, false otherwise.
     */
    [[nodiscard]] bool isSelected();
    /**
     * Return the selected tracks.
     *
     * @return a list of tuples of audio track index, track number and track name.
     */
    [[nodiscard]] QList<std::tuple<int,QString,QString>> getSelected();

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
     * Update the progress of the rip process.
     *
     * @param track number of the track currently ripped.
     * @param progress the progress for the current track.
     */
    void ripProgress(int track, int progress);

signals:
    /**
     * Emitted if audio tracks selection changes.
     */
    void isSelectedUpdate();

private:
    /**
     * Change focus order for cycling through with Tab and Shift+Tab.
     */
    void updateTabOrder();
    /**
     * Convert the time in ms to a human readable time string.
     *
     * @param ms the time to be converted in ms.
     * @return the human readable time string.
     */
    static QString millisecondsToLabel(qint64 ms);

    int audioTracksOffset;
    QWidget* audioMain;
    QVBoxLayout* audioLayout;
    QVector<xAudioTrackItemWidget*> audioTracks;
};

#endif
