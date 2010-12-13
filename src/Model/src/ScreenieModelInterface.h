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
#include <QtGui/QPixmap>

class QString;

class ScreenieModelInterface : public QObject
{
    Q_OBJECT

public:

    static const int MaxDistance;

    virtual ~ScreenieModelInterface() {}

    /*!
     * Returns the QPixmap.
     *
     * \sa #setFilePath(const QString &)
     */
    virtual QPixmap readPixmap() = 0;

    /*!
     * Returns \c true if #readPixmap() returned a valid QPixmap. Call #readPixmap() first.
     *
     * \sa #readPixmap()
     */
    virtual bool isValid() const = 0;

    /*!
     * Returns the \em original size of the image, as loaded from \p filePath. Call #readPixmap() first.
     *
     * \return the \em original QSize of the image; an \em invalid QSize if this ScreenieModel is \em invalid
     * \sa #readPixmap()
     * \sa #isValid()
     */
    virtual QSize getSize() const = 0;

    virtual QPointF getPosition() const = 0;
    virtual void setPosition(QPointF position) = 0;

    virtual int getDistance() const = 0;
    virtual void setDistance(int distance) = 0;
    virtual void addDistance(int distance) = 0;

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
    virtual void setReflectionOffset(int reflectionOffset) = 0;

    virtual int getReflectionOpacity() const = 0;
    virtual void setReflectionOpacity(int reflectionOpacity) = 0;

    virtual void convert(ScreenieModelInterface &source) = 0;

signals:
    void reflectionChanged();
    void distanceChanged();
    void changed();
    void pixmapChanged(const QPixmap &pixmap);
    void filePathChanged(const QString &filePath);
};

#endif // SCREENIEMODELINTERFACE_H
