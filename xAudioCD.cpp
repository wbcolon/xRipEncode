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
/*
 * Handling of Audio CDs with libcdio was in part borrowed from the paranoia.c example.
 * https://github.com/rocky/libcdio-paranoia/blob/master/example/paranoia.c
 * MusicBrainz lookup was in part borrowed from cdlookup.cc example.
 * https://github.com/metabrainz/libmusicbrainz/blob/master/examples/cdlookup.cc
 */

#include "xAudioCD.h"
#include "xRipEncodeConfiguration.h"
#include <QFile>
#include <QDataStream>
#include <QCryptographicHash>
#include <QDebug>

#include "musicbrainz5/Query.h"
#include "musicbrainz5/Medium.h"
#include "musicbrainz5/MediumList.h"
#include "musicbrainz5/ReleaseGroup.h"
#include "musicbrainz5/Track.h"
#include "musicbrainz5/TrackList.h"
#include "musicbrainz5/Recording.h"
#include "musicbrainz5/Disc.h"
#include "musicbrainz5/HTTPFetch.h"
#include "musicbrainz5/Release.h"
#include "musicbrainz5/ArtistCredit.h"
#include "musicbrainz5/Artist.h"
#include "musicbrainz5/NameCreditList.h"
#include "musicbrainz5/NameCredit.h"


/**
 * xAudioCDLookup
 *
 * This class handles the MusicBrainz lookup in a separate thread. The for the lookup required ID
 * is computed in the xAudioCD class.
 */
xAudioCDLookup::xAudioCDLookup(const QString& id, QObject* parent):
        QThread(parent),
        musicBrainzID(id) {
}

QList<xAudioCDLookup::xAudioCDLookupResult> xAudioCDLookup::result() const {
    return musicBrainzResult;
}

void xAudioCDLookup::run() {
    // Clear musicbrainz results.
    musicBrainzResult.clear();
    // Borrowed from https://github.com/metabrainz/libmusicbrainz/blob/master/examples/cdlookup.cc
    MusicBrainz5::CQuery query("xRipEncode");
    try {
        MusicBrainz5::CMetadata metaData = query.Query("discid",musicBrainzID.toStdString());
        if ((metaData.Disc()) && (metaData.Disc()->ReleaseList())) {
            MusicBrainz5::CReleaseList *releaseList = metaData.Disc()->ReleaseList();
            qDebug() << "Found: " << releaseList->NumItems() << " release(s)";
            for (auto count = 0; count < releaseList->NumItems(); ++count) {
                xAudioCDLookupResult audioCDLookupResult;
                MusicBrainz5::CRelease* release = releaseList->Item(count);
                // The releases returned from LookupDiscID don't contain full information
                MusicBrainz5::CQuery::tParamMap params;
                params["inc"]="artists labels recordings release-groups url-rels discids artist-credits";
				std::string releaseID=release->ID();
                MusicBrainz5::CMetadata metaDataRelease=query.Query("release",releaseID,"",params);
                if (metaDataRelease.Release()) {
                    MusicBrainz5::CRelease* fullRelease = metaDataRelease.Release();
                    // However, these releases will include information for all media in the release
                    // So we need to filter out the only the media we want.
                    MusicBrainz5::CMediumList mediaList = fullRelease->MediaMatchingDiscID(musicBrainzID.toStdString());
                    if (mediaList.NumItems() != 0) {
                        if (fullRelease->ReleaseGroup()) {
                            audioCDLookupResult.album = QString::fromStdString(fullRelease->ReleaseGroup()->Title());
                            qDebug() << "Album: '" << audioCDLookupResult.album << "'";
                            if (fullRelease->ReleaseGroup()->ArtistCredit()) {
                                MusicBrainz5::CNameCreditList* nameCreditList = fullRelease->ReleaseGroup()->ArtistCredit()->NameCreditList();
                                if (nameCreditList) {
                                    MusicBrainz5::CNameCredit name = *nameCreditList->Item(0);
                                    audioCDLookupResult.artist = QString::fromStdString(name.Artist()->Name());
                                    qDebug() << "Artist: '" << audioCDLookupResult.artist << "'";
                                }
                            }
                        } else {
                            qDebug() << "No release group for this release";
                        }
                        qDebug() << "Found " << mediaList.NumItems() << " media item(s)";
                        for (auto media = 0; media < mediaList.NumItems(); ++media) {
                            MusicBrainz5::CMedium* medium=mediaList.Item(media);
                            qDebug() << "Found media: '" << QString::fromStdString(medium->Title()) << "', position " << medium->Position();
                            MusicBrainz5::CTrackList* trackList = medium->TrackList();
                            if (trackList) {
                                for (int num = 0; num < trackList->NumItems(); ++num) {
                                    MusicBrainz5::CTrack* track=trackList->Item(num);
                                    MusicBrainz5::CRecording* recording = track->Recording();
                                    QString trackName;
                                    if (recording) {
                                        trackName = QString::fromStdString(recording->Title());
                                    } else {
                                        trackName = QString::fromStdString(track->Title());
                                    }
                                    audioCDLookupResult.tracks.push_back(trackName);
                                }
                            }
                        }
                    }
                }
                qDebug() << "Result(artist): " << audioCDLookupResult.artist;
                qDebug() << "Result(album): " << audioCDLookupResult.album;
                qDebug() << "Result(tracks): " << audioCDLookupResult.tracks;
                // Check for duplicates.
                bool duplicate = false;
                for (const auto& result : musicBrainzResult) {
                    if ((result.artist == audioCDLookupResult.artist) &&
                        (result.album == audioCDLookupResult.album) &&
                        (result.tracks == audioCDLookupResult.tracks)) {
                        qDebug() << "Duplicate result found. Ignoring.";
                        duplicate = true;
                        break;
                    }
                }
                // Only add if it is not a duplicate.
                if (!duplicate) {
                    musicBrainzResult.push_back(audioCDLookupResult);
                }
            }
        }
    } catch (MusicBrainz5::CExceptionBase& error) {
        qCritical() << "MusicBrainz Exception: " << error.what();
    }
}


/**
 * xAudioCDRipper
 *
 * This class handles the rip process in a separate thread. The process is initiated
 * from the xAudioCD class. Output is written as wav files.
 */
xAudioCDRipper::xAudioCDRipper(cdrom_drive_t* drive, const QList<std::pair<int, QString>>& tracks, QObject* parent):
        QThread(parent),
        audioDrive(drive),
        audioTracks(tracks) {
}

void xAudioCDRipper::run() {
    // Retrieve current temp file.
    auto tempFileDirectory = xRipEncodeConfiguration::configuration()->getTempFileDirectory();
    // Init paranoia.
    cdrom_paranoia_t* audioDriveParanoia = paranoia_init(audioDrive);
    for (const auto& track : audioTracks) {
        if ((track.first <= 0) || (track.first > audioDrive->tracks)) {
            qInfo() << "Illegal track number: " << track.first << ". Ignore and continue.";
            continue;
        }
        lsn_t iFirstLsn = cdda_track_firstsector(audioDrive, track.first);
        lsn_t iLastLsn = cdda_track_lastsector(audioDrive, track.first);
        // Set reading mode for full paranoia, but allow skipping sectors.
        paranoia_modeset(audioDriveParanoia, PARANOIA_MODE_FULL^PARANOIA_MODE_NEVERSKIP);
        paranoia_seek(audioDriveParanoia, iFirstLsn, SEEK_SET);
        // Compute byte count. We need the size for the wav header.
        int byteCount = (iLastLsn-iFirstLsn+1) * CDIO_CD_FRAMESIZE_RAW;
        // Create wave file.
        auto wavFilePath = tempFileDirectory+"/"+track.second;
        QFile wavFile(wavFilePath);
        if (!wavFile.open(QIODevice::WriteOnly)) {
            qCritical() << "Unable to open wav file: " << wavFilePath;
            emit error(track.first, "Unable to open wav file: "+wavFilePath, true);
            continue;
        }
        QDataStream wavFileStream(&wavFile);
        wavFileStream.setByteOrder(QDataStream::LittleEndian);
        // Write the wav header.
        writeWaveHeader(wavFileStream, byteCount);
        // Read sectors.
        for (auto i = iFirstLsn; i <= iLastLsn; ++i) {
            // Read a sector
            std::int16_t* readBuffer = paranoia_read(audioDriveParanoia, nullptr);
            char* cddaErrors = cdda_errors(audioDrive);
            char* cddaMessages = cdda_messages(audioDrive);
            if (cddaErrors) {
                qCritical() << "CDDA Errors: " << cddaErrors;
                emit error(track.first, QString(cddaErrors), false);
                free(cddaErrors);
            }
            if (cddaMessages) {
                qInfo() << "CDDA Messages: " << cddaMessages;
                emit messages(track.first, QString(cddaMessages));
                free(cddaMessages);
            }
            if (!readBuffer) {
                // Notify UI about the error.
                emit error(track.first, tr("Aborted due to a paranoia reading error"), true);
                // Remove the corresponding wav file.
                wavFile.remove();
                break;
            }
            emit progress(track.first, ((i-iFirstLsn)*100)/(iLastLsn-iFirstLsn));
            wavFileStream.writeRawData((char* )readBuffer, CDIO_CD_FRAMESIZE_RAW);
        }
    }
    // Free paranoia and close drive.
    paranoia_free(audioDriveParanoia);

}

void xAudioCDRipper::writeWaveHeader(QDataStream& dataStream, qint32 byteCount) {
    dataStream.writeRawData("RIFF", 4); // Bytes 0 - 3
    dataStream << quint32(byteCount+44-8); // Bytes 4 - 7
    dataStream.writeRawData("WAVEfmt ", 8); // Bytes 8 - 15
    dataStream << quint32(16); // Bytes 16 - 19
    dataStream << quint16(1); // Bytes 20 - 21
    dataStream << quint16(2); // Bytes 22 - 23
    dataStream << quint32(44100); // Bytes 24 - 27
    dataStream << quint32(44100*2*2); // Bytes 28 - 31
    dataStream << quint16(4); // Bytes 32 - 33
    dataStream << quint16(16); // Bytes 34 - 35
    dataStream.writeRawData("data", 4); // Bytes 36 - 39
    dataStream << quint32(byteCount); // Bytes 40 - 43
}

/**
 * xAudioCD
 *
 * This class handles the operation for audio CDs.
 */
xAudioCD::xAudioCD(QObject* parent):
        QObject(parent),
        audioDrive(nullptr),
        audioRipper(nullptr) {

}

xAudioCD::~xAudioCD() {
    if (audioDrive) {
        cdda_close(audioDrive);
    }
}

bool xAudioCD::detect() {
    // Close and retry detect.
    if (audioDrive) {
        cdda_close(audioDrive);
    }
    auto audioDrives = cdio_get_devices_with_cap(nullptr, CDIO_FS_AUDIO, false);
    if ((audioDrives) && (*audioDrives)) {
        // Take the first drive.
        audioDrive = cdda_identify(*audioDrives, 1, nullptr);
        cdio_free_device_list(audioDrives);
        if (audioDrive) {
            // Log messages and errors.
            cdda_verbose_set(audioDrive, CDDA_MESSAGE_LOGIT, CDDA_MESSAGE_LOGIT);
            // We'll set for verbose paranoia messages.
            if (cdda_open(audioDrive)) {
                qCritical() << "Unable to open drive.";
                return false;
            }
            // Get messages.
            char* cddaMessages = cdda_messages(audioDrive);
            if (cddaMessages) {
                emit ripMessages(0, QString(cddaMessages));
                free(cddaMessages);
            }
            return true;
        } else {
            qCritical() << "Unable to access disc.";
            return false;
        }
    } else {
        qInfo() << "No CD-ROM drive with an audio CD available";
        return false;
    }
}

void xAudioCD::eject() {
    if (audioDrive) {
        if (cdio_eject_media(&audioDrive->p_cdio)) {
            qCritical() << "Unable to eject disc.";
            return;
        }
        cdda_close(audioDrive);
        audioDrive = nullptr;
    }
}

void xAudioCD::close() {
    if (audioDrive) {
        cdda_close(audioDrive);
        audioDrive = nullptr;
    }
}

int xAudioCD::getTracks() {
    if (audioDrive) {
        return static_cast<int>(cdio_cddap_tracks(audioDrive));
    } else {
        // No Audio CD or not yet detected.
        return -1;
    }
}

QString xAudioCD::getID() {
    if (!audioDrive) {
        return QString();
    }
    // Vector for toc data. Fill with zeros.
    QVector<uint32_t> tocData(100, 0);
    uint32_t noTracks = cdio_cddap_tracks(audioDrive);
    uint32_t noAudioTracks = noTracks;
    uint32_t firstAudioTrack = 1;
    if (noTracks >= CDIO_CD_MAX_TRACKS) {
        return QString();
    }
    // Leadout track.
    tocData[0] = cdio_cddap_track_firstsector(audioDrive, CDIO_CDROM_LEADOUT_TRACK)+CDIO_PREGAP_SECTORS;
    for (uint32_t track = 1; track <= noTracks; ++track) {
        // Do we have an audio track. Break if we do not.
        if (cdda_track_audiop(audioDrive, track) == 0) {
            tocData[0] = cdio_cddap_track_firstsector(audioDrive, CDIO_CDROM_LEADOUT_TRACK)+CDIO_PREGAP_SECTORS;
            noAudioTracks = track-1;
            break;
        }
        // Track offsets for tracks 1..99
        tocData[track] = cdio_cddap_track_firstsector(audioDrive, track)+CDIO_PREGAP_SECTORS;
    }
    qDebug() << "xAudio::getID: first: " << firstAudioTrack << ", last: " << noAudioTracks << ", toc-data: " << tocData;
#if 0
    // Examples from https://musicbrainz.org/doc/Disc_ID_Calculation
    // Result: "49HHV7Eb8UKF3aQiNmu1GR8vKTY-"
    tocData[0] = 95462;
    tocData[1] = 150;
    tocData[2] = 15363;
    tocData[3] = 32314;
    tocData[4] = 46592;
    tocData[5] = 63414;
    tocData[6] = 80489;
    for (int i = 7; i < 100; ++i) tocData[i] = 0;
    noAudioTracks = 6;
    qDebug() << "xAudio::getID: musicbrainz example: \"49HHV7Eb8UKF3aQiNmu1GR8vKTY-\"";
#endif
    // Compute ID using SHA1.
    QCryptographicHash sha1(QCryptographicHash::Sha1);
    sha1.addData(QString("%1").arg(firstAudioTrack, 2, 16, QChar('0')).toUpper().toStdString().c_str(), 2);
    sha1.addData(QString("%1").arg(noAudioTracks, 2, 16, QChar('0')).toUpper().toStdString().c_str(), 2);
    for (uint32_t track = 0; track < 100; ++track) {
        sha1.addData(QString("%1").arg(tocData[track], 8, 16, QChar('0')).toUpper().toStdString().c_str(), 8);
    }
    // Compute base64 encoded string. Replace '+','/','=' with URL friendly '.','_','-'.
    QString id = sha1.result().toBase64().replace('+','.').replace('/','_').replace('=','-');
    qDebug() << "xAudio::getID: " << id;
    return id;
}

void xAudioCD::rip(const QList<std::pair<int, QString>>& tracks) {
    if (audioRipper) {
        qInfo() << "Unable to start another rip process while current in progress.";
        return;
    }
    audioRipper = new xAudioCDRipper(audioDrive, tracks, this);
    // Forward signals.
    connect(audioRipper, &xAudioCDRipper::progress, this, &xAudioCD::ripProgress);
    connect(audioRipper, &xAudioCDRipper::messages, this, &xAudioCD::ripMessages);
    connect(audioRipper, &xAudioCDRipper::error, this, &xAudioCD::ripError);
    // Delete object
    connect(audioRipper, &xAudioCDRipper::finished, this, &xAudioCD::ripThreadFinished);
    audioRipper->start();
}

void xAudioCD::ripCancel() {
    if (audioRipper) {
        // End the thread.
        audioRipper->quit();
        // Delete and reset the object.
        delete audioRipper;
        audioRipper = nullptr;
    }
}

void xAudioCD::ripThreadFinished() {
    if (audioRipper) {
        delete audioRipper;
        audioRipper = nullptr;
        emit ripFinished();
    }
}
