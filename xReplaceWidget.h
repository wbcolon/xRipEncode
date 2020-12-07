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

#ifndef __XREPLACEWIDGET_H__
#define __XREPLACEWIDGET_H__

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QListWidget>

class xReplaceItemWidget:public QWidget {
public:
    /**
     * Constructor. Item widget for replace widget.
     *
     * @param from string to be found.
     * @param to string to replace the found string with.
     * @param parent pointer to the parent widget.
     * @param flags window flags for the widget object.
     */
    xReplaceItemWidget(const QString& from, const QString& to, QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    /**
     * Destructor. Default.
     */
    ~xReplaceItemWidget() override = default;
    /**
     * Get the string that is to be found.
     *
     * @return the from string.
     */
    [[nodiscard]] QString getReplaceFrom() const;
    /**
     * Get the string that replaces the to from string.
     *
     * @return the to string.
     */
    [[nodiscard]] QString getReplaceTo() const;

private:
    QString replaceFrom;
    QString replaceTo;
};


class xReplaceWidget:public QListWidget {
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent pointer to the parent widget.
     */
    explicit xReplaceWidget(QWidget* parent=nullptr);
    /**
     * Destructor. Default.
     */
    ~xReplaceWidget() override = default;
    /**
     * Add a replace widget item to the list widget.
     *
     * @param from string to be found.
     * @param to string to replace the found string with.
     */
    void addReplace(const QString& from, const QString& to);
};

#endif
