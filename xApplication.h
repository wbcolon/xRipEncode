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
#ifndef __XAPPLICATION_H__
#define __XAPPLICATION_H__

#include "xMainAudioCDWidget.h"
#include <QSettings>
#include <QStackedWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

class xApplication:public QMainWindow {
public:
    xApplication(QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~xApplication() noexcept;

private:
    /**
     * Generate the File and View menus and connect actions.
     */
    void createMenus();

    xMainAudioCDWidget* audioCDWidget;
};

#endif