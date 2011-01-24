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
#include <QtCore/QBuffer>
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QXmlStreamWriter>
#include <QtGui/QImage>

#include "../../ScreenieImageModel.h"
#include "XmlScreenieImageModelDao.h"

class XmlScreenieImageModelDaoPrivate
{
public:
    const ScreenieImageModel *writeModel;
    ScreenieImageModel *readModel;
};

//  public

XmlScreenieImageModelDao::XmlScreenieImageModelDao(QXmlStreamWriter &xmlStreamWriter)
    : AbstractXmlScreenieModelDao(xmlStreamWriter),
      d(new XmlScreenieImageModelDaoPrivate())
{
}

XmlScreenieImageModelDao::XmlScreenieImageModelDao(QXmlStreamReader &xmlStreamReader)
    : AbstractXmlScreenieModelDao(xmlStreamReader),
      d(new XmlScreenieImageModelDaoPrivate())
{
}

XmlScreenieImageModelDao::~XmlScreenieImageModelDao()
{
#ifdef DEBUG
    qDebug("XmlScreenieImageModelDao::~XmlScreenieImageModelDao: called.");
#endif
    delete d;
}

bool XmlScreenieImageModelDao::write(const ScreenieImageModel &screenieImageModel)
{
    bool result;
    d->writeModel = &screenieImageModel;
    result = AbstractXmlScreenieModelDao::write(screenieImageModel);
    return result;
}

ScreenieImageModel *XmlScreenieImageModelDao::read()
{
    ScreenieImageModel *result = new ScreenieImageModel();
    d->readModel = result;
    AbstractXmlScreenieModelDao::read(*result);
    return result;
}

// protected

bool XmlScreenieImageModelDao::writeSpecific()
{
    bool result = true;
    QXmlStreamWriter *streamWriter = getStreamWriter();
    streamWriter->writeStartElement("img");
    {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        d->writeModel->getImage().save(&buffer, "PNG");
        buffer.close();
        QByteArray byteArray = buffer.buffer();
        QString data = QString(byteArray.toBase64());
        streamWriter->writeCDATA(data);
    }
    streamWriter->writeEndElement();
    return result;
}

bool XmlScreenieImageModelDao::readSpecific()
{
    bool result = true;
    QXmlStreamReader *streamReader = getStreamReader();
    streamReader->readNextStartElement();

    QString data = streamReader->readElementText();
    if (!data.isEmpty()) {
        QByteArray str;
        str.append(data);
        QByteArray png = QByteArray::fromBase64(str);

        QImage image;
        result = image.loadFromData(png, "PNG");
        if (result && !image.isNull()) {
            d->readModel->setImage(image);
        } else {
            result = false;
        }
    } else {
        result = false;
    }
    return result;
}

