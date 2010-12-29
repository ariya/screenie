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

#include <QtCore/QtAlgorithms>
#include <QtCore/QList>

#include "../../../Model/src/ScreenieModelInterface.h"
#include "ScreenieMimeData.h"

class ScreenieMimeDataPrivate
{
public:
    ScreenieMimeDataPrivate(QList<const ScreenieModelInterface *> models)
        : screenieModels(models)
    {}

    QList<const ScreenieModelInterface *> screenieModels;
};

// public

ScreenieMimeData::ScreenieMimeData()
    : QMimeData(),
      d(new ScreenieMimeDataPrivate(QList<const ScreenieModelInterface *>()))
{
}

ScreenieMimeData::ScreenieMimeData(const QList<const ScreenieModelInterface *> screenieModels)
    : QMimeData(),
      d(new ScreenieMimeDataPrivate(screenieModels))
{
}

ScreenieMimeData::~ScreenieMimeData()
{
#ifdef DEBUG
    qDebug("ScreenieMimeData::~ScreenieMimeData: called, %d items...", d->screenieModels.size());
#endif
    qDeleteAll(d->screenieModels);
    delete d;
}

const QList<const ScreenieModelInterface *> ScreenieMimeData::getScreenieModels() const
{
    return d->screenieModels;
}

bool ScreenieMimeData::hasScreenieModels() const
{
    return d->screenieModels.size() > 0;
}
