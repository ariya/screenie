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

#include "../../AbstractScreenieModel.h"
#include "AbstractXmlScreenieModelDao.h"

class AbstractXmlScreenieModelDaoPrivate
{
public:
    AbstractXmlScreenieModelDaoPrivate(QXmlStreamWriter &xmlStreamWriter)
        : streamWriter(xmlStreamWriter)
    {
    }

    QXmlStreamWriter &streamWriter;
};

// public

AbstractXmlScreenieModelDao::AbstractXmlScreenieModelDao(QXmlStreamWriter &xmlStreamWriter)
{
    d = new AbstractXmlScreenieModelDaoPrivate(xmlStreamWriter);
}

AbstractXmlScreenieModelDao::~AbstractXmlScreenieModelDao()
{
    delete d;
}

bool AbstractXmlScreenieModelDao::writeCommon(const AbstractScreenieModel &screenieModel)
{
    bool result = true;
    d->streamWriter.writeStartElement("item");
    {
        writeSpecific();

        d->streamWriter.writeStartElement("position");
        {
            QXmlStreamAttributes positionAttributes;
            QPointF position = screenieModel.getPosition();
            positionAttributes.append("x", QString::number(position.x()));
            positionAttributes.append("y", QString::number(position.y()));
            positionAttributes.append("dist", QString::number(screenieModel.getDistance()));
            positionAttributes.append("rot", QString::number(screenieModel.getRotation()));
            d->streamWriter.writeAttributes(positionAttributes);
        }
        d->streamWriter.writeEndElement();

        d->streamWriter.writeStartElement("reflection");
        {
            QXmlStreamAttributes reflectionAttributes;
            reflectionAttributes.append("enabled", screenieModel.isReflectionEnabled() ? "true" : "false");
            reflectionAttributes.append("offset", QString::number(screenieModel.getReflectionOffset()));
            reflectionAttributes.append("opacity", QString::number(screenieModel.getReflectionOpacity()));
            d->streamWriter.writeAttributes(reflectionAttributes);
        }
        d->streamWriter.writeEndElement();
    }
    d->streamWriter.writeEndElement();
    return result;
}

bool AbstractXmlScreenieModelDao::readCommon(AbstractScreenieModel &abstractScreenieModel)
{
    bool result = false;
    return result;
}

// protected

QXmlStreamWriter &AbstractXmlScreenieModelDao::getStreamWriter() const
{
    return d->streamWriter;
}

