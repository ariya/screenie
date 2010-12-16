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
#include <QtCore/QString>
#include <QtGui/QPixmap>

#include "../../Utils/src/PaintTools.h"
#include "ScreeniePixmapModel.h"

class ScreeniePixmapModelPrivate
{
public:
    ScreeniePixmapModelPrivate()
        : valid(false) {}

    QPixmap pixmap;
    bool valid;
    QSize size;    
};

ScreeniePixmapModel::ScreeniePixmapModel(QPixmap pixmap)
    : d(new ScreeniePixmapModelPrivate())
{
    d->pixmap = fitToMaximumSize(pixmap);
}

ScreeniePixmapModel::~ScreeniePixmapModel()
{
    delete d;
#ifdef DEBUG
    qDebug("ScreeniePixmapModel:~ScreeniePixmapModel: called.");
#endif
}

QPixmap ScreeniePixmapModel::readPixmap() const
{
    return d->pixmap;
}

QSize ScreeniePixmapModel::getSize() const
{
    return d->pixmap.size();
}

bool ScreeniePixmapModel::isValid() const
{
    return d->valid;
}

void ScreeniePixmapModel::convert(ScreenieModelInterface &source)
{
    AbstractScreenieModel::convert(source);
}

void ScreeniePixmapModel::setPixmap(QPixmap pixmap)
{
    if (d->pixmap.cacheKey() != pixmap.cacheKey()) {
        d->pixmap = pixmap;
        emit pixmapChanged(pixmap);
    }
}

QPixmap ScreeniePixmapModel::getPixmap() const
{
    return d->pixmap;
}



