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

#ifndef SCREENIEPIXMAPMODEL_H
#define SCREENIEPIXMAPMODEL_H

#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtGui/QPixmap>

#include "AbstractScreenieModel.h"
#include "ModelLib.h"

class ScreeniePixmapModelPrivate;

/*!
 * Implementation note: we need do export the whole class here, since
 * we also need to export the QObject::staticMetaObject methods from
 * the QObject base class.
 */
class MODEL_API ScreeniePixmapModel : public AbstractScreenieModel
{
    Q_OBJECT
public:
    /*!
     * Creates this ScreeniePixmapModel. Call #readPixmap() after creation.
     *
     * \sa #readPixmap()
     */
    explicit ScreeniePixmapModel(const QPixmap pixmap = QPixmap());

    /*!
     * Copy c'tor.
     */
    explicit ScreeniePixmapModel(const ScreeniePixmapModel &other);
    virtual ~ScreeniePixmapModel();

    virtual const QPixmap &readPixmap() const;
    virtual QSize getSize() const;
    virtual ScreenieModelInterface *copy() const;
    virtual bool isTemplate() const;

    QPixmap getPixmap() const;
    void setPixmap(QPixmap pixmap);

private:
    ScreeniePixmapModelPrivate *d;
};

#endif // SCREENIEPIXMAPMODEL_H
