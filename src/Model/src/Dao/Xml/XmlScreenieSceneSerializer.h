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

#ifndef XMLSCREENIESCENESERIALIZER_H
#define XMLSCREENIESCENESERIALIZER_H

#include "../../ModelLib.h"
#include "../ScreenieSceneSerializer.h"

class XmlScreenieSceneSerializerPrivate;

class XmlScreenieSceneSerializer : public ScreenieSceneSerializer
{
public:
    MODEL_API XmlScreenieSceneSerializer();
    MODEL_API virtual ~XmlScreenieSceneSerializer();

    MODEL_API virtual QByteArray serialize(const ScreenieScene &screenieScene, Mode mode);
    MODEL_API virtual ScreenieScene *deserialize(QByteArray &data) const;

private:
    XmlScreenieSceneSerializerPrivate *d;
};

#endif // XMLSCREENIESCENESERIALIZER_H
