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

#ifndef PAINTTOOLS_H
#define PAINTTOOLS_H

#include <QtGui/QPixmap>
#include <QtGui/QBrush>

class QPainter;
class QString;
class QSize;

#include "UtilsLib.h"

/*!
 * Helper paint functions.
 */
class PaintTools
{
public:
    /*!
     * Creates a stub image with a big '?' in the center.
     */
    UTILS_API static QPixmap createDefaultImage();

    /*!
     * Creates a template image.
     */
    UTILS_API static QPixmap createTemplateImage(const QSize &size);

    /*!
     * Returns the upper half of the \p pixmap.
     *
     * \return A QPixmap with a copy of the upper half area of the \p pixmap
     */
    UTILS_API static QPixmap upperHalf(const QPixmap &pixmap);

    /*!
     * Creates a 16x16 checker pattern.
     *
     * \return the QBrush containing the checker pattern
     */
    UTILS_API static QBrush createCheckerPattern();

private:
   static void drawBackground(QPainter &painter, QPixmap &pixmap);
   static void drawText(const QString &text, QPainter &painter, QPixmap &pixmap);
};

#endif // PAINTTOOLS_H
