/* This file is part of the Screenie project.
   Screenie is a fancy screenshot composer.

   Copyright (C) 2008 Ariya Hidayat <ariya.hidayat@gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QtCore/QObject>

class QGraphicsScene;

#include "../KernelLib.h"

class ScreenieControl;
class ScreenieModelInterface;
class ClipboardPrivate;

/*!
 * Support for cut/copy/paste operations.
 */
class Clipboard : public QObject
{
    Q_OBJECT
public:
    KERNEL_API explicit Clipboard(ScreenieControl &screenieControl, QObject *parent = 0);
    KERNEL_API virtual ~Clipboard();

    /*!
     * \return \c true if this Clipboard has valid MIME data to paste; \c false else
     */
    KERNEL_API bool hasData() const;

signals:
    /*!
     * Connected to the underlying QClipboard::dataChanged() signal.
     */
    void dataChanged();

public slots:
    KERNEL_API void cut();
    KERNEL_API void copy();
    KERNEL_API void paste();

private:
    ClipboardPrivate *d;

    void frenchConnection();
    void storeMimeData();
};

#endif // CLIPBOARD_H
