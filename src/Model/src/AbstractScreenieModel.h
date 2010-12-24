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

#ifndef ABSTRACTSCREENIEMODEL_H
#define ABSTRACTSCREENIEMODEL_H

#include <QtCore/QPointF>
#include <QtGui/QPixmap>

class AbstractScreenieModelPrivate;

#include "ScreenieModelInterface.h"

class AbstractScreenieModel : public ScreenieModelInterface
{
    Q_OBJECT

public:
    AbstractScreenieModel();
    /*!
     * Copy c'tor.
     */
    explicit AbstractScreenieModel(const AbstractScreenieModel &other);
    virtual ~AbstractScreenieModel();

    virtual QPointF getPosition() const;
    virtual void setPosition(QPointF position);
    virtual void translate(qreal dx, qreal dy);

    virtual int getDistance() const;
    virtual void setDistance(int distance);
    virtual void addDistance(int distance);

    virtual int getRotation() const;
    virtual void setRotation(int angle);
    virtual void rotate(int angle);

    virtual bool isReflectionEnabled() const;
    virtual void setReflectionEnabled(bool enable);

    virtual int getReflectionOffset() const;
    virtual void setReflectionOffset(int reflectionOffset);
    virtual void addReflectionOffset(int reflectionOffset);

    virtual int getReflectionOpacity() const;
    virtual void setReflectionOpacity(int reflectionOpacity);
    virtual void addReflectionOpacity(int reflectionOpacity);
    virtual void setSelected(bool enable);
    virtual bool isSelected() const;

    virtual void convert(ScreenieModelInterface &source);

protected:
    QPixmap fitToMaximumSize(const QPixmap &pixmap) const;

private:
    AbstractScreenieModelPrivate *d;
};

#endif // ABSTRACTSCREENIEMODEL_H
