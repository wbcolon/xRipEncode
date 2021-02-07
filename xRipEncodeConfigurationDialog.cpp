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
    configurationLayout->setSpacing(0);
    auto configurationTab = new QTabWidget(this);
    configurationTab->setTabPosition(QTabWidget::South);
    configurationTab->setFocusPolicy(Qt::NoFocus);
    // Create file configuration box.
    auto fileTab = new QGroupBox(tr("File Configuration"), configurationTab);
    fileTab->setFlat(xRipEncodeUseFlatGroupBox);
    auto fileTempDirectoryLabel = new QLabel(tr("Temp Directory"), fileTab);
    fileTempDirectoryLabel->setAlignment(Qt::AlignLeft);
    fileTempDirectoryInput = new QLineEdit(fileTab);
    auto fileTempDirectoryButton = new QPushButton("...", fileTab);
    auto fileBackupDirectoryLabel = new QLabel(tr("Backup Directory"), fileTab);
    fileBackupDirectoryLabel->setAlignment(Qt::AlignLeft);
    fileBackupDirectoryInput = new QLineEdit(fileTab);
    auto fileBackupDirectoryButton = new QPushButton("...", fileTab);
    auto fileEncodingDirectoryLabel = new QLabel(tr("Encoding Directory"), fileTab);
    fileEncodingDirectoryLabel->setAlignment(Qt::AlignLeft);
    fileEncodingDirectoryInput = new QLineEdit(fileTab);
    auto fileEncodingDirectoryButton = new QPushButton("...", fileTab);
    // Layout for file configuration.
    auto fileLayout = new QGridLayout();
    fileLayout->addWidget(fileTempDirectoryLabel, 0, 0, 1, 4);
    fileLayout->addWidget(fileTempDirectoryInput, 1, 0, 1, 3);
    fileLayout->addWidget(fileTempDirectoryButton, 1, 3, 1, 1);
    fileLayout->addWidget(fileBackupDirectoryLabel, 2, 0, 1, 4);
    fileLayout->addWidget(fileBackupDirectoryInput, 3, 0, 1, 3);
    fileLayout->addWidget(fileBackupDirectoryButton, 3, 3, 1, 1);
    fileLayout->addWidget(fileEncodingDirectoryLabel, 4, 0, 1, 4);
    fileLayout->addWidget(fileEncodingDirectoryInput, 5, 0, 1, 3);
    fileLayout->addWidget(fileEncodingDirectoryButton, 5, 3, 1, 1);
    fileLayout->setRowMinimumHeight(6, 0);
    fileLayout->setRowStretch(6, 2);
    fileTab->setLayout(fileLayout);
    // Required Programs programs.
    auto programsTab = new QGroupBox(tr("Progam Configuration"), configurationTab);
    programsTab->setFlat(xRipEncodeUseFlatGroupBox);
    auto fileFFMpegLabel = new QLabel(tr("ffmpeg"), programsTab);
    fileFFMpegLabel->setAlignment(Qt::AlignLeft);
    fileFFMpegInput = new QLineEdit(programsTab);
    auto fileFFMpegButton = new QPushButton("...", programsTab);
    auto fileFFProbeLabel = new QLabel(tr("ffprobe"), programsTab);
    fileFFProbeLabel->setAlignment(Qt::AlignLeft);
    fileFFProbeInput = new QLineEdit(programsTab);
    auto fileFFProbeButton = new QPushButton("...", programsTab);
    auto fileMKVMergeLabel = new QLabel(tr("mkvmerge"), programsTab);
    fileMKVMergeLabel->setAlignment(Qt::AlignLeft);
    fileMKVMergeInput = new QLineEdit(programsTab);
    auto fileMKVMergeButton = new QPushButton("...", programsTab);
    auto fileMKVExtractLabel = new QLabel(tr("mkvextract"), programsTab);
    fileMKVExtractLabel->setAlignment(Qt::AlignLeft);
    fileMKVExtractInput = new QLineEdit(programsTab);
    auto fileMKVExtractButton = new QPushButton("...", programsTab);
    auto fileFlacLabel = new QLabel(tr("flac"), programsTab);
    fileFlacLabel->setAlignment(Qt::AlignLeft);
    fileFlacInput = new QLineEdit(programsTab);
    auto fileFlacButton = new QPushButton("...", programsTab);
    auto fileWavPackLabel = new QLabel(tr("wavpack"), programsTab);
    fileWavPackLabel->setAlignment(Qt::AlignLeft);
    fileWavPackInput = new QLineEdit(programsTab);
    auto fileWavPackButton = new QPushButton("...", programsTab);
    auto fileLLTagLabel = new QLabel(tr("lltag"), programsTab);
    fileLLTagLabel->setAlignment(Qt::AlignLeft);
    fileLLTagInput = new QLineEdit(programsTab);
    auto fileLLTagButton = new QPushButton("...", programsTab);
    // Layout for programs configuration.
    auto programsLayout = new QGridLayout();
    programsLayout->addWidget(fileFFMpegLabel, 0, 0, 1, 4);
    programsLayout->addWidget(fileFFMpegInput, 1, 0, 1, 3);
    programsLayout->addWidget(fileFFMpegButton, 1, 3, 1, 1);
    programsLayout->addWidget(fileFFProbeLabel, 2, 0, 1, 4);
    programsLayout->addWidget(fileFFProbeInput, 3, 0, 1, 3);
    programsLayout->addWidget(fileFFProbeButton, 3, 3, 1, 1);
    programsLayout->addWidget(fileMKVMergeLabel, 4, 0, 1, 4);
    programsLayout->addWidget(fileMKVMergeInput, 5, 0, 1, 3);
    programsLayout->addWidget(fileMKVMergeButton, 5, 3, 1, 1);
    programsLayout->addWidget(fileMKVExtractLabel, 6, 0, 1, 4);
    programsLayout->addWidget(fileMKVExtractInput, 7, 0, 1, 3);
    programsLayout->addWidget(fileMKVExtractButton, 7, 3, 1, 1);
    programsLayout->addWidget(fileFlacLabel, 8, 0, 1, 4);
    programsLayout->addWidget(fileFlacInput, 9, 0, 1, 3);
    programsLayout->addWidget(fileFlacButton, 9, 3, 1, 1);
    programsLayout->addWidget(fileWavPackLabel, 10, 0, 1, 4);
    programsLayout->addWidget(fileWavPackInput, 11, 0, 1, 3);
    programsLayout->addWidget(fileWavPackButton, 11, 3, 1, 1);
    programsLayout->addWidget(fileLLTagLabel, 12, 0, 1, 4);
    programsLayout->addWidget(fileLLTagInput, 13, 0, 1, 3);
    programsLayout->addWidget(fileLLTagButton, 13, 3, 1, 1);
    programsLayout->setRowMinimumHeight(14, 0);
    programsLayout->setRowStretch(14, 2);
    programsTab->setLayout(programsLayout);
    // Create format configuration tab.
    auto formatTab = new QGroupBox(tr("Format Configuration"), configurationTab);
    formatTab->setFlat(xRipEncodeUseFlatGroupBox);
    auto formatEncodingFormatLabel = new QLabel(tr("Encoding Format"), formatTab);
    formatEncodingFormatLabel->setAlignment(Qt::AlignLeft);
    formatEncodingFormatInput = new QLineEdit(formatTab);
    auto formatFileNameFormatLabel = new QLabel(tr("File Format"), formatTab);
    formatFileNameFormatLabel->setAlignment(Qt::AlignLeft);
    formatFileNameFormatInput = new QLineEdit(formatTab);
    formatFileNameLowerCase = new QCheckBox(tr("Lowercase"), formatTab);
    // Layout for format configuration box.
    auto formatLayout = new QGridLayout();
    formatLayout->addWidget(formatEncodingFormatLabel, 0, 0, 1, 4);
    formatLayout->addWidget(formatEncodingFormatInput, 1, 0, 1, 4);
    formatLayout->addWidget(formatFileNameFormatLabel, 2, 0, 1, 4);
    formatLayout->addWidget(formatFileNameFormatInput, 3, 0, 1, 4);
    formatLayout->setRowMinimumHeight(4, 24);
    formatLayout->setRowStretch(4, 0);
    formatLayout->addWidget(formatFileNameLowerCase, 5, 0, 1, 4);
    formatLayout->setRowMinimumHeight(6, 0);
    formatLayout->setRowStretch(6, 2);
    formatTab->setLayout(formatLayout);
    // Create replace configuration box
    auto replaceTab = new QGroupBox(tr("Replace Configuration"), configurationTab);
    replaceTab->setFlat(xRipEncodeUseFlatGroupBox);
    auto replaceFromLabel = new QLabel(tr("From"), replaceTab);
    replaceFromLabel->setAlignment(Qt::AlignLeft);
    auto replaceToLabel = new QLabel(tr("To"), replaceTab);
    replaceToLabel->setAlignment(Qt::AlignLeft);
    replaceFromInput = new QLineEdit(replaceTab);
    replaceToInput = new QLineEdit(replaceTab);
    replaceList = new xReplaceWidget(replaceTab);
    auto replaceButtons = new QDialogButtonBox(Qt::Horizontal, replaceTab);
    replaceButtons->addButton(QDialogButtonBox::Apply);
    replaceButtons->addButton(QDialogButtonBox::Discard);
    // Layout for replace configuration box.
    auto replaceLayout = new QGridLayout();
    replaceLayout->addWidget(replaceFromLabel, 0, 0, 1, 2);
    replaceLayout->addWidget(replaceToLabel, 0, 2, 1, 2);
    replaceLayout->addWidget(replaceFromInput, 1, 0, 1, 2);
    replaceLayout->addWidget(replaceToInput, 1, 2, 1, 2);
    replaceLayout->addWidget(replaceList, 2, 0, 6, 4);
    replaceLayout->setRowStretch(2, 2);
    replaceLayout->setRowMinimumHeight(8, 32);
    replaceLayout->setRowStretch(8, 0);
    replaceLayout->addWidget(replaceButtons, 9, 0, 1, 4);
    replaceTab->setLayout(replaceLayout);
    // Add tabs
    configurationTab->addTab(fileTab, tr("File Configuration"));
    configurationTab->addTab(programsTab, tr("Programs Configuration"));
    configurationTab->addTab(formatTab, tr("Format Configuration"));
    configurationTab->addTab(replaceTab, tr("Replace Configuration"));
    // Buttons for configuration.
    auto configurationButtons = new QDialogButtonBox(Qt::Horizontal, this);
    configurationButtons->addButton(QDialogButtonBox::Save);
    configurationButtons->addButton(QDialogButtonBox::Reset);
    configurationButtons->addButton(QDialogButtonBox::Cancel);
    // Layout for configuration
    configurationLayout->addWidget(configurationTab, 0, 0, 4, 4);
    configurationLayout->setRowMinimumHeight(4, 32);
    configurationLayout->setRowStretch(4, 0);
    configurationLayout->addWidget(configurationButtons, 5, 0, 1, 4);
    // Connect file configuration buttons.
    connect(fileTempDirectoryButton, &QPushButton::pressed, [=]() { openDirectory(tr("Open Temp Directory"), fileTempDirectoryInput); });
    connect(fileEncodingDirectoryButton, &QPushButton::pressed, [=]() { openDirectory(tr("Open Encoding Directory"), fileEncodingDirectoryInput); });
    connect(fileBackupDirectoryButton, &QPushButton::pressed, [=]() { openDirectory(tr("Open Backup Directory"), fileBackupDirectoryInput); });
    connect(fileFFMpegButton, &QPushButton::pressed, [=]() { openFile(tr("Open ffmpeg Binary"), fileFFMpegInput); });
    connect(fileFFProbeButton, &QPushButton::pressed, [=]() { openFile(tr("Open ffprobe Binary"), fileFFProbeInput); });
    connect(fileMKVMergeButton, &QPushButton::pressed, [=]() { openFile(tr("Open mkvmerge Binary"), fileMKVMergeInput); });
    connect(fileMKVExtractButton, &QPushButton::pressed, [=]() { openFile(tr("Open mkvextract Binary"), fileMKVExtractInput); });
    connect(fileFlacButton, &QPushButton::pressed, [=]() { openFile(tr("Open flac Binary"), fileFlacInput); });
    connect(fileWavPackButton, &QPushButton::pressed, [=]() { openFile(tr("Open wavpack Binary"), fileWavPackInput); });
    connect(fileLLTagButton, &QPushButton::pressed, [=]() { openFile(tr("Open lltag Binary"), fileLLTagInput); });
    // Connect movie library.
    connect(replaceButtons->button(QDialogButtonBox::Apply), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::replaceEntryAdd);
    connect(replaceButtons->button(QDialogButtonBox::Discard), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::replaceEntryRemove);
    connect(replaceList, &QListWidget::currentItemChanged, this, &xRipEncodeConfigurationDialog::selectReplaceEntry);
    // Connect dialog buttons.
    connect(configurationButtons->button(QDialogButtonBox::Save), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::saveSettings);
    connect(configurationButtons->button(QDialogButtonBox::Reset), &QPushButton::pressed, this, &xRipEncodeConfigurationDialog::loadSettings);
    connect(configurationButtons->button(QDialogButtonBox::Cancel), &QPushButton::pressed, this, &QDialog::reject);
    // Load and resize.
    loadSettings();
    setMinimumWidth(static_cast<int>(sizeHint().height()*1.6));
    setMinimumHeight(sizeHint().height());
}

void xRipEncodeConfigurationDialog::loadSettings() {
    // Load settings.
    fileTempDirectoryInput->setText(xRipEncodeConfiguration::configuration()->getTempDirectory());
    fileBackupDirectoryInput->setText(xRipEncodeConfiguration::configuration()->getBackupDirectory());
    fileEncodingDirectoryInput->setText(xRipEncodeConfiguration::configuration()->getEncodingDirectory());
    fileFFMpegInput->setText(xRipEncodeConfiguration::configuration()->getFFMpeg());
    fileFFProbeInput->setText(xRipEncodeConfiguration::configuration()->getFFProbe());
    fileMKVMergeInput->setText(xRipEncodeConfiguration::configuration()->getMKVMerge());
    fileMKVExtractInput->setText(xRipEncodeConfiguration::configuration()->getMKVExtract());
    fileFlacInput->setText(xRipEncodeConfiguration::configuration()->getFlac());
    fileWavPackInput->setText(xRipEncodeConfiguration::configuration()->getWavPack());
    fileLLTagInput->setText(xRipEncodeConfiguration::configuration()->getLLTag());
    formatEncodingFormatInput->setText(xRipEncodeConfiguration::configuration()->getEncodingFormat());
    formatFileNameFormatInput->setText(xRipEncodeConfiguration::configuration()->getFileNameFormat());
    formatFileNameLowerCase->setChecked(xRipEncodeConfiguration::configuration()->getFileNameLowerCase());
    replaceList->clear();
    auto replace = xRipEncodeConfiguration::configuration()->getFileNameReplace();
    for (const auto& replaceEntry : replace) {
        replaceList->addReplace(replaceEntry.first, replaceEntry.second);
    }
}

void xRipEncodeConfigurationDialog::saveSettings() {
    // Read setting entries.
    xRipEncodeConfiguration::configuration()->setTempDirectory(fileTempDirectoryInput->text());
    xRipEncodeConfiguration::configuration()->setBackupDirectory(fileBackupDirectoryInput->text());
    xRipEncodeConfiguration::configuration()->setEncodingDirectory(fileEncodingDirectoryInput->text());
    xRipEncodeConfiguration::configuration()->setFFMpeg(fileFFMpegInput->text());
    xRipEncodeConfiguration::configuration()->setFFProbe(fileFFProbeInput->text());
    xRipEncodeConfiguration::configuration()->setMKVMerge(fileMKVMergeInput->text());
    xRipEncodeConfiguration::configuration()->setMKVExtract(fileMKVExtractInput->text());
    xRipEncodeConfiguration::configuration()->setFlac(fileFlacInput->text());
    xRipEncodeConfiguration::configuration()->setWavPack(fileWavPackInput->text());
    xRipEncodeConfiguration::configuration()->setLLTag(fileLLTagInput->text());
    xRipEncodeConfiguration::configuration()->setEncodingFormat(formatEncodingFormatInput->text());
    xRipEncodeConfiguration::configuration()->setFileNameFormat(formatFileNameFormatInput->text());
    xRipEncodeConfiguration::configuration()->setFileNameLowerCase(formatFileNameLowerCase->isChecked());
    QList<std::pair<QString,QString>> replace;
    for (auto index = 0; index < replaceList->count(); ++index) {
        auto replaceWidget = dynamic_cast<xReplaceItemWidget*>(replaceList->itemWidget(replaceList->item(index)));
        replace.push_back(std::make_pair(replaceWidget->getReplaceFrom(), replaceWidget->getReplaceTo()));
    }
    xRipEncodeConfiguration::configuration()->setFileNameReplace(replace);
    // End dialog.
    accept();
}

void xRipEncodeConfigurationDialog::selectReplaceEntry(QListWidgetItem* item) {
    if (item) {
        auto replaceWidget = dynamic_cast<xReplaceItemWidget*>(replaceList->itemWidget(item));
        replaceFromInput->setText(replaceWidget->getReplaceFrom());
        replaceToInput->setText(replaceWidget->getReplaceTo());
    } else {
        replaceFromInput->clear();
        replaceToInput->clear();
    }
}

void xRipEncodeConfigurationDialog::replaceEntryAdd() {
    auto addReplaceFrom = replaceFromInput->text();
    auto addReplaceTo = replaceToInput->text();
    if (!addReplaceFrom.isEmpty()) {
        replaceList->addReplace(addReplaceFrom, addReplaceTo);
    }
}

void xRipEncodeConfigurationDialog::replaceEntryRemove() {
    auto currentIndex = replaceList->currentRow();
    if ((currentIndex >= 0) && (currentIndex < replaceList->count())) {
        replaceList->takeItem(currentIndex);
    }
}

void xRipEncodeConfigurationDialog::openDirectory(const QString& title, QLineEdit* lineEdit) {
    QString newDirectory =
            QFileDialog::getExistingDirectory(this, title, lineEdit->text(),
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (!newDirectory.isEmpty()) {
        lineEdit->setText(newDirectory);
    }
}

void xRipEncodeConfigurationDialog::openFile(const QString& title, QLineEdit* lineEdit) {
    QString newFileName = QFileDialog::getOpenFileName(this, title, QDir::cleanPath(lineEdit->text()));
    if (!newFileName.isEmpty()) {
        lineEdit->setText(newFileName);
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
