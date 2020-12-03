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

#include <QMenuBar>
#include <QFileDialog>
#include <QMetaType>

xApplication::xApplication(QWidget* parent, Qt::WindowFlags flags):
        QMainWindow(parent, flags) {

    mainView = new QStackedWidget(this);
    audioCDWidget = new xMainAudioCDWidget(mainView);
    movieFileWidget = new xMainMovieFileWidget(mainView);
    encodingWidget = new xMainEncodingWidget(mainView);
    mainView->addWidget(audioCDWidget);
    mainView->addWidget(movieFileWidget);
    mainView->addWidget(encodingWidget);
    mainView->setCurrentWidget(audioCDWidget);
    // Register Type
    qRegisterMetaType<xAudioFile>();
    qRegisterMetaType<xAudioFile*>();
    qRegisterMetaType<QList<xAudioFile*>>();
    // Connections.
    connect(movieFileWidget, &xMainMovieFileWidget::audioFiles, encodingWidget, &xMainEncodingWidget::audioFiles);
    connect(audioCDWidget, &xMainAudioCDWidget::audioFiles, encodingWidget, &xMainEncodingWidget::audioFiles);
    // Set central widget
    setCentralWidget(mainView);
    // Create Menu
    createMenus();
}

xApplication::~xApplication() noexcept {
}

void xApplication::configure() {
    xRipEncodeConfigurationDialog configurationDialog(this);
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
    // Create actions for view menu.
    auto viewMenuSelectAudioCD = new QAction("Select &Audio CD View", this);
    auto viewMenuSelectMovieFile = new QAction("Select &Movie File View", this);
    auto viewMenuSelectEncoding = new QAction("Select &Encoding View", this);
    // Connect actions from view menu.
    connect(viewMenuSelectAudioCD, &QAction::triggered, [=]() { mainView->setCurrentWidget(audioCDWidget); });
    connect(viewMenuSelectMovieFile, &QAction::triggered, [=]() { mainView->setCurrentWidget(movieFileWidget); });
    connect(viewMenuSelectEncoding, &QAction::triggered, [=]() { mainView->setCurrentWidget(encodingWidget); });
    // Create view menu.
    auto viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(viewMenuSelectAudioCD);
    viewMenu->addAction(viewMenuSelectMovieFile);
    viewMenu->addAction(viewMenuSelectEncoding);
}

