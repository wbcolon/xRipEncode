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

#include "xMainEncodingWidget.h"

#include <QListWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>

class xEncodingItem:public QWidget {

public:
    xEncodingItem(const QString& fileName, QListWidget* parent=nullptr):QWidget(parent){
        auto itemLayout = new QGridLayout(this);
        fileNameWidget = new QLineEdit(this);
        fileNameWidget->setText(fileName);
        backupWidget = new QCheckBox("Backup", this);
        encodingWidget = new QCheckBox("Encode", this);
        itemLayout->addWidget(encodingWidget, 0, 0);
        itemLayout->addWidget(backupWidget, 0, 1);
        itemLayout->addWidget(fileNameWidget, 0, 2, 1, 10);
        setLayout(itemLayout);
        qDebug() << "SIZE_HINT: " << sizeHint();
        resize(sizeHint());
    }

private:
    QLineEdit* fileNameWidget;
    QCheckBox* backupWidget;
    QCheckBox* encodingWidget;

};


xMainEncodingWidget::xMainEncodingWidget(QWidget *parent, Qt::WindowFlags flags):
        QWidget(parent, flags) {

    auto mainLayout = new QGridLayout(this);

    auto formatBox = new QGroupBox(tr("Format"), this);
    auto formatLayout = new QGridLayout();
    auto formatEncodingFormatLabel = new QLabel(tr("Encoding Format"), formatBox);
    auto formatEncodingFormatInput = new QLineEdit(formatBox);
    auto formatFileFormatLabel = new QLabel(tr("File Format"), formatBox);
    auto formatFileFormatInput = new QLineEdit(formatBox);
    formatLayout->addWidget(formatEncodingFormatLabel, 0, 0, 1, 4);
    formatLayout->addWidget(formatEncodingFormatInput, 1, 0, 1, 4);
    formatLayout->addWidget(formatFileFormatLabel, 2, 0, 1, 4);
    formatLayout->addWidget(formatFileFormatInput, 3, 0, 1, 4);
    formatBox->setLayout(formatLayout);

    auto encodeFormatBox = new QGroupBox(tr("Encode"), this);
    auto encodeFormatLayout = new QVBoxLayout();
    auto encodeFormatEncodingButton =  new QRadioButton(tr("Use Encoding Format"), encodeFormatBox);
    auto encodeFormatFileButton =  new QRadioButton(tr("Use File Format"), encodeFormatBox);
    encodeFormatLayout->addWidget(encodeFormatEncodingButton);
    encodeFormatLayout->addWidget(encodeFormatFileButton);
    encodeFormatBox->setLayout(encodeFormatLayout);

    auto backupFormatBox = new QGroupBox(tr("Backup"), this);
    auto backupFormatLayout = new QVBoxLayout();
    auto backupFormatEncodingButton =  new QRadioButton(tr("Use Encoding Format"), backupFormatBox);
    auto backupFormatFileButton =  new QRadioButton(tr("Use File Format"), backupFormatBox);
    backupFormatLayout->addWidget(backupFormatEncodingButton);
    backupFormatLayout->addWidget(backupFormatFileButton);
    backupFormatBox->setLayout(backupFormatLayout);

    encodingTracksWidget = new xEncodingTracksWidget(this);
    mainLayout->addWidget(formatBox, 0, 0, 2, 2);
    mainLayout->addWidget(encodeFormatBox, 2, 0, 2, 2);
    mainLayout->addWidget(backupFormatBox, 4, 0, 2, 2);
    mainLayout->setRowStretch(6, 2);
    mainLayout->setColumnMinimumWidth(3, 20);
    mainLayout->setColumnStretch(3, 0);
    mainLayout->addWidget(encodingTracksWidget, 0, 4, 7, 6);


}

xMainEncodingWidget::~xMainEncodingWidget() noexcept {
    for (auto& file : encodingAudioFiles) {
        delete file;
    }
}


void xMainEncodingWidget::audioFiles(QList<xAudioFile*> files) {
    qDebug() << "AUDIO_FILE: " << files[0]->getFileName();
    encodingAudioFiles.push_back(files[0]);
}