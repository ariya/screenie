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

#include "../../Utils/src/SizeFitter.h"
#include "../../Utils/src/PaintTools.h"
#include "ScreenieFilePathModel.h"

class ScreenieFilePathModelPrivate
{
public:
    ScreenieFilePathModelPrivate(const QString &theFilePath, const SizeFitter *theSizeFitter)
        : filePath(theFilePath),
          sizeFitter(theSizeFitter) {}
    QString filePath;
    QPixmap pixmap;
    const SizeFitter *sizeFitter;
};

ScreenieFilePathModel::ScreenieFilePathModel(const QString &filePath, const SizeFitter *sizeFitter)
    : d(new ScreenieFilePathModelPrivate(filePath, sizeFitter))
{
}

ScreenieFilePathModel::~ScreenieFilePathModel()
{
    delete d;
#ifdef DEBUG
    qDebug("ScreenieFilePathModel:~ScreenieFilePathModel: called.");
#endif
}

const QPixmap &ScreenieFilePathModel::readPixmap() const
{
    d->pixmap.load(d->filePath);
    if (!d->pixmap.isNull()) {
        if (d->sizeFitter != 0) {
            QSize fittedSize;
            bool doResize = d->sizeFitter->fit(d->pixmap.size(), fittedSize);
            if (doResize) {
                d->pixmap = d->pixmap.scaled(fittedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }
        } else {
            d->pixmap = fitToMaximumSize(d->pixmap);
        }
    } else {
        d->pixmap = PaintTools::createDefaultImage();
    }
    return d->pixmap;
}

QSize ScreenieFilePathModel::getSize() const
{
    QSize result;
    if (!d->pixmap.isNull()) {
        result = d->pixmap.size();
    } else {
        /*!\todo Optimisation: use Exiv2 library to quickly read the image size from disk (EXIF data) */
        result = readPixmap().size();
    }
    return result;
}

void ScreenieFilePathModel::convert(ScreenieModelInterface &source)
{
    AbstractScreenieModel::convert(source);
}

void ScreenieFilePathModel::setFilePath(const QString &filePath)
{
    if (d->filePath != filePath) {
        d->filePath = filePath;
        d->pixmap = QPixmap();
        emit filePathChanged(filePath);
    }
}

QString ScreenieFilePathModel::getFilePath() const
{
    return d->filePath;
}



