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
#include <QtCore/QByteArray>
#include <QtXml/QXmlStreamWriter>

#include "../../ScreeniePixmapModel.h"
#include "XmlScreeniePixmapModelDao.h"

class XmlScreeniePixmapModelDaoPrivate
{
public:
    const ScreeniePixmapModel *screeniePixmapModel;
};

//  public

XmlScreeniePixmapModelDao::XmlScreeniePixmapModelDao(QXmlStreamWriter &streamWriter)
    : AbstractXmlScreenieModelDao(streamWriter),
      d(new XmlScreeniePixmapModelDaoPrivate())
{

}

XmlScreeniePixmapModelDao::~XmlScreeniePixmapModelDao(){
    delete d;
}

bool XmlScreeniePixmapModelDao::write(const ScreeniePixmapModel &screeniePixmapModel)
{
    bool result;
    d->screeniePixmapModel = &screeniePixmapModel;
    result = AbstractXmlScreenieModelDao::writeCommon(screeniePixmapModel);
    return result;
}

ScreeniePixmapModel *XmlScreeniePixmapModelDao::read()
{
    ScreeniePixmapModel *result = 0;
    return result;
}

// protected

bool XmlScreeniePixmapModelDao::writeSpecific()
{
    bool result = true;
    getStreamWriter().writeStartElement("img");
    {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        d->screeniePixmapModel->getPixmap().save(&buffer, "PNG");
        buffer.close();
        QByteArray byteArray = buffer.buffer();
        QString data = QString(byteArray.toBase64());
        getStreamWriter().writeCDATA(data);
    }
    getStreamWriter().writeEndElement();
    return result;
}

bool XmlScreeniePixmapModelDao::readSpecific()
{
    bool result;
    result = false;
    return result;
}

