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

#include <QtCore/QByteArray>
#include <QtCore/QBuffer>

#include "../../ScreenieScene.h"
#include "XmlScreenieSceneDao.h"
#include "XmlScreenieSceneSerializer.h"

class XmlScreenieSceneSerializerPrivate
{
public:
    XmlScreenieSceneSerializerPrivate()
    {
    }

    XmlScreenieSceneDao *screenieSceneDao;
};

// public

XmlScreenieSceneSerializer::XmlScreenieSceneSerializer()
    : d(new XmlScreenieSceneSerializerPrivate())
{
}

XmlScreenieSceneSerializer::~XmlScreenieSceneSerializer(){
    delete d;
}

QByteArray XmlScreenieSceneSerializer::serialize(const ScreenieScene &screenieScene, Mode mode)
{
   QByteArray result;
   bool ok;
   QBuffer buffer(&result);
   XmlScreenieSceneDao screenieSceneDao(buffer);
   ok = screenieSceneDao.write(screenieScene, mode);
   if (!ok) {
       result.resize(0);
   }
   return result;
}

ScreenieScene *XmlScreenieSceneSerializer::deserialize(QByteArray &data) const
{
   ScreenieScene *result = 0;
   QBuffer buffer(&data);
   XmlScreenieSceneDao screenieSceneDao(buffer);
   result = screenieSceneDao.read();
   return result;
}
