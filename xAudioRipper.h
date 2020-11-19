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

#ifndef __XAUDIORIPPER_H__
#define __XAUDIORIPPER_H__

#include <QObject>
#include <QDataStream>
#include <cdio/paranoia/paranoia.h>
#include <cdio/cd_types.h>
#include <cdio/device.h>

class xAudioRipper:public QObject {
    Q_OBJECT

public:
    xAudioRipper(QObject* parent=nullptr);
    ~xAudioRipper();

    bool detect();
    void eject();
    void close();
    int getTracks();
    void rip(const QList<std::pair<int, QString>>& tracks);

signals:
    void ripProgress(int track, int percent);
    void ripError(int track, const QString& error);
    void ripMessages(int track, const QString& message);

private:
    void writeWaveHeader(QDataStream& dataStream, int byteCount);

    cdrom_drive_t* cdromDrive;
};

#endif
