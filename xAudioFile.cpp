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

#include "xAudioFile.h"

xAudioFile::xAudioFile(const QString& fileName, int audioTrackNr, const QString& artist, const QString& album,
                       const QString& trackNr, const QString& trackName, const QString& tag, int tagId, QObject* parent):
        QObject(parent),
        inputFileName(fileName),
        inputAudioTrackNr(audioTrackNr),
        encodingArtist(artist),
        encodingAlbum(album),
        encodingTrackNr(trackNr),
        encodingTrackName(trackName),
        encodingTag(tag),
        encodingTagId(tagId) {
}

void xAudioFile::backupWavPack() {

}

void xAudioFile::encodeFlac() {

}

QString xAudioFile::getFileName() const {
    return inputFileName;
}

int xAudioFile::getAudioTrackNr() const {
    return inputAudioTrackNr;
}

QString xAudioFile::getArtist() const {
    return encodingArtist;
}

QString xAudioFile::getAlbum() const {
    return encodingAlbum;
}

QString xAudioFile::getTrackNr() const {
    return encodingTrackNr;
}

QString xAudioFile::getTrackName() const {
    return encodingTrackName;
}

QString xAudioFile::getTag() const {
    return encodingTag;
}

int xAudioFile::getTagId() const {
    return encodingTagId;
}

