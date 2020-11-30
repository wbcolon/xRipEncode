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

#ifndef __XMAINENCODINGWIDGET_H__
#define __XMAINENCODINGWIDGET_H__

#include "xAudioFile.h"
#include "xEncodingTracksWidget.h"
#include <QListWidget>
#include <QWidget>

class xMainEncodingWidget:public QWidget {
    Q_OBJECT

public:
    explicit xMainEncodingWidget(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~xMainEncodingWidget() override;

public slots:
    void audioFiles(QList<xAudioFile*> files);

private:
    QVector<xAudioFile*> encodingAudioFiles;
    xEncodingTracksWidget* encodingTracksWidget;

};

#endif
