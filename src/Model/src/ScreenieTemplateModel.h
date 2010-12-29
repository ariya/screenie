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

class SizeFitter;

class ScreenieTemplateModelPrivate;

/*!
 * The template model acts as a placeholder for the actual pixmap to
 * be added to the ScreenieScene. The templates have an \em order
 * which defines the order in which they are replaced by actual image
 * data. Ordering starts at 0 (highest order) and grows up to n-1,
 * where n is the number of templates in the ScreenieScene. By default
 * ScreenieTemplateModel has an \em order of \c Unordered.
 *
 * Implementation note: we need do export the whole class here, since
 * we also need to export the QObject::staticMetaObject methods from
 * the QObject base class.
 */
class MODEL_API ScreenieTemplateModel : public AbstractScreenieModel
{
    Q_OBJECT
public:
    /*!
     * No \em order has yet been defined.
     *
     * \sa #getOrder
     */
    static const int Unordered;

    explicit ScreenieTemplateModel(const QSize &size);
    explicit ScreenieTemplateModel(const ScreenieTemplateModel &other);
    virtual ~ScreenieTemplateModel();

    virtual const QPixmap &readPixmap() const;
    virtual ScreenieModelInterface *copy() const;
    virtual bool isTemplate() const;

    const SizeFitter &getSizeFitter() const;

    /*!
     * Returns the requested size.
     *
     * \sa ScreenieTemplateModel(const QSize &)
     */
    virtual QSize getSize() const;

    /*!
     * \return the order of this ScreenieTemplateModel.
     */
    int getOrder() const;
    void setOrder(int order);

private:
    ScreenieTemplateModelPrivate *d;

    void frenchConnection();
};

#endif // SCREENIETEMPLATEMODEL_H
