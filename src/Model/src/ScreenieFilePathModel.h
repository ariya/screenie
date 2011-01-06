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

#ifndef SCREENIEFILEPATHMODEL_H
#define SCREENIEFILEPATHMODEL_H

#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtGui/QImage>

class QString;

#include "AbstractScreenieModel.h"
#include "ModelLib.h"

class SizeFitter;
class ScreenieFilePathModelPrivate;

/*!
 * Implementation note: we need do export the whole class here, since
 * we also need to export the QObject::staticMetaObject methods from
 * the QObject base class.
 */
class MODEL_API ScreenieFilePathModel : public AbstractScreenieModel
{
    Q_OBJECT
public:
    /*!
     * Creates this ScreenieFilePathModel. Call #readImage() after creation.
     *
     * \param filePath
     *        the file path to the image to be read
     * \param sizeFitter
     *        if given the image is scaled with the \p sizeFitter; may be 0; ownership
     *        remains with the caller
     *
     * \sa #readImage()
     */
    explicit ScreenieFilePathModel(const QString &filePath = QString(), const SizeFitter *sizeFitter = 0);
    explicit ScreenieFilePathModel(const ScreenieFilePathModel &other);
    virtual ~ScreenieFilePathModel();

    virtual const QImage &readImage() const;
    virtual QSize getSize() const;
    virtual ScreenieModelInterface *copy() const;
    virtual bool isTemplate() const;

    virtual QString getFilePath() const;
    virtual void setFilePath(const QString &filePath);

private:
    ScreenieFilePathModelPrivate *d;
};

#endif // SCREENIEFILEPATHMODEL_H
