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

#include "../../ScreenieTemplateModel.h"
#include "XmlScreenieTemplateModelDao.h"

class XmlScreenieTemplateModelDaoPrivate
{
public:
    const ScreenieTemplateModel *writeModel;
    ScreenieTemplateModel *readModel;
};

//  public

XmlScreenieTemplateModelDao::XmlScreenieTemplateModelDao(QXmlStreamWriter *xmlStreamWriter)
    : AbstractXmlScreenieModelDao(xmlStreamWriter),
      d(new XmlScreenieTemplateModelDaoPrivate())
{
}

XmlScreenieTemplateModelDao::XmlScreenieTemplateModelDao(QXmlStreamReader *xmlStreamReader)
    : AbstractXmlScreenieModelDao(xmlStreamReader),
      d(new XmlScreenieTemplateModelDaoPrivate())
{
}

XmlScreenieTemplateModelDao::~XmlScreenieTemplateModelDao()
{
#ifdef DEBUG
    qDebug("XmlScreenieTemplateModelDao::~XmlScreenieTemplateModelDao: called.");
#endif
    delete d;
}

bool XmlScreenieTemplateModelDao::write(const ScreenieTemplateModel &screenieTemplateModel)
{
    bool result;
    d->writeModel = &screenieTemplateModel;
    result = AbstractXmlScreenieModelDao::write(screenieTemplateModel);
    return result;
}

ScreenieTemplateModel *XmlScreenieTemplateModelDao::read()
{
    ScreenieTemplateModel *result = new ScreenieTemplateModel();
    d->readModel = result;
    AbstractXmlScreenieModelDao::read(*result);
    return result;
}

// protected

bool XmlScreenieTemplateModelDao::writeSpecific()
{
    bool result = true;
    /*!\todo Size fitter settings */
    QXmlStreamWriter *streamWriter = getStreamWriter();
    streamWriter->writeStartElement("template");
    {
        QXmlStreamAttributes templateAttributes;
        templateAttributes.append("order", QString::number(d->writeModel->getOrder()));
        streamWriter->writeAttributes(templateAttributes);
    }
    streamWriter->writeEndElement();
    return result;
}

bool XmlScreenieTemplateModelDao::readSpecific()
{
    bool result = true;
    bool ok;
    /*!\todo Size fitter settings */
    QXmlStreamReader *streamReader = getStreamReader();
    streamReader->readNextStartElement();
    QXmlStreamAttributes templateAttributes = streamReader->attributes();
    int order = templateAttributes.value("order").toString().toInt(&ok);
    if (ok) {
        d->readModel->setOrder(order);
    } else {
        result = false;
    }
    streamReader->skipCurrentElement();
    return result;
}

