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

#include "../../Utils/src/SizeFitter.h"
#include "../../Utils/src/PaintTools.h"
#include "ScreenieTemplateModel.h"

class ScreenieTemplateModelPrivate
{
public:
    ScreenieTemplateModelPrivate(const QSize &theSize)
        : sizeFitter(theSize) {
        sizeFitter.setFitMode(SizeFitter::Fit);
        sizeFitter.setFitOptionEnabled(SizeFitter::Enlarge, true);
    }

    ScreenieTemplateModelPrivate(const ScreenieTemplateModelPrivate &other)
        : pixmap(other.pixmap),
          sizeFitter(other.sizeFitter) {}
    QPixmap pixmap;
    SizeFitter sizeFitter;
};

// public

ScreenieTemplateModel::ScreenieTemplateModel(const QSize &size)
    : d(new ScreenieTemplateModelPrivate(size))
{
}

ScreenieTemplateModel::ScreenieTemplateModel(const ScreenieTemplateModel &other)
    : AbstractScreenieModel(other),
      d(new ScreenieTemplateModelPrivate(*other.d))
{
}

ScreenieTemplateModel::~ScreenieTemplateModel()
{
    delete d;
}

const QPixmap &ScreenieTemplateModel::readPixmap() const
{
    if (d->pixmap.isNull()) {
        d->pixmap = PaintTools::createTemplateImage(getSize());
    }
    return d->pixmap;
}

ScreenieModelInterface *ScreenieTemplateModel::copy() const
{
    ScreenieTemplateModel *result = new ScreenieTemplateModel(*this);
    return result;
}

const SizeFitter &ScreenieTemplateModel::getSizeFitter() const
{
    return d->sizeFitter;
}



QSize ScreenieTemplateModel::getSize() const
{
    return d->sizeFitter.getTargetSize();
}

// private

void ScreenieTemplateModel::frenchConnection()
{
    connect(&d->sizeFitter, SIGNAL(changed()),
            this, SIGNAL(changed()));
}

