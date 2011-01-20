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

#include <limits>

#include <QtCore/QSize>
#include <QtCore/QString>

#include "../../Utils/src/SizeFitter.h"
#include "../../Utils/src/Settings.h"
#include "../../Utils/src/PaintTools.h"
#include "ScreenieTemplateModel.h"

class ScreenieTemplateModelPrivate
{
public:
    ScreenieTemplateModelPrivate(const QSize &theSize, int theOrder)
        : sizeFitter(theSize),
          order(theOrder)
    {
          sizeFitter.setFitMode(SizeFitter::Fit);
          sizeFitter.setFitOptionEnabled(SizeFitter::Enlarge, true);
          sizeFitter.setMaxTargetSize(Settings::getInstance().getMaximumImageSize());
          sizeFitter.setFitOptionEnabled(SizeFitter::RespectMaxTargetSize, true);
    }

    ScreenieTemplateModelPrivate(const ScreenieTemplateModelPrivate &other)
        : image(other.image),
          sizeFitter(other.sizeFitter),
          order(other.order)
    {}

    QImage image;
    SizeFitter sizeFitter;
    int order;
};

const int ScreenieTemplateModel::Unordered = std::numeric_limits<int>::max();

// public

ScreenieTemplateModel::ScreenieTemplateModel()
    : d(new ScreenieTemplateModelPrivate(QSize(640, 640), ScreenieTemplateModel::Unordered))
{
    frenchConnection();
}

ScreenieTemplateModel::ScreenieTemplateModel(const QSize &size)
    : d(new ScreenieTemplateModelPrivate(size, ScreenieTemplateModel::Unordered))
{
    frenchConnection();
}

ScreenieTemplateModel::ScreenieTemplateModel(const ScreenieTemplateModel &other)
    : AbstractScreenieModel(other),
      d(new ScreenieTemplateModelPrivate(*other.d))
{
    frenchConnection();
}

ScreenieTemplateModel::~ScreenieTemplateModel()
{
#ifdef DEBUG
    qDebug("ScreenieTemplateModel:~ScreenieTemplateModel: called.");
#endif
    delete d;
}

const QImage &ScreenieTemplateModel::readImage() const
{
    if (d->image.isNull()) {
        d->image = PaintTools::createTemplateImage(getSize());
    }
    return d->image;
}

QSize ScreenieTemplateModel::getSize() const
{
    return d->sizeFitter.getTargetSize();
}

void ScreenieTemplateModel::convert(ScreenieModelInterface &source)
{
    AbstractScreenieModel::convert(source);
    d->sizeFitter.setTargetSize(source.getSize());
    d->image = QImage();
}

ScreenieModelInterface *ScreenieTemplateModel::copy() const
{
    ScreenieTemplateModel *result = new ScreenieTemplateModel(*this);
    return result;
}

bool ScreenieTemplateModel::isTemplate() const
{
    return true;
}

QString ScreenieTemplateModel::getOverlayText() const
{
    QString result = tr("ID: %1").arg(d->order);
    QString fitMode;
    switch (d->sizeFitter.getFitMode()) {
    case SizeFitter::NoFit:
        fitMode = tr("No Fit", "Size Fitter fit mode option");
        break;
    case SizeFitter::Fit:
        fitMode = tr("Fit", "Size Fitter fit mode option");
        break;
    case SizeFitter::FitToWidth:
        fitMode = tr("Fit To Width", "Size Fitter fit mode option");
        break;
    case SizeFitter::FitToHeight:
        fitMode = tr("Fit To Height", "Size Fitter fit mode option");
        break;
    case SizeFitter::ExactFit:
        fitMode = tr("Exact Fit", "Size Fitter fit mode option");
        break;
    default:
        break;
    }
    result.append(QString("\n") + tr("Fit Mode: %1").arg(fitMode));
    return result;
}

SizeFitter &ScreenieTemplateModel::getSizeFitter() const
{
    return d->sizeFitter;
}

int ScreenieTemplateModel::getOrder() const
{
    return d->order;
}

void ScreenieTemplateModel::setOrder(int order)
{
    if (d->order != order) {
        d->order = order;
        emit changed();
    }
}

// private

void ScreenieTemplateModel::frenchConnection()
{
    connect(&d->sizeFitter, SIGNAL(changed()),
            this, SLOT(handleSizeFitterChanged()));
}

// private slots

void ScreenieTemplateModel::handleSizeFitterChanged()
{
    if (d->image.size() != d->sizeFitter.getTargetSize()) {
        d->image = QImage();
        emit imageChanged(readImage());
    }
    emit changed();
}

