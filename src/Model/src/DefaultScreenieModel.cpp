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

#include <QtCore/QObject>

#include "DefaultScreenieModel.h"

class DefaultScreenieModelPrivate
{
public:
    DefaultScreenieModelPrivate()
        : distance(0.0),
          rotation(0),
          reflectionEnabled(true),
          reflectionOffset(40),
          reflectionOpacity(50)
    {}
    qreal distance;
    int rotation;
    bool reflectionEnabled;
    int reflectionOffset;
    int reflectionOpacity;
};

const qreal DefaultScreenieModel::Distance = 0.0;
const int DefaultScreenieModel::Rotation = 0;
const bool DefaultScreenieModel::ReflectionEnabled = true;
const int DefaultScreenieModel::ReflectionOffset = 40;
const int DefaultScreenieModel::ReflectionOpacity = 50;

// public

DefaultScreenieModel::DefaultScreenieModel()
{
    d = new DefaultScreenieModelPrivate();
}

DefaultScreenieModel::~DefaultScreenieModel()
{
    delete d;
}

qreal DefaultScreenieModel::getDistance() const
{
    return d->distance;
}

void DefaultScreenieModel::setDistance(qreal distance)
{
    d->distance = distance;
}

int DefaultScreenieModel::getRotation() const
{
    return d->rotation;
}

void DefaultScreenieModel::setRotation(int rotation)
{
    d->rotation = rotation;
}

bool DefaultScreenieModel::isReflectionEnabled() const
{
    return d->reflectionEnabled;
}

void DefaultScreenieModel::setReflectionEnabled(bool enable)
{
    d->reflectionEnabled = enable;
}

int DefaultScreenieModel::getReflectionOffset() const
{
    return d->reflectionOffset;
}

void DefaultScreenieModel::setReflectionOffset(int reflectionOffset)
{
    d->reflectionOffset = reflectionOffset;
}

int DefaultScreenieModel::getReflectionOpacity() const {
    return d->reflectionOpacity;
}

void DefaultScreenieModel::setReflectionOpacity(int reflectionOpacity)
{
    d->reflectionOpacity = reflectionOpacity;
}

void DefaultScreenieModel::reset()
{
    d->distance = Distance;
    d->rotation = Rotation;
    d->reflectionEnabled = ReflectionEnabled;
    d->reflectionOffset = ReflectionOffset;
    d->reflectionOpacity = ReflectionOpacity;
}




