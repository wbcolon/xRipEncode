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

class xAudioTrackItemWidget:public QWidget {
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param track the track number of the audio CD.
     * @param offset the offset in the filename.
     * @param parent pointer to the parent object.
     */
    explicit xAudioTrackItemWidget(int track, int offset=0, QWidget* parent=nullptr);
    /**
     * Destructor.
     */
    ~xAudioTrackItemWidget() override = default;
    /**
     * Retrieve the track number for the audio CD.
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
     * Retrieve the currently set offset for the file name.
     *
     * @return the offset to the track number as integer.
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
     * @param length the track length as proper formatted as string.
     */
    void setTrackLength(const QString& length);
    /**
     * Select/deselect the current track.
     *
     * @param select select if the true, deselect otherwise.
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

private:
    int audioTrackNr;
    int audioTrackOffset;
    QCheckBox* trackSelect;
    QLineEdit* trackNr;
    QLineEdit* trackName;
    QLabel* trackLength;
    QProgressBar* trackProgress;
    QStackedWidget* trackStacked;
};

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
     * Autofill the track names with a generic name.
     */
    void autofill();
    /**
     * Set the number of tracks.
     *
     * @param tracks the new number of tracks as integer.
     */
    void setTracks(int tracks);
    /**
     * Set the track names.
     *
     * @param names the new track names as vector of strings.
     */
    void setTrackNames(const QVector<QString>& names);
    /**
     * Set the track times.
     *
     * @param lengths a vector of track lengths in ms.
     */
    void setTrackLengths(const QVector<qint64>& lengths);
    /**
     * Clear the widget. Remove all tracks.
     */
    void clear();
    /**
     * Return the selected tracks.
     *
     * @return a list of tuples of audio track index, track number and track name.
     */
    [[nodiscard]] QList<std::tuple<int,QString,QString>> isSelected();

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

private:
    static QString millisecondsToLabel(qint64 ms);


    int audioTracksOffset;
    QWidget* audioMain;
    QVBoxLayout* audioLayout;
    QVector<xAudioTrackItemWidget*> audioTracks;
};

#endif
