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
#include "ScreenieFilePathModel.h"

class ScreenieFilePathModelPrivate
{
public:
    ScreenieFilePathModelPrivate()
        : valid(false) {}

    QString filePath;
    bool valid;
    QSize size;    
};

ScreenieFilePathModel::ScreenieFilePathModel(const QString &filePath)
    : d(new ScreenieFilePathModelPrivate())
{
    d->filePath = filePath;
}

ScreenieFilePathModel::~ScreenieFilePathModel()
{
    delete d;
#ifdef DEBUG
    qDebug("ScreenieFilePathModel:~ScreenieFilePathModel: called.");
#endif
}

QPixmap ScreenieFilePathModel::readPixmap() const
{
    QPixmap result;

    d->valid = result.load(d->filePath);
    if (d->valid) {
        result = fitToMaximumSize(result);
    } else {
        result = PaintTools::createDefaultImage();
    }
    d->size = result.size();

    return result;
}

QSize ScreenieFilePathModel::getSize() const
{
    return d->size;
}

bool ScreenieFilePathModel::isValid() const
{
    return d->valid;
}

void ScreenieFilePathModel::convert(ScreenieModelInterface &source)
{
    AbstractScreenieModel::convert(source);
}

void ScreenieFilePathModel::setFilePath(const QString &filePath)
{
    if (d->filePath != filePath) {
        d->filePath = filePath;
        emit filePathChanged(filePath);
    }
}

QString ScreenieFilePathModel::getFilePath() const
{
    return d->filePath;
}



