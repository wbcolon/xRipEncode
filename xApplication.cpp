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
#include "xApplication.h"
#include "xRipEncodeConfigurationDialog.h"
#include "xRipEncodeConfiguration.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QInputDialog>

xApplication::xApplication(QWidget* parent, Qt::WindowFlags flags):
        QMainWindow(parent, flags) {

    audioCDWidget = new xMainAudioCDWidget(this);
    // Set central widget
    setCentralWidget(audioCDWidget);
    // Create Menu
    createMenus();
}

xApplication::~xApplication() noexcept {
}

void xApplication::configure() {
    xRipEncodeConfigurationDialog configurationDialog;
    configurationDialog.show();
    configurationDialog.exec();
}

void xApplication::createMenus() {
    // Create actions for file menu.
    auto fileMenuConfigure = new QAction("&Configure", this);
    auto fileMenuExitAction = new QAction(tr("&Exit"), this);
    // Connect actions from file menu.
    connect(fileMenuConfigure, &QAction::triggered, this, &xApplication::configure);
    connect(fileMenuExitAction, &QAction::triggered, this, &xApplication::close);
    // Create file menu.
    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(fileMenuConfigure);
    fileMenu->addSeparator();
    fileMenu->addAction(fileMenuExitAction);
}

