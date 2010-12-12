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
#include <QtGui/QPixmap>

class QString;

#include "AbstractScreenieModel.h"

class ScreenieFilePathModelPrivate;

class ScreenieFilePathModel : public AbstractScreenieModel
{
    Q_OBJECT
public:
    /*!
     * Creates this ScreenieFilePathModel. Call #readPixmap() after creation.
     *
     * \sa #readPixmap()
     */
    explicit ScreenieFilePathModel(const QString &filePath = QString());
    virtual ~ScreenieFilePathModel();

    virtual QPixmap readPixmap();
    virtual QSize getSize() const;
    virtual bool isValid() const;

    virtual QString getFilePath() const;
    virtual void setFilePath(const QString &filePath);

    virtual bool operator=(const ScreenieModelInterface &other);

signals:
    void filePathChanged(QString filePath);

private:
    ScreenieFilePathModelPrivate *d;
};

#endif // SCREENIEFILEPATHMODEL_H
