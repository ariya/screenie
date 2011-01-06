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

#ifndef SCREENIEMODELINTERFACE_H
#define SCREENIEMODELINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtGui/QImage>

#include "ModelLib.h"

class QString;

class ScreenieModelInterface : public QObject
{
    Q_OBJECT

public:

    MODEL_API static const qreal MaxDistance;

    virtual ~ScreenieModelInterface() {}

    /*!
     * Returns the QImage.
     *
     * \sa #setFilePath(const QString &)
     */
    virtual const QImage &readImage() const = 0;

    /*!
     * Returns the size of the image.
     *
     * Implementation note: if #readImage has not yet been called the image is currently
     * read from disk first, so it might be potentially expensive, but only for the first
     * time (the image is stored in memory).
     *
     * \return the QSize of the image
     * \sa #readImage()
     */
    virtual QSize getSize() const = 0;

    virtual QPointF getPosition() const = 0;
    /*!
     * \sa #positionChanged()
     */
    virtual void setPosition(QPointF position) = 0;

    /*!
     * \sa #positionChanged()
     */
    virtual void translate(qreal dx, qreal dy) = 0;

    virtual qreal getDistance() const = 0;
    virtual void setDistance(qreal distance) = 0;
    virtual void addDistance(qreal distance) = 0;

    virtual int getRotation() const = 0;

    /*!
     * Rotates this instance to \p angle degrees, that is the rotation is set to the absolute value \c angle.
     *
     * \param angle
     *        absolute angle to which this ScreenieModel is to be rotated in degrees; [0, 360]
     */
    virtual void setRotation(int angle) = 0;

    /*!
     * Rotates this instance by \p angle degrees, that is \p angle is added to the existing rotation.
     *
     * \param angle
     *        relative angle by which this ScreenieModel is to be rotated in degrees
     */
    virtual void rotate(int angle) = 0;

    virtual bool isReflectionEnabled() const = 0;
    virtual void setReflectionEnabled(bool enable) = 0;

    virtual int getReflectionOffset() const = 0;

    /*!
     * \param reflectionOffset
     *        the reflection offset in percent [1, 100]
     */
    virtual void setReflectionOffset(int reflectionOffset) = 0;

    /*!
     * \sa #reflectionChanged()
     */
    virtual void addReflectionOffset(int reflectionOffset) = 0;

    virtual int getReflectionOpacity() const = 0;
    /*!
     * \sa #reflectionChanged()
     */
    virtual void setReflectionOpacity(int reflectionOpacity) = 0;
    virtual void addReflectionOpacity(int reflectionOpacity) = 0;

    virtual void convert(ScreenieModelInterface &source) = 0;

    /*!
     * Creates a copy of this instance and all its associated data. The caller
     * is the owner.
     *
     * \return a copy of this instance; must be \c deleted by the caller
     */
    virtual ScreenieModelInterface *copy() const = 0;

    /*!
     * \sa #selectionChanged()
     */
    virtual void setSelected(bool enable) = 0;
    virtual bool isSelected() const = 0;

    virtual bool isTemplate() const = 0;

signals:
    void reflectionChanged();
    void distanceChanged();
    void positionChanged();
    void changed();
    void imageChanged(const QImage &image);
    void filePathChanged(const QString &filePath);
    void selectionChanged();
};

#endif // SCREENIEMODELINTERFACE_H
