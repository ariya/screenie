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
#include <QtGui/QImage>

#include "../../Utils/src/SizeFitter.h"
#include "../../Utils/src/PaintTools.h"
#include "ScreenieFilePathModel.h"

class ScreenieFilePathModelPrivate
{
public:
    ScreenieFilePathModelPrivate(const QString &theFilePath, const SizeFitter *theSizeFitter)
        : filePath(theFilePath),
          sizeFitter(theSizeFitter) {}

    ScreenieFilePathModelPrivate(const ScreenieFilePathModelPrivate &other)
        : filePath(other.filePath),
          image(other.image),
          sizeFitter(other.sizeFitter)
    {
    }

    QString filePath;
    QImage image;
    const SizeFitter *sizeFitter;
};

ScreenieFilePathModel::ScreenieFilePathModel(const QString &filePath, const SizeFitter *sizeFitter)
    : d(new ScreenieFilePathModelPrivate(filePath, sizeFitter))
{
}

ScreenieFilePathModel::ScreenieFilePathModel(const ScreenieFilePathModel &other)
    : AbstractScreenieModel(other),
      d(new ScreenieFilePathModelPrivate(*other.d))
{
}

ScreenieFilePathModel::~ScreenieFilePathModel()
{
    delete d;
#ifdef DEBUG
    qDebug("ScreenieFilePathModel:~ScreenieFilePathModel: called.");
#endif
}

const QImage &ScreenieFilePathModel::readImage() const
{
    d->image.load(d->filePath);
    if (!d->image.isNull()) {
        if (d->sizeFitter != 0) {
            QSize fittedSize;
            bool doResize = d->sizeFitter->fit(d->image.size(), fittedSize);
            if (doResize) {
                d->image = d->image.scaled(fittedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }
        } else {
            d->image = fitToMaximumSize(d->image);
        }
    } else {
        d->image = PaintTools::createDefaultImage();
    }
    return d->image;
}

QSize ScreenieFilePathModel::getSize() const
{
    QSize result;
    if (!d->image.isNull()) {
        result = d->image.size();
    } else {
        /*!\todo Optimisation: use Exiv2 library to quickly read the image size from disk (EXIF data) */
        result = readImage().size();
    }
    return result;
}

ScreenieModelInterface *ScreenieFilePathModel::copy() const
{
    ScreenieFilePathModel *result = new ScreenieFilePathModel(*this);
    return result;
}

bool ScreenieFilePathModel::isTemplate() const
{
    return false;
}

void ScreenieFilePathModel::setFilePath(const QString &filePath)
{
    if (d->filePath != filePath) {
        d->filePath = filePath;
        d->image = QImage();
        emit filePathChanged(filePath);
    }
}

QString ScreenieFilePathModel::getFilePath() const
{
    return d->filePath;
}



