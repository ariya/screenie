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

#include <QtCore/QString>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QXmlStreamAttributes>

#include "../../ScreenieFilePathModel.h"
#include "XmlScreenieFilePathModelDao.h"

class XmlScreenieFilePathModelDaoPrivate
{
public:
    XmlScreenieFilePathModelDaoPrivate(QXmlStreamWriter &xmlStreamWriter)
        : streamWriter(xmlStreamWriter)
    {
    }

    QXmlStreamWriter &streamWriter;
};

//  public

XmlScreenieFilePathModelDao::XmlScreenieFilePathModelDao(QXmlStreamWriter &streamWriter)
{
    d = new XmlScreenieFilePathModelDaoPrivate(streamWriter);
}

XmlScreenieFilePathModelDao::~XmlScreenieFilePathModelDao(){
    delete d;
}

bool XmlScreenieFilePathModelDao::write(const ScreenieFilePathModel &screenieFilePathModel)
{
    bool result = true;
    d->streamWriter.writeStartElement("item");
    {
        QXmlStreamAttributes itemAttributes;
        itemAttributes.append("path", screenieFilePathModel.getFilePath());
        d->streamWriter.writeAttributes(itemAttributes);

        d->streamWriter.writeStartElement("position");
        {
            QXmlStreamAttributes positionAttributes;
            QPointF position = screenieFilePathModel.getPosition();
            positionAttributes.append("x", QString::number(position.x()));
            positionAttributes.append("y", QString::number(position.y()));
            positionAttributes.append("dist", QString::number(screenieFilePathModel.getDistance()));
            positionAttributes.append("rot", QString::number(screenieFilePathModel.getRotation()));
            d->streamWriter.writeAttributes(positionAttributes);
        }
        d->streamWriter.writeEndElement();

        d->streamWriter.writeStartElement("reflection");
        {
            QXmlStreamAttributes reflectionAttributes;
            reflectionAttributes.append("enabled", screenieFilePathModel.isReflectionEnabled() ? "true" : "false");
            reflectionAttributes.append("offset", QString::number(screenieFilePathModel.getReflectionOffset()));
            reflectionAttributes.append("opacity", QString::number(screenieFilePathModel.getReflectionOpacity()));
            d->streamWriter.writeAttributes(reflectionAttributes);
        }
        d->streamWriter.writeEndElement();
    }
    d->streamWriter.writeEndElement();
    return result;
}

ScreenieFilePathModel *XmlScreenieFilePathModelDao::read()
{
    ScreenieFilePathModel *result = 0;
    return result;
}
