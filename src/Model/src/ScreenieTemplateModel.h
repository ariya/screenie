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

#ifndef SCREENIETEMPLATEMODEL_H
#define SCREENIETEMPLATEMODEL_H

class QSize;

#include "AbstractScreenieModel.h"
#include "ModelLib.h"

class ScreenieTemplateModelPrivate;

/*!
 * The template model acts as a placeholder for the actual pixmap to
 * be added to the ScreenieScene.
 *
 * Implementation note: we need do export the whole class here, since
 * we also need to export the QObject::staticMetaObject methods from
 * the QObject base class.
 */
class MODEL_API ScreenieTemplateModel : public AbstractScreenieModel
{
public:
    ScreenieTemplateModel(const QSize &size);
    virtual ~ScreenieTemplateModel();

    virtual QPixmap readPixmap() const;

    /*!
     * Returns the requested \c size.
     *
     * \sa ScreenieTemplateModel(const QSize &)
     */
    virtual QSize getSize() const;

    /*!
     * \return always \c true
     */
    virtual bool isValid() const;

private:
    ScreenieTemplateModelPrivate *d;
};

#endif // SCREENIETEMPLATEMODEL_H
