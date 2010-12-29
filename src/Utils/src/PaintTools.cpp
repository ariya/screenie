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

#include <QtCore/QSize>
#include <QtGui/QPixmap>
#include <QtGui/QLinearGradient>
#include <QtGui/QPainter>
#include <QtGui/QBrush>

#include "PaintTools.h"

// public

QPixmap PaintTools::createDefaultImage()
{
    QPixmap result = QPixmap(400, 300);
    QPainter painter(&result);
    drawBackground(painter, result);
    drawText("?", painter, result);
    return result;
}

QPixmap PaintTools::createTemplateImage(const QSize &size)
{
    QPixmap result = QPixmap(size);
    QPainter painter(&result);
    drawBackground(painter, result);
    drawText("T", painter, result);
    return result;
}

QPixmap PaintTools::upperHalf(const QPixmap &pixmap)
{
    return pixmap.copy(0, 0, pixmap.width(), pixmap.height() / 2);
}

QBrush PaintTools::createCheckerPattern()
{
    QBrush result;

    QImage checker(16, 16, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&checker);
    // Inspired by The GIMP ;)
    painter.fillRect(checker.rect(), QColor(153, 153, 153));
    painter.fillRect(0, 0, 8, 8, QColor(102, 102, 102));
    painter.fillRect(8, 8, 8, 8, QColor(102, 102, 102));
    painter.end();
    result.setTextureImage(checker);

    return result;
}

// private

void PaintTools::drawBackground(QPainter &painter, QPixmap &pixmap)
{
    QLinearGradient gradient(QPoint(0, 0), QPoint(0, pixmap.height()));
    gradient.setColorAt(0, QColor(192, 192, 192));
    gradient.setColorAt(1, QColor(128, 128, 128));
    painter.fillRect(pixmap.rect(), gradient);
    painter.setPen(QPen(QColor(96, 96, 96), 3, Qt::SolidLine));
    painter.drawRect(pixmap.rect());
}

void PaintTools::drawText(const QString &text, QPainter &painter, QPixmap &pixmap)
{
    QFont font;
    font.setPixelSize(100);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, text);
    painter.end();
}


