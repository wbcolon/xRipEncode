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

#ifndef __XMAINAUDIORIPPERWIDGET_H__
#define __XMAINAUDIORIPPERWIDGET_H__

#include "xAudioRipper.h"
#include <QListWidget>
#include <QWidget>

class xMainAudioRipperWidget:public QWidget {
    Q_OBJECT

public:
    xMainAudioRipperWidget(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~xMainAudioRipperWidget() = default;

private slots:
    void detectAudioCD();
    void ejectAudioCD();

private:
    QListWidget* audioTracksList;
    xAudioRipper* audioRipper;
};

#endif
