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
#include <QtCore/QString>

#include "FileUtils.h"

// public

const QString FileUtils::SceneExtension = QString("xsc");
const QString FileUtils::TemplateExtension = QString("xst");

QString FileUtils::getOpenImageFileFilter()
{
    QString result = QObject::tr("JPEG") + " (*.jpg);;" +
                     QObject::tr("Portable Network Graphics") + " (*.png);;" +
                     QObject::tr("Windows Bitmap") + " (*.bmp);;" +
                     QObject::tr("Tagged Image File Format") + " (*.tif);;" +
                     QObject::tr("All Files") + " (*)";
    return result;
}

QString FileUtils::getSaveImageFileFilter()
{
    /*!\todo Support JPEG (and maybe TIFF/BMP) in the future as well */
    QString result = QObject::tr("Portable Network Graphics") + "(*.png)";
    return result;
}
