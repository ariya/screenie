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
#include <QtGui/QImage>
#include <QtGui/QLinearGradient>
#include <QtGui/QPainter>

#include "Reflection.h"

class ReflectionPrivate
{
public:
    ReflectionPrivate()
        : lastOffset(-1) {}
    QImage mask;
    int lastOffset;
};

// public

Reflection::Reflection()
{
    d = new ReflectionPrivate();
}

Reflection::~Reflection() {
    delete d;
}

QPixmap Reflection::addReflection(const QPixmap &pixmap, int opacityPercent, int offsetPercent)
{
    QPixmap result(pixmap.width(), pixmap.height() * 2);
    result.fill(Qt::transparent);

    QPainter painter(&result);
    painter.drawPixmap(0, 0, pixmap);
    painter.setOpacity(qMin(1.0, opacityPercent / 100.0));
    painter.drawPixmap(0, pixmap.height(), reflect(pixmap, offsetPercent));
    painter.end();

    return result;
}

// private

QPixmap Reflection::reflect(const QPixmap &pixmap, int offset)
{
    QLinearGradient gradient(QPoint(0, 0), QPoint(0, pixmap.height()));
    if (offset <= 0.0) {
        // make sure the black offset is never 0.0, but at least 1% (arbitrarily small);
        // otherwise the entire "gradient" would be white
        offset = 1;
    }
    gradient.setColorAt(qMin(1.0, offset / 100.0), Qt::black);
    gradient.setColorAt(0.0, Qt::white);

    bool recreate;
    if (pixmap.size() != d->mask.size()) {
        /*! \todo What is the fastest format here to draw a simple linear gradient (without alpha)
                  and apply it to another image (with alpha) as an alpha mask later on? */
        d->mask = QImage(pixmap.size(), QImage::Format_ARGB32_Premultiplied);
        recreate = true;
    } else {
        recreate = false;
    }

    // repaint gradient IF either pixmap size or offset (since last time) have changed
    if (recreate || d->lastOffset != offset) {
        QPainter painter(&d->mask);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(d->mask.rect(), gradient);
        painter.end();
        d->lastOffset = offset;
    }

    QImage reflection = pixmap.toImage().mirrored();
    reflection.setAlphaChannel(d->mask);

    return QPixmap::fromImage(reflection);
}


