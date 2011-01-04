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
#include <QtGui/QPixmap>

#include "../../ScreeniePixmapModel.h"
#include "XmlScreeniePixmapModelDao.h"

class XmlScreeniePixmapModelDaoPrivate
{
public:
    const ScreeniePixmapModel *writeModel;
    ScreeniePixmapModel *readModel;
};

//  public

XmlScreeniePixmapModelDao::XmlScreeniePixmapModelDao(QXmlStreamWriter &xmlStreamWriter)
    : AbstractXmlScreenieModelDao(xmlStreamWriter),
      d(new XmlScreeniePixmapModelDaoPrivate())
{
}

XmlScreeniePixmapModelDao::XmlScreeniePixmapModelDao(QXmlStreamReader &xmlStreamReader)
    : AbstractXmlScreenieModelDao(xmlStreamReader),
      d(new XmlScreeniePixmapModelDaoPrivate())
{
}

XmlScreeniePixmapModelDao::~XmlScreeniePixmapModelDao()
{
#ifdef DEBUG
    qDebug("XmlScreeniePixmapModelDao::~XmlScreeniePixmapModelDao: called.");
#endif
    delete d;
}

bool XmlScreeniePixmapModelDao::write(const ScreeniePixmapModel &screeniePixmapModel)
{
    bool result;
    d->writeModel = &screeniePixmapModel;
    result = AbstractXmlScreenieModelDao::write(screeniePixmapModel);
    return result;
}

ScreeniePixmapModel *XmlScreeniePixmapModelDao::read()
{
    ScreeniePixmapModel *result = new ScreeniePixmapModel();
    d->readModel = result;
    AbstractXmlScreenieModelDao::read(*result);
    return result;
}

// protected

bool XmlScreeniePixmapModelDao::writeSpecific()
{
    bool result = true;
    QXmlStreamWriter *streamWriter = getStreamWriter();
    streamWriter->writeStartElement("img");
    {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        d->writeModel->getPixmap().save(&buffer, "PNG");
        buffer.close();
        QByteArray byteArray = buffer.buffer();
        QString data = QString(byteArray.toBase64());
        streamWriter->writeCDATA(data);
    }
    streamWriter->writeEndElement();
    return result;
}

bool XmlScreeniePixmapModelDao::readSpecific()
{
    bool result = true;
    QXmlStreamReader *streamReader = getStreamReader();
    streamReader->readNextStartElement();

    QString data = streamReader->readElementText();
    if (!data.isEmpty()) {
        QByteArray str;
        str.append(data);
        QByteArray png = QByteArray::fromBase64(str);

        QPixmap pixmap;
        result = pixmap.loadFromData(png, "PNG");
        if (result && !pixmap.isNull()) {
            d->readModel->setPixmap(pixmap);
        } else {
            result = false;
        }
    } else {
        result = false;
    }
    return result;
}

