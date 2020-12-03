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


class xEncodingTrackItemWidget:public QWidget {
    Q_OBJECT

public:
    explicit xEncodingTrackItemWidget(xAudioFile* file, QWidget* parent=nullptr);
    /**
     * Destructor.
     */
    ~xEncodingTrackItemWidget() override = default;

    void setArtist(const QString& artist);
    void setAlbum(const QString& album);
    void setTag(const QString& tag);
    void setTrackNrOffset(int offset);
    void setTrackNr(const QString& nr);
    /**
     * Set the track name used in the file name.
     *
     * @param name the new track name as string.
     */
    void setTrackName(const QString& name);
    void setEncodedFormat(const QString& format);

    [[nodiscard]] QString getArtist() const;
    [[nodiscard]] QString getAlbum() const;
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

    [[nodiscard]] xAudioFile* getAudioFile() const;

    void viewOutput(bool autofill=false);
    /**
     * Switch to the track name view of the widget.
     *
     * @param autofill set a generic name if true.
     */
    void viewInput();
    /**
     * Switch to the progress view of the widget.
     *
     * @param progress the progress in percent.
     */
    void ripProgress(int progress);

private slots:
    void updateEncodedFileName();
    void toggleViews();


private:
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
     * Destructor
     */
    ~xEncodingTracksWidget() override = default;
    /**
     * Set all track names using the generic name "track".
     */
    void viewInput();

    void viewOutput(bool autofill=false);
    /**
     * Set all track names.
     *
     * @param names the new track names as vector of strings.
     */
    void setTracks(const QVector<xAudioFile*>& files);

    void setEncodedFormat(const QString& format);

    [[nodiscard]] QList<xEncodingTrackItemWidget*> getSelected();

    /**
     * Clear the widget. Remove all tracks.
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
    int audioTracksOffset;
    QWidget* audioMain;
    QVBoxLayout* audioLayout;
    QVector<xEncodingTrackItemWidget*> encodingTracks;
};

#endif
