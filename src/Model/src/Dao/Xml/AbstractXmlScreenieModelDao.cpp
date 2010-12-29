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
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamAttributes>

#include "../../AbstractScreenieModel.h"
#include "AbstractXmlScreenieModelDao.h"

class AbstractXmlScreenieModelDaoPrivate
{
public:
    AbstractXmlScreenieModelDaoPrivate(QXmlStreamWriter *xmlStreamWriter, QXmlStreamReader *xmlStreamReader)
        : streamWriter(xmlStreamWriter),
          streamReader(xmlStreamReader)
    {}

    QXmlStreamWriter *streamWriter;
    QXmlStreamReader *streamReader;
};

// public

AbstractXmlScreenieModelDao::AbstractXmlScreenieModelDao(QXmlStreamWriter *xmlStreamWriter)
    : d(new AbstractXmlScreenieModelDaoPrivate(xmlStreamWriter, 0))
{    
}

AbstractXmlScreenieModelDao::AbstractXmlScreenieModelDao(QXmlStreamReader *xmlStreamReader)
    : d(new AbstractXmlScreenieModelDaoPrivate(0, xmlStreamReader))
{
}

AbstractXmlScreenieModelDao::~AbstractXmlScreenieModelDao()
{
#ifdef DEBUG
    qDebug("AbstractXmlScreenieModelDao::~AbstractXmlScreenieModelDao: called.");
#endif
    delete d;
}

bool AbstractXmlScreenieModelDao::write(const AbstractScreenieModel &screenieModel)
{
    bool result = true;
    QXmlStreamWriter *streamWriter = getStreamWriter();

    writeSpecific();

    streamWriter->writeStartElement("position");
    {
        QXmlStreamAttributes positionAttributes;
        QPointF position = screenieModel.getPosition();
        positionAttributes.append("x", QString::number(position.x()));
        positionAttributes.append("y", QString::number(position.y()));
        positionAttributes.append("z", QString::number(screenieModel.getDistance()));
        positionAttributes.append("roty", QString::number(screenieModel.getRotation()));
        streamWriter->writeAttributes(positionAttributes);
    }
    streamWriter->writeEndElement();

    streamWriter->writeStartElement("reflection");
    {
        QXmlStreamAttributes reflectionAttributes;
        reflectionAttributes.append("enabled", screenieModel.isReflectionEnabled() ? "true" : "false");
        reflectionAttributes.append("offset", QString::number(screenieModel.getReflectionOffset()));
        reflectionAttributes.append("opacity", QString::number(screenieModel.getReflectionOpacity()));
        streamWriter->writeAttributes(reflectionAttributes);
    }
    streamWriter->writeEndElement();
    return result;
}

bool AbstractXmlScreenieModelDao::read(AbstractScreenieModel &abstractScreenieModel)
{
    bool result = true;
    bool ok;
    QXmlStreamReader *streamReader = getStreamReader();

    readSpecific();

    while (streamReader->readNextStartElement()) {
        if (streamReader->name() == "position") {
            QXmlStreamAttributes positionAttributes = streamReader->attributes();
            qreal x = positionAttributes.value("x").toString().toFloat(&ok);
            if (!ok) {
                result = false;
                break;
            }
            qreal y = positionAttributes.value("y").toString().toFloat(&ok);
            if (!ok) {
                result = false;
                break;
            }
            abstractScreenieModel.setPosition(QPointF(x, y));
            qreal z = positionAttributes.value("z").toString().toFloat(&ok);
            if (!ok) {
                result = false;
                break;
            }
            abstractScreenieModel.setDistance(z);
            int roty = positionAttributes.value("roty").toString().toInt(&ok);
            if (!ok) {
                result = false;
                break;
            }
            abstractScreenieModel.setRotation(roty);
            streamReader->skipCurrentElement();

        } else if (streamReader->name() == "reflection") {

            QXmlStreamAttributes reflectionAttributes = streamReader->attributes();
            bool enabled = reflectionAttributes.value("enabled") == "true" ? true : false;
            abstractScreenieModel.setReflectionEnabled(enabled);
            int offset = reflectionAttributes.value("offset").toString().toInt(&ok);
            if (!ok) {
                result = false;
                break;
            }
            abstractScreenieModel.setReflectionOffset(offset);
            int opacity = reflectionAttributes.value("opacity").toString().toInt(&ok);
            if (!ok) {
                result = false;
                break;
            }
            abstractScreenieModel.setReflectionOpacity(opacity);
            streamReader->skipCurrentElement();

        } else {
            // skip unknown features
            streamReader->skipCurrentElement();
        }
    }
    return result;
}

// protected

QXmlStreamWriter *AbstractXmlScreenieModelDao::getStreamWriter() const
{
    return d->streamWriter;
}

QXmlStreamReader *AbstractXmlScreenieModelDao::getStreamReader() const
{
    return d->streamReader;
}



