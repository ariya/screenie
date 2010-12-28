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

#include "../../ScreenieFilePathModel.h"
#include "XmlScreenieFilePathModelDao.h"

class XmlScreenieFilePathModelDaoPrivate
{
public:
    const ScreenieFilePathModel *writeModel;
    ScreenieFilePathModel *readModel;
};

//  public

XmlScreenieFilePathModelDao::XmlScreenieFilePathModelDao(QXmlStreamWriter *xmlStreamWriter)
    : AbstractXmlScreenieModelDao(xmlStreamWriter),
      d(new XmlScreenieFilePathModelDaoPrivate())
{
}

XmlScreenieFilePathModelDao::XmlScreenieFilePathModelDao(QXmlStreamReader *xmlStreamReader)
    : AbstractXmlScreenieModelDao(xmlStreamReader),
      d(new XmlScreenieFilePathModelDaoPrivate())
{
}

XmlScreenieFilePathModelDao::~XmlScreenieFilePathModelDao(){
    delete d;
}

bool XmlScreenieFilePathModelDao::write(const ScreenieFilePathModel &screenieFilePathModel)
{
    bool result;
    d->writeModel = &screenieFilePathModel;
    result = AbstractXmlScreenieModelDao::write(screenieFilePathModel);
    return result;
}

ScreenieFilePathModel *XmlScreenieFilePathModelDao::read()
{
    ScreenieFilePathModel *result = new ScreenieFilePathModel();
    d->readModel = result;
    AbstractXmlScreenieModelDao::read(*result);
    return result;
}

// protected

bool XmlScreenieFilePathModelDao::writeSpecific()
{
    bool result = true;
    QXmlStreamWriter *streamWriter = getStreamWriter();
    streamWriter->writeTextElement("path", d->writeModel->getFilePath());
    return result;
}

bool XmlScreenieFilePathModelDao::readSpecific()
{
    bool result = true;
    /*!\todo Check file path existence */
    QXmlStreamReader *streamReader = getStreamReader();
    streamReader->readNextStartElement();
    QString filePath = streamReader->readElementText();
    d->readModel->setFilePath(filePath);
    return result;
}

