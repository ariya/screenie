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

#ifndef SCREENIEIMAGEMODEL_H
#define SCREENIEIMAGEMODEL_H

#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QImage>

#include "AbstractScreenieModel.h"
#include "ModelLib.h"

class ScreeniePixmapModelPrivate;

/*!
 * Implementation note: we need do export the whole class here, since
 * we also need to export the QObject::staticMetaObject methods from
 * the QObject base class.
 */
class MODEL_API ScreenieImageModel : public AbstractScreenieModel
{
    Q_OBJECT
public:
    /*!
     * Creates this ScreenieImageModel. Call #readImage() after creation.
     *
     * \sa #readImage()
     */
    explicit ScreenieImageModel(const QImage image = QImage());

    /*!
     * Copy c'tor.
     */
    explicit ScreenieImageModel(const ScreenieImageModel &other);
    virtual ~ScreenieImageModel();

    virtual const QImage &readImage() const;
    virtual QSize getSize() const;
    virtual ScreenieModelInterface *copy() const;
    virtual bool isTemplate() const;
    virtual QString getOverlayText() const;

    QImage getImage() const;
    void setImage(QImage image);

private:
    ScreeniePixmapModelPrivate *d;
};

#endif // SCREENIEIMAGEMODEL_H
