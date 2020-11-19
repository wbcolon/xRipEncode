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
 */

#include "xAudioRipper.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

xAudioRipper::xAudioRipper(QObject* parent):
        QObject(parent),
        cdromDrive(nullptr) {
}

xAudioRipper::~xAudioRipper() {
    if (cdromDrive) {
        cdda_close(cdromDrive);
    }
}

bool xAudioRipper::detect() {
    // Close and retry detect.
    if (cdromDrive) {
        cdda_close(cdromDrive);
    }
    auto cdromDrives = cdio_get_devices_with_cap(nullptr, CDIO_FS_AUDIO, false);
    if ((cdromDrives) && (*cdromDrives)) {
        // Take the first drive.
        cdromDrive = cdda_identify(*cdromDrives, 1, nullptr);
        cdio_free_device_list(cdromDrives);
        if (cdromDrive) {
            // We'll set for verbose paranoia messages.
            cdda_verbose_set(cdromDrive, CDDA_MESSAGE_PRINTIT, CDDA_MESSAGE_PRINTIT);
            if (cdda_open(cdromDrive)) {
                qCritical() << "Unable to open drive.";
                return false;
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

void xAudioRipper::eject() {
    if (cdromDrive) {
        if (cdio_eject_media(&cdromDrive->p_cdio)) {
            qCritical() << "Unable to eject disc.";
            return;
        }
        cdda_close(cdromDrive);
        cdromDrive = nullptr;
    }
}

void xAudioRipper::close() {
    if (cdromDrive) {
        cdda_close(cdromDrive);
        cdromDrive = nullptr;
    }
}

int xAudioRipper::getTracks() {
    if (cdromDrive) {
        return static_cast<int>(cdio_cddap_tracks(cdromDrive));
    } else {
        // No Audio CD or not yet detected.
        return -1;
    }
}

void xAudioRipper::rip(const QList<std::pair<int, QString>>& tracks) {
    // Init paranoia.
    cdrom_paranoia_t* cdromDriveParanoia = paranoia_init(cdromDrive);
    for (const auto& track : tracks) {
        if ((track.first <= 0) || (track.first > cdromDrive->tracks)) {
            qInfo() << "Illegal track number: " << track.first << ". Ignore and continue.";
            continue;
        }
        lsn_t iFirstLsn = cdda_track_firstsector(cdromDrive, track.first);
        lsn_t iLastLsn = cdda_track_lastsector(cdromDrive, track.first);
        // Set reading mode for full paranoia, but allow skipping sectors.
        paranoia_modeset(cdromDriveParanoia, PARANOIA_MODE_FULL^PARANOIA_MODE_NEVERSKIP);
        paranoia_seek(cdromDriveParanoia, iFirstLsn, SEEK_SET);
        // Compute byte count. We need the size for the wav header.
        int byteCount = (iLastLsn-iFirstLsn+1) * CDIO_CD_FRAMESIZE_RAW;
        // Create wave file.
        QFile wavFile(track.second);
        wavFile.open(QIODevice::WriteOnly);
        QDataStream wavFileStream(&wavFile);
        wavFileStream.setByteOrder(QDataStream::LittleEndian);
        // Write the wav header.
        writeWaveHeader(wavFileStream, byteCount);
        // Read sectors.
        for (auto i = iFirstLsn; i <= iLastLsn; ++i) {
            // Read a sector
            std::int16_t* readBuffer = paranoia_read(cdromDriveParanoia, nullptr);
            char* cddaErrors = cdda_errors(cdromDrive);
            char* cddaMessages = cdda_messages(cdromDrive);
            if (cddaErrors) {
                qCritical() << "CDDA Errors: " << cddaErrors;
                emit ripError(track.first, QString(cddaErrors));
                free(cddaErrors);
            }
            if (cddaMessages) {
                qInfo() << "CDDA Messages: " << cddaMessages;
                emit ripMessages(track.first, QString(cddaMessages));
                free(cddaMessages);
            }
            if (!readBuffer) {
                // Notify UI about the error.
                emit ripError(track.first, tr("Stopped due to a paranoia reading error"));
                // Remove the corresponding wav file.
                wavFile.remove();
                break;
            }
            emit ripProgress(track.first, ((i-iFirstLsn)*100)/(iLastLsn-iFirstLsn));
            wavFileStream.writeRawData((char* )readBuffer, CDIO_CD_FRAMESIZE_RAW);
        }
    }
    // Free paranoia and close drive.
    paranoia_free(cdromDriveParanoia);
}

void xAudioRipper::writeWaveHeader(QDataStream& dataStream, qint32 byteCount) {
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

