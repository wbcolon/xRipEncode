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
    // Create file configuration box.
    auto fileBox = new QGroupBox(tr("File Configuration"), this);
    auto fileTempDirectoryLabel = new QLabel(tr("Temp Directory"), fileBox);
    fileTempDirectoryLabel->setAlignment(Qt::AlignLeft);
    fileTempDirectoryInput = new QLineEdit(fileBox);
    auto fileTempDirectoryButton = new QPushButton("...", fileBox);
    auto fileBackupDirectoryLabel = new QLabel(tr("Backup Directory"), fileBox);
    fileBackupDirectoryLabel->setAlignment(Qt::AlignLeft);
    fileBackupDirectoryInput = new QLineEdit(fileBox);
    auto fileBackupDirectoryButton = new QPushButton("...", fileBox);
    auto fileEncodingDirectoryLabel = new QLabel(tr("Encoding Directory"), fileBox);
    fileEncodingDirectoryLabel->setAlignment(Qt::AlignLeft);
    fileEncodingDirectoryInput = new QLineEdit(fileBox);
    auto fileEncodingDirectoryButton = new QPushButton("...", fileBox);
    auto fileFFMpegLabel = new QLabel(tr("ffmpeg"), fileBox);
    fileFFMpegLabel->setAlignment(Qt::AlignLeft);
    fileFFMpegInput = new QLineEdit(fileBox);
    auto fileFFMpegButton = new QPushButton("...", fileBox);
    auto fileFFProbeLabel = new QLabel(tr("ffprobe"), fileBox);
    fileFFProbeLabel->setAlignment(Qt::AlignLeft);
    fileFFProbeInput = new QLineEdit(fileBox);
    auto fileFFProbeButton = new QPushButton("...", fileBox);
    auto fileMKVMergeLabel = new QLabel(tr("mkvmerge"), fileBox);
    fileMKVMergeLabel->setAlignment(Qt::AlignLeft);
    fileMKVMergeInput = new QLineEdit(fileBox);
    auto fileMKVMergeButton = new QPushButton("...", fileBox);
    auto fileMKVExtractLabel = new QLabel(tr("mkvextract"), fileBox);
    fileMKVExtractLabel->setAlignment(Qt::AlignLeft);
    fileMKVExtractInput = new QLineEdit(fileBox);
    auto fileMKVExtractButton = new QPushButton("...", fileBox);
    auto fileFlacLabel = new QLabel(tr("flac"), fileBox);
    fileFlacLabel->setAlignment(Qt::AlignLeft);
    fileFlacInput = new QLineEdit(fileBox);
    auto fileFlacButton = new QPushButton("...", fileBox);
    auto fileWavPackLabel = new QLabel(tr("wavpack"), fileBox);
    fileWavPackLabel->setAlignment(Qt::AlignLeft);
    fileWavPackInput = new QLineEdit(fileBox);
    auto fileWavPackButton = new QPushButton("...", fileBox);
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
    fileLayout->setRowMinimumHeight(6, 50);
    fileLayout->setRowStretch(6, 0);
    fileLayout->addWidget(fileFFMpegLabel, 7, 0, 1, 4);
    fileLayout->addWidget(fileFFMpegInput, 8, 0, 1, 3);
    fileLayout->addWidget(fileFFMpegButton, 8, 3, 1, 1);
    fileLayout->addWidget(fileFFProbeLabel, 9, 0, 1, 4);
    fileLayout->addWidget(fileFFProbeInput, 10, 0, 1, 3);
    fileLayout->addWidget(fileFFProbeButton, 10, 3, 1, 1);
    fileLayout->addWidget(fileMKVMergeLabel, 11, 0, 1, 4);
    fileLayout->addWidget(fileMKVMergeInput, 12, 0, 1, 3);
    fileLayout->addWidget(fileMKVMergeButton, 12, 3, 1, 1);
    fileLayout->addWidget(fileMKVExtractLabel, 13, 0, 1, 4);
    fileLayout->addWidget(fileMKVExtractInput, 14, 0, 1, 3);
    fileLayout->addWidget(fileMKVExtractButton, 14, 3, 1, 1);
    fileLayout->addWidget(fileFlacLabel, 15, 0, 1, 4);
    fileLayout->addWidget(fileFlacInput, 16, 0, 1, 3);
    fileLayout->addWidget(fileFlacButton, 16, 3, 1, 1);
    fileLayout->addWidget(fileWavPackLabel, 17, 0, 1, 4);
    fileLayout->addWidget(fileWavPackInput, 18, 0, 1, 3);
    fileLayout->addWidget(fileWavPackButton, 18, 3, 1, 1);
    fileLayout->setRowMinimumHeight(19, 0);
    fileLayout->setRowStretch(19, 2);
    fileBox->setLayout(fileLayout);
    // Create format configuration box.
    auto formatBox = new QGroupBox(tr("Format Configuration"), this);
    auto formatEncodingFormatLabel = new QLabel(tr("Encoding Format"), formatBox);
    formatEncodingFormatLabel->setAlignment(Qt::AlignLeft);
    formatEncodingFormatInput = new QLineEdit(formatBox);
    auto formatFileNameFormatLabel = new QLabel(tr("File Format"), formatBox);
    formatFileNameFormatLabel->setAlignment(Qt::AlignLeft);
    formatFileNameFormatInput = new QLineEdit(formatBox);
    formatFileNameLowerCase = new QCheckBox(tr("Lowercase"), formatBox);
    // Layout for format configuration box.
    auto formatLayout = new QGridLayout();
    formatLayout->addWidget(formatEncodingFormatLabel, 0, 0, 1, 4);
    formatLayout->addWidget(formatEncodingFormatInput, 1, 0, 1, 4);
    formatLayout->addWidget(formatFileNameFormatLabel, 2, 0, 1, 4);
    formatLayout->addWidget(formatFileNameFormatInput, 3, 0, 1, 4);
    formatLayout->setRowMinimumHeight(4, 20);
    formatLayout->setRowStretch(4, 0);
    formatLayout->addWidget(formatFileNameLowerCase, 5, 0, 1, 4);
    formatLayout->setRowMinimumHeight(6, 0);
    formatLayout->setRowStretch(6, 2);
    formatBox->setLayout(formatLayout);
    // Create replace configuration box
    auto replaceBox = new QGroupBox(tr("Replace Configuration"), this);
    auto replaceFromLabel = new QLabel(tr("From"), replaceBox);
    replaceFromLabel->setAlignment(Qt::AlignLeft);
    auto replaceToLabel = new QLabel(tr("To"), replaceBox);
    replaceToLabel->setAlignment(Qt::AlignLeft);
    replaceFromInput = new QLineEdit(replaceBox);
    replaceToInput = new QLineEdit(replaceBox);
    replaceList = new xReplaceWidget(replaceBox);
    auto replaceButtons = new QDialogButtonBox(Qt::Horizontal, replaceBox);
    replaceButtons->addButton(QDialogButtonBox::Apply);
    replaceButtons->addButton(QDialogButtonBox::Discard);
    // Layout for replace configuration box.
    auto replaceLayout = new QGridLayout();
    replaceLayout->addWidget(replaceFromLabel, 0, 0, 1, 2);
    replaceLayout->addWidget(replaceToLabel, 0, 2, 1, 2);
    replaceLayout->addWidget(replaceFromInput, 1, 0, 1, 2);
    replaceLayout->addWidget(replaceToInput, 1, 2, 1, 2);
    replaceLayout->addWidget(replaceList, 2, 0, 3, 4);
    replaceLayout->addWidget(replaceButtons, 5, 0, 1, 4);
    replaceBox->setLayout(replaceLayout);
    // Buttons for configuration.
    auto configurationButtons = new QDialogButtonBox(Qt::Horizontal, this);
    configurationButtons->addButton(QDialogButtonBox::Save);
    configurationButtons->addButton(QDialogButtonBox::Reset);
    configurationButtons->addButton(QDialogButtonBox::Cancel);
    // Layout for configuration
    configurationLayout->addWidget(fileBox, 0, 0, 5, 4);
    configurationLayout->addWidget(formatBox, 0, 4, 2, 4);
    configurationLayout->addWidget(replaceBox, 2, 4, 3, 4);
    configurationLayout->setRowMinimumHeight(5, 50);
    configurationLayout->setRowStretch(5, 0);
    configurationLayout->addWidget(configurationButtons, 6, 4, 1, 4);
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
