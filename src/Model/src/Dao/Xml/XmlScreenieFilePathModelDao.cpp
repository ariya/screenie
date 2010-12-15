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

#include "../../ScreenieFilePathModel.h"
#include "XmlScreenieFilePathModelDao.h"

class XmlScreenieFilePathModelDaoPrivate
{
public:
    const ScreenieFilePathModel *screenieFilePathModel;
};

//  public

XmlScreenieFilePathModelDao::XmlScreenieFilePathModelDao(QXmlStreamWriter &streamWriter)
    : AbstractXmlScreenieModelDao(streamWriter)
{
    d = new XmlScreenieFilePathModelDaoPrivate();
}

XmlScreenieFilePathModelDao::~XmlScreenieFilePathModelDao(){
    delete d;
}

bool XmlScreenieFilePathModelDao::write(const ScreenieFilePathModel &screenieFilePathModel)
{
    bool result;
    d->screenieFilePathModel = &screenieFilePathModel;
    result = AbstractXmlScreenieModelDao::writeCommon(screenieFilePathModel);
    return result;
}

ScreenieFilePathModel *XmlScreenieFilePathModelDao::read()
{
    ScreenieFilePathModel *result = 0;
    return result;
}

// protected

bool XmlScreenieFilePathModelDao::writeSpecific()
{
    bool result = true;
    getStreamWriter().writeAttribute("path", d->screenieFilePathModel->getFilePath());
    return result;
}

bool XmlScreenieFilePathModelDao::readSpecific()
{
    bool result;
    result = false;
    return result;
}

