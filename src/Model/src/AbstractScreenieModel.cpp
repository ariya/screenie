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

#include <QtCore/QPointF>
#include <QtGui/QPixmap>

#include "../../Utils/src/Settings.h"
#include "DefaultScreenieModel.h"

#include "AbstractScreenieModel.h"

class AbstractScreenieModelPrivate
{
public:
    AbstractScreenieModelPrivate()
        : position(QPointF(0.0, 0.0)),
          distance(DefaultScreenieModel::Distance),
          rotation(DefaultScreenieModel::Rotation),
          reflectionEnabled(DefaultScreenieModel::ReflectionEnabled),
          reflectionOffset(DefaultScreenieModel::ReflectionOffset),
          reflectionOpacity(DefaultScreenieModel::ReflectionOpacity) {}

    QPointF position;
    int distance;
    int rotation;
    int reflectionEnabled;
    int reflectionOffset;
    int reflectionOpacity;    
};

// public

AbstractScreenieModel::AbstractScreenieModel()
    : d(new AbstractScreenieModelPrivate())
{
}

AbstractScreenieModel::~AbstractScreenieModel()
{
    delete d;
#ifdef DEBUG
    qDebug("AbstractScreenieModel:~AbstractScreenieModel: called.");
#endif
}

QPointF AbstractScreenieModel::getPosition() const {
    return d->position;
}

void AbstractScreenieModel::setPosition(QPointF position)
{
    if (d->position != position) {
        d->position = position;
        emit changed();
    }
}

int AbstractScreenieModel::getDistance() const
{
    return d->distance;
}

void AbstractScreenieModel::setDistance(int distance)
{
    if (d->distance != distance && 0 <= distance && distance <= MaxDistance) {
        d->distance = distance;
        emit distanceChanged();
    }
}

void AbstractScreenieModel::addDistance(int distance)
{
    if (distance != 0) {
        int oldDistance = d->distance;
        d->distance += distance;
        if (d->distance < 0) {
            d->distance = 0;
        } else if (d->distance > MaxDistance) {
            d->distance = MaxDistance;
        }
        if (d->distance != oldDistance) {
            emit distanceChanged();
        }
    }
}

int AbstractScreenieModel::getRotation() const
{
    return d->rotation;
}

void AbstractScreenieModel::setRotation(int rotation) {
    if (d->rotation != rotation) {
        d->rotation = rotation;
        emit changed();
    }
}

void AbstractScreenieModel::rotate(int angle) {
    if (angle != 0) {
        d->rotation = (d->rotation + angle) % 360;
        while (d->rotation < 0) {
            d->rotation += 360;
        }
        emit changed();
    }
}

bool AbstractScreenieModel::isReflectionEnabled() const
{
    return d->reflectionEnabled;
}

void AbstractScreenieModel::setReflectionEnabled(bool enable)
{
    if (d->reflectionEnabled != enable) {
        d->reflectionEnabled = enable;
        emit reflectionChanged();
    }
}

int AbstractScreenieModel::getReflectionOffset() const
{
    return d->reflectionOffset;
}

void AbstractScreenieModel::setReflectionOffset(int reflectionOffset)
{
    if (d->reflectionOffset != reflectionOffset) {
        d->reflectionOffset = reflectionOffset;
        emit reflectionChanged();
    }
}

int AbstractScreenieModel::getReflectionOpacity() const
{
    return d->reflectionOpacity;
}

void AbstractScreenieModel::setReflectionOpacity(int reflectionOpacity)
{
    if (d->reflectionOpacity != reflectionOpacity) {
        d->reflectionOpacity = reflectionOpacity;
        emit reflectionChanged();
    }
}

void AbstractScreenieModel::convert(ScreenieModelInterface &source)
{
    d->position = source.getPosition();
    d->distance = source.getDistance();
    d->rotation = source.getRotation();
    d->reflectionEnabled = source.isReflectionEnabled();
    d->reflectionOffset = source.getReflectionOffset();
    d->reflectionOpacity = source.getReflectionOpacity();
}

// protected

QPixmap AbstractScreenieModel::fitToMaximumSize(QPixmap pixmap)
{
    QPixmap result;
    QSize maximumImageSize = Settings::getInstance().getMaximumImageSize();
    QSize actualSize = pixmap.size();
    if (actualSize.width() > maximumImageSize.width() || actualSize.height() > maximumImageSize.height()) {
        result = pixmap.scaled(maximumImageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        result = pixmap;
    }
    return result;
}
