/*
 * This file is part of xPlay.
 *
 * xPlay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * xPlay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "xRipEncodeConfigurationDialog.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QRegularExpression>
#include <QDebug>

xRipEncodeConfigurationDialog::xRipEncodeConfigurationDialog(QWidget* parent, Qt::WindowFlags flags):
        QDialog(parent, flags) {
    auto configurationLayout = new QGridLayout(this);

    auto fileBox = new QGroupBox(tr("File Configuration"), this);
    auto fileTempDirectoryLabel = new QLabel(tr("Temp Directory"), fileBox);
    fileTempDirectoryLabel->setAlignment(Qt::AlignLeft);
    fileTempDirectoryInput = new QLineEdit(fileBox);
    auto fileTempDirectoryButton = new QPushButton("...", fileBox);
    auto fileFileNameFormatLabel = new QLabel(tr("File Format"), fileBox);
    fileFileNameFormatLabel->setAlignment(Qt::AlignLeft);
    fileFileNameFormatInput = new QLineEdit(fileBox);
    fileFileNameLowerCase = new QCheckBox(tr("Lowercase"), fileBox);

    auto fileLayout = new QGridLayout();
    fileLayout->addWidget(fileTempDirectoryLabel, 0, 0, 1, 4);
    fileLayout->addWidget(fileTempDirectoryInput, 1, 0, 1, 3);
    fileLayout->addWidget(fileTempDirectoryButton, 1, 3, 1, 1);
    fileLayout->setRowMinimumHeight(2, 50);
    fileLayout->setRowStretch(2, 0);
    fileLayout->addWidget(fileFileNameFormatLabel, 3, 0, 1, 4);
    fileLayout->addWidget(fileFileNameFormatInput, 4, 0, 1, 4);
    fileLayout->setRowMinimumHeight(5, 20);
    fileLayout->setRowStretch(5, 0);
    fileLayout->addWidget(fileFileNameLowerCase, 6, 0, 1, 4);
    fileLayout->setRowMinimumHeight(7, 0);
    fileLayout->setRowStretch(7, 2);
    fileBox->setLayout(fileLayout);

    auto replaceBox = new QGroupBox(tr("Replace"), this);
    auto replaceFromLabel = new QLabel(tr("From"), replaceBox);
    replaceFromLabel->setAlignment(Qt::AlignLeft);
    auto replaceToLabel = new QLabel(tr("To"), replaceBox);
    replaceToLabel->setAlignment(Qt::AlignLeft);
    replaceFromInput = new QLineEdit(replaceBox);
    replaceToInput = new QLineEdit(replaceBox);
    replaceList = new QListWidget(replaceBox);
    auto replaceButtons = new QDialogButtonBox(Qt::Horizontal, replaceBox);
    replaceButtons->addButton(QDialogButtonBox::Apply);
    replaceButtons->addButton(QDialogButtonBox::Discard);

    auto replaceLayout = new QGridLayout();
    replaceLayout->addWidget(replaceFromLabel, 0, 0, 1, 2);
    replaceLayout->addWidget(replaceToLabel, 0, 2, 1, 2);
    replaceLayout->addWidget(replaceFromInput, 1, 0, 1, 2);
    replaceLayout->addWidget(replaceToInput, 1, 2, 1, 2);
    replaceLayout->addWidget(replaceList, 2, 0, 3, 4);
    replaceLayout->addWidget(replaceButtons, 5, 0, 1, 4);
    replaceBox->setLayout(replaceLayout);

    auto configurationButtons = new QDialogButtonBox(Qt::Horizontal, this);
    configurationButtons->addButton(QDialogButtonBox::Save);
    configurationButtons->addButton(QDialogButtonBox::Reset);
    configurationButtons->addButton(QDialogButtonBox::Cancel);

    configurationLayout->addWidget(fileBox, 0, 0, 2, 4);
    configurationLayout->addWidget(replaceBox, 0, 4, 2, 4);
    configurationLayout->setRowMinimumHeight(4, 50);
    configurationLayout->setRowStretch(4, 0);
    configurationLayout->addWidget(configurationButtons, 5, 4, 1, 4);

    // Connect movie library.
    connect(replaceButtons->button(QDialogButtonBox::Apply), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::replaceEntryAdd);
    connect(replaceButtons->button(QDialogButtonBox::Discard), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::replaceEntryRemove);
    connect(replaceList, &QListWidget::currentItemChanged, this, &xRipEncodeConfigurationDialog::selectReplaceEntry);
    // Connect dialog buttons.
    connect(configurationButtons->button(QDialogButtonBox::Save), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::saveSettings);
    connect(configurationButtons->button(QDialogButtonBox::Reset), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::loadSettings);
    connect(configurationButtons->button(QDialogButtonBox::Cancel), &QPushButton::pressed, this, &QDialog::reject);
#if 0
    auto musicLibraryBox = new QGroupBox(tr("Music Library Configuration"), this);
    auto movieLibraryBox = new QGroupBox(tr("Movie Library Configuration"), this);
    auto streamingSitesBox = new QGroupBox(tr("Streaming Sites Configuration"), this);

    // Setup movie library setup, with tags, directories and extensions.
    auto movieLibraryLayout = new QGridLayout(movieLibraryBox);
    auto movieLibraryTagLabel = new QLabel(tr("Tag"), movieLibraryBox);
    movieLibraryTagWidget = new QLineEdit(movieLibraryBox);
    auto movieLibraryDirectoryLabel = new QLabel(tr("Directory"), movieLibraryBox);
    movieLibraryDirectoryWidget = new QLineEdit(movieLibraryBox);
    auto movieLibraryDirectoryOpenButton = new QPushButton(tr("..."), movieLibraryBox);
    movieLibraryListWidget = new QListWidget(movieLibraryBox);
    movieLibraryListWidget->setSortingEnabled(true);
    auto movieLibraryExtensionsLabel = new QLabel(tr("Extensions"), movieLibraryBox);
    movieLibraryExtensionsWidget = new QLineEdit(movieLibraryBox);
    movieLibraryLayout->addWidget(movieLibraryTagLabel, 0, 0, 1, 5);
    movieLibraryLayout->addWidget(movieLibraryTagWidget, 1, 0, 1, 5);
    movieLibraryLayout->addWidget(movieLibraryDirectoryLabel, 2, 0, 1, 5);
    movieLibraryLayout->addWidget(movieLibraryDirectoryWidget, 3, 0, 1, 4);
    movieLibraryLayout->addWidget(movieLibraryDirectoryOpenButton, 3, 4);
    movieLibraryLayout->addWidget(movieLibraryListWidget, 4, 0, 3, 5);
    movieLibraryLayout->addWidget(movieLibraryButtons, 7, 0, 1, 5);
    movieLibraryLayout->addWidget(movieLibraryExtensionsLabel, 8, 0, 1, 5);
    movieLibraryLayout->addWidget(movieLibraryExtensionsWidget, 9, 0, 1, 5);
    // Setup music library with directory and extensions.
    auto musicLibraryLayout = new QGridLayout(musicLibraryBox);
    auto musicLibraryDirectoryLabel = new QLabel(tr("Directory"), musicLibraryBox);
    musicLibraryDirectoryWidget = new QLineEdit(musicLibraryBox);
    auto musicLibraryDirectoryOpenButton = new QPushButton(tr("..."), musicLibraryBox);
    auto musicLibraryExtensionsLabel = new QLabel(tr("Extensions"), musicLibraryBox);
    musicLibraryExtensionsWidget = new QLineEdit(musicLibraryBox);
    musicLibraryLayout->addWidget(musicLibraryDirectoryLabel, 0, 0, 1, 5);
    musicLibraryLayout->addWidget(musicLibraryDirectoryWidget, 1, 0, 1, 4);
    musicLibraryLayout->addWidget(musicLibraryDirectoryOpenButton, 1, 4);
    musicLibraryLayout->addWidget(musicLibraryExtensionsLabel, 2, 0, 1, 5);
    musicLibraryLayout->addWidget(musicLibraryExtensionsWidget, 3, 0, 1, 5);
    // Setup rotel amp with network address and port.
    auto rotelLayout = new QGridLayout(rotelBox);
    auto rotelNetworkAddressLabel = new QLabel(tr("Network Address"), rotelBox);
    auto rotelNetworkPortLabel = new QLabel(tr("Port"), rotelBox);
    rotelNetworkAddressWidget = new QLineEdit(rotelBox);
    rotelNetworkPortWidget = new QSpinBox(rotelBox);
    rotelNetworkPortWidget->setRange(0, 10000);
    rotelLayout->addWidget(rotelNetworkAddressLabel, 0, 0, 1, 4);
    rotelLayout->addWidget(rotelNetworkAddressWidget, 1, 0, 1, 4);
    rotelLayout->addWidget(rotelNetworkPortLabel, 0, 4);
    rotelLayout->addWidget(rotelNetworkPortWidget, 1, 4);
    // Setup streaming sites with URL and short name.
    auto streamingSitesLayout = new QGridLayout(streamingSitesBox);
    auto streamingNameLabel = new QLabel(tr("Name"), streamingSitesBox);
    streamingNameWidget = new QLineEdit(streamingSitesBox);
    auto streamingUrlLabel = new QLabel(tr("Url"), streamingSitesBox);
    streamingUrlWidget = new QLineEdit(streamingSitesBox);
    auto streamingSitesButtons = new QDialogButtonBox(Qt::Horizontal, streamingSitesBox);
    streamingSitesButtons->addButton(QDialogButtonBox::Apply);
    streamingSitesButtons->addButton(QDialogButtonBox::Discard);
    auto streamingSitesDefaultButton = streamingSitesButtons->addButton(tr("Default"), QDialogButtonBox::ResetRole);
    streamingSitesListWidget = new QListWidget(movieLibraryBox);
    streamingSitesListWidget->setSortingEnabled(true);
    streamingSitesLayout->addWidget(streamingNameLabel, 0, 0, 1, 2);
    streamingSitesLayout->addWidget(streamingNameWidget, 1, 0, 1, 2);
    streamingSitesLayout->addWidget(streamingUrlLabel, 1, 2, 1, 3);
    streamingSitesLayout->addWidget(streamingUrlWidget, 1, 2, 1, 3);
    streamingSitesLayout->addWidget(streamingSitesListWidget, 2, 0, 3, 5);
    streamingSitesLayout->addWidget(streamingSitesButtons, 5, 0, 1, 5);
    // Configuration layout.
    configurationLayout->addWidget(musicLibraryBox, 0, 0, 2, 4);
    configurationLayout->addWidget(streamingSitesBox, 2, 0, 3, 4);
    configurationLayout->addWidget(rotelBox, 5, 0, 1, 4);
    configurationLayout->addWidget(movieLibraryBox, 0, 4, 6, 4);
    configurationLayout->setRowMinimumHeight(6, 50);
    configurationLayout->setRowStretch(6, 0);
    configurationLayout->addWidget(configurationButtons, 7, 4, 1, 4);
    // Connect dialog buttons.
    connect(musicLibraryDirectoryOpenButton, &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::openMusicLibraryDirectory);
    connect(movieLibraryDirectoryOpenButton, &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::openMovieLibraryDirectory);
    // Connect movie library.
    connect(movieLibraryButtons->button(QDialogButtonBox::Apply), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::movieLibraryAdd);
    connect(movieLibraryButtons->button(QDialogButtonBox::Discard), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::movieLibraryRemove);
    connect(movieLibraryListWidget, &QListWidget::currentItemChanged, this, &xRipEncodeConfigurationDialog::selectMovieLibrary);
    // Connect streaming sites.
    connect(streamingSitesButtons->button(QDialogButtonBox::Apply), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::streamingSiteAdd);
    connect(streamingSitesButtons->button(QDialogButtonBox::Discard), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::streamingSiteRemove);
    connect(streamingSitesDefaultButton, &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::streamingSiteDefault);
    connect(streamingSitesListWidget, &QListWidget::currentItemChanged, this, &xRipEncodeConfigurationDialog::selectStreamingSite);
#endif
    // Load and resize.
    loadSettings();
    setMinimumWidth(static_cast<int>(sizeHint().height()*1.7));
    setMinimumHeight(sizeHint().height());
}

void xRipEncodeConfigurationDialog::loadSettings() {
    // Load settings.
    fileTempDirectoryInput->setText(xRipEncodeConfiguration::configuration()->getTempFileDirectory());
    fileFileNameFormatInput->setText(xRipEncodeConfiguration::configuration()->getFileNameFormat());
    fileFileNameLowerCase->setChecked(xRipEncodeConfiguration::configuration()->getFileNameLowerCase());
    replaceList->clear();
    auto replace = xRipEncodeConfiguration::configuration()->getFileNameReplace();
    for (const auto& replaceEntry : replace) {
        replaceList->addItem(QString(R"(replace "%1" with "%2")").arg(replaceEntry.first).arg(replaceEntry.second));
    }
}

void xRipEncodeConfigurationDialog::saveSettings() {
    // Read setting entries.
    xRipEncodeConfiguration::configuration()->setTempFileDirectory(fileTempDirectoryInput->text());
    xRipEncodeConfiguration::configuration()->setFileNameFormat(fileFileNameFormatInput->text());
    xRipEncodeConfiguration::configuration()->setFileNameLowerCase(fileFileNameLowerCase->isChecked());
    QList<std::pair<QString,QString>> replace;
    for (auto index = 0; index < replaceList->count(); ++index) {
        replace.push_back(xRipEncodeConfigurationDialog::splitReplaceEntries(replaceList->item(index)->text()));
    }
    xRipEncodeConfiguration::configuration()->setFileNameReplace(replace);
    // End dialog.
    accept();
}

void xRipEncodeConfigurationDialog::selectReplaceEntry(QListWidgetItem* item) {
    if (item) {
        auto [replaceFrom,replaceTo] = xRipEncodeConfigurationDialog::splitReplaceEntries(item->text());
        replaceFromInput->setText(replaceFrom);
        replaceToInput->setText(replaceTo);
    } else {
        replaceFromInput->clear();
        replaceToInput->clear();
    }
}

void xRipEncodeConfigurationDialog::replaceEntryAdd() {
    auto addReplaceFrom = replaceFromInput->text();
    auto addReplaceTo = replaceToInput->text();
    if (!addReplaceFrom.isEmpty()) {
        replaceList->addItem(QString(R"(replace "%1" with "%2")").arg(addReplaceFrom).arg(addReplaceTo));
    }
}

void xRipEncodeConfigurationDialog::replaceEntryRemove() {
    auto currentIndex = replaceList->currentRow();
    if ((currentIndex >= 0) && (currentIndex < replaceList->count())) {
        replaceList->takeItem(currentIndex);
    }
}

std::pair<QString,QString> xRipEncodeConfigurationDialog::splitReplaceEntries(const QString& entry) {
    QRegularExpression regExp(R"(replace \"(?<from>.*)\" with \"(?<to>.*)\")");
    QRegularExpressionMatch regExpMatch = regExp.match(entry);
    if (regExpMatch.hasMatch()) {
        return std::make_pair(regExpMatch.captured("from"), regExpMatch.captured("to"));
    } else {
        return std::make_pair("","");
    }
}
