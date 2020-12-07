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

#include "xReplaceWidget.h"
#include <QDebug>

xReplaceItemWidget::xReplaceItemWidget(const QString& from, const QString& to, QWidget *parent, Qt::WindowFlags flags):
        QWidget(parent, flags),
        replaceFrom(from),
        replaceTo(to) {
    auto replaceLayout = new QGridLayout(this);
    auto replaceFromLabel = new QLabel(tr("From"), this);
    replaceFromLabel->setAlignment(Qt::AlignCenter);
    auto replaceFromInput = new QLineEdit(this);
    replaceFromInput->setText(replaceFrom);
    replaceFromInput->setReadOnly(true);
    auto replaceToLabel = new QLabel(tr("To"), this);
    replaceToLabel->setAlignment(Qt::AlignCenter);
    auto replaceToInput = new QLineEdit(this);
    replaceToInput->setText(replaceTo);
    replaceToInput->setReadOnly(true);
    replaceLayout->addWidget(replaceFromLabel, 0, 0);
    replaceLayout->addWidget(replaceFromInput, 0, 1, 1, 2);
    replaceLayout->addWidget(replaceToLabel, 0, 4);
    replaceLayout->addWidget(replaceToInput, 0, 5, 1, 2);
    replaceLayout->setColumnMinimumWidth(7, 20);
    replaceLayout->setColumnStretch(7, 0);
    setLayout(replaceLayout);
    // Ensure the height is fixed.
    setFixedHeight(sizeHint().height());
}

QString xReplaceItemWidget::getReplaceFrom() const {
    return replaceFrom;
}

QString xReplaceItemWidget::getReplaceTo() const {
    return replaceTo;
}

xReplaceWidget::xReplaceWidget(QWidget* parent):QListWidget(parent) {
}


void xReplaceWidget::addReplace(const QString& from, const QString& to) {
    auto item = new QListWidgetItem(this);
    auto replace = new xReplaceItemWidget(from, to, this);
    // Set a light grey as background color.
    item->setBackgroundColor(QColor(0xe0, 0xe0, 0xe0));
    setItemWidget(item, replace);
    item->setSizeHint(replace->sizeHint());
    addItem(item);
}
