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
#include <QtCore/QIODevice>
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamAttributes>

#include "../../AbstractScreenieModel.h"
#include "AbstractXmlScreenieModelDao.h"

class AbstractXmlScreenieModelDaoPrivate
{
public:
    AbstractXmlScreenieModelDaoPrivate(QIODevice *theDevice)
        : device(theDevice),
          streamWriter(0),
          streamReader(0)
    {}

    AbstractXmlScreenieModelDaoPrivate(QXmlStreamWriter *xmlStreamWriter)
        : device(0),
          streamWriter(xmlStreamWriter),
          streamReader(0)
    {}

    AbstractXmlScreenieModelDaoPrivate(QXmlStreamReader *xmlStreamReader)
        : device(0),
          streamWriter(0),
          streamReader(xmlStreamReader)
    {}

    QIODevice *device;
    QXmlStreamWriter *streamWriter;
    QXmlStreamReader *streamReader;
};

// public

AbstractXmlScreenieModelDao::AbstractXmlScreenieModelDao(QIODevice &device)
    : d(new AbstractXmlScreenieModelDaoPrivate(&device))
{
}

AbstractXmlScreenieModelDao::AbstractXmlScreenieModelDao(QXmlStreamWriter &xmlStreamWriter)
    : d(new AbstractXmlScreenieModelDaoPrivate(&xmlStreamWriter))
{    
}

AbstractXmlScreenieModelDao::AbstractXmlScreenieModelDao(QXmlStreamReader &xmlStreamReader)
    : d(new AbstractXmlScreenieModelDaoPrivate(&xmlStreamReader))
{
}

AbstractXmlScreenieModelDao::~AbstractXmlScreenieModelDao()
{
#ifdef DEBUG
    qDebug("AbstractXmlScreenieModelDao::~AbstractXmlScreenieModelDao: called.");
#endif
    cleanUp();
    delete d;
}

bool AbstractXmlScreenieModelDao::write(const AbstractScreenieModel &screenieModel)
{
    bool result = true;
    QXmlStreamWriter *streamWriter = getStreamWriter();

    if (streamWriter != 0) {

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
    } else {
        result = false;
    }
    return result;
}

bool AbstractXmlScreenieModelDao::read(AbstractScreenieModel &abstractScreenieModel)
{
    bool result = true;
    bool ok;
    QXmlStreamReader *streamReader = getStreamReader();

    if (streamReader != 0) {

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
    } else {
        result = false;
    }
    return result;
}

// protected

QXmlStreamWriter *AbstractXmlScreenieModelDao::getStreamWriter() const
{
    QXmlStreamWriter *result;
    if (d->streamWriter != 0) {
        result = d->streamWriter;
    } else if (d->device != 0) {
        // PRE: if no stream writer is given, device must be valid
        if (d->device->open(QIODevice::WriteOnly)) {
            result = new QXmlStreamWriter(d->device);
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    return result;
}

QXmlStreamReader *AbstractXmlScreenieModelDao::getStreamReader() const
{
    QXmlStreamReader *result;
    if (d->streamReader != 0) {
        result = d->streamReader;
    } else if (d->device != 0) {
        if (d->device->open(QIODevice::ReadOnly)) {
            result = new QXmlStreamReader(d->device);
        } else {
            result = 0;
        }
    } else {
        result = 0;
    }
    return result;
}

// private

void AbstractXmlScreenieModelDao::cleanUp()
{
    // if a QIODevice was given in the c'tor, then we know
    // that WE created the readers, so we have to deallocate
    // them
    if (d->device != 0) {
        if (d->streamReader != 0) {
            delete d->streamReader;
            d->streamReader = 0;
        }
        if (d->streamWriter != 0) {
            delete d->streamWriter;
            d->streamWriter = 0;
        }
    }
}



