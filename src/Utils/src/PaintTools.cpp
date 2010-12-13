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

#include <QtGui/QPixmap>
#include <QtGui/QLinearGradient>
#include <QtGui/QPainter>

#include "PaintTools.h"



QPixmap PaintTools::createDefaultImage()
{
    QPixmap result = QPixmap(400, 300);

    QLinearGradient gradient(QPoint(0, 0), QPoint(0, result.height()));
    gradient.setColorAt(0, QColor(192, 192, 192));
    gradient.setColorAt(1, QColor(128, 128, 128));
    QPainter painter(&result);
    painter.fillRect(result.rect(), gradient);
    painter.setPen(QPen(QColor(96, 96, 96), 3, Qt::SolidLine));
    painter.drawRect(result.rect());
    QFont font;
    font.setPixelSize(100);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(result.rect(), Qt::AlignCenter, "?");
    painter.end();

    return result;
}

QPixmap PaintTools::upperHalf(const QPixmap &pixmap)
{
    return pixmap.copy(0, 0, pixmap.width(), pixmap.height() / 2);
}


