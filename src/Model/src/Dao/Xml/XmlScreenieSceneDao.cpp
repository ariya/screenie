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

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtXml/QXmlStreamWriter>

#include "../../ScreenieScene.h"
#include "../../ScreenieModelInterface.h"
#include "../../ScreenieFilePathModel.h"
#include "../../ScreeniePixmapModel.h"
#include "../ScreenieFilePathModelDao.h"
#include "XmlScreenieFilePathModelDao.h"
#include "XmlScreeniePixmapModelDao.h"
#include "XmlScreenieSceneDao.h"

// public

class XmlScreenieSceneDaoPrivate
{
public:
    XmlScreenieSceneDaoPrivate(const QString &path)
        : filePath(path),
          version("1.0"),
          streamWriter(0),
          screenieFilePathModelDao(0),
          screeniePixmapModelDao(0)
    {
    }

    QString filePath;
    QString version;
    QXmlStreamWriter *streamWriter;
    ScreenieFilePathModelDao *screenieFilePathModelDao;
    ScreeniePixmapModelDao *screeniePixmapModelDao;
};

XmlScreenieSceneDao::XmlScreenieSceneDao(const QString &filePath)
    : d(new XmlScreenieSceneDaoPrivate(filePath))
{
}

XmlScreenieSceneDao::~XmlScreenieSceneDao()
{
    cleanUp();
    delete d;
}

bool XmlScreenieSceneDao::write(const ScreenieScene &screenieScene)
{
    bool result;
    QFile file(d->filePath);
    if (file.open(QIODevice::WriteOnly)) {
        d->streamWriter = new QXmlStreamWriter(&file);
        d->streamWriter->setAutoFormatting(true);
        d->streamWriter->writeStartDocument(d->version, true);
        result = writeScreenieScene(screenieScene);
        d->streamWriter->writeEndDocument();
        file.close();
        result = result && QFile::NoError == file.error();
    } else {
        result = false;
    }
    cleanUp();
    return result;
}

ScreenieScene *XmlScreenieSceneDao::read()
{
    ScreenieScene *result = 0;
    /*!\todo Implement this */
    return result;
}

// private

bool XmlScreenieSceneDao::writeScreenieScene(const ScreenieScene &screenieScene)
{
    bool result = true;
    d->streamWriter->writeStartElement("screeniescene");
    {
        d->streamWriter->writeStartElement("background");
        {
            QXmlStreamAttributes backgroundAttributes;
            backgroundAttributes.append("enabled", screenieScene.isBackgroundEnabled() ? "true" : "false");
            backgroundAttributes.append("bgcolor", screenieScene.getBackgroundColor().name());
            d->streamWriter->writeAttributes(backgroundAttributes);
        }
        d->streamWriter->writeEndElement();
        d->streamWriter->writeStartElement("items");
        {
            result = writeScreenieModels(screenieScene);
        }
        d->streamWriter->writeEndElement();
    }
    d->streamWriter->writeEndElement();
    return result;
}

bool XmlScreenieSceneDao::writeScreenieModels(const ScreenieScene &screenieScene)
{
    bool result;
    int n = screenieScene.count();
    d->streamWriter->writeAttribute("count", QString::number(n));
    result = true;
    for (int i = 0; result && i < n; ++i) {
        ScreenieModelInterface *screenieModel = screenieScene.getModel(i);
        if (screenieModel->inherits(ScreenieFilePathModel::staticMetaObject.className())) {
            result = writeFilePathModel(dynamic_cast<ScreenieFilePathModel &>(*screenieModel));
        } else if (screenieModel->inherits(ScreeniePixmapModel::staticMetaObject.className())) {
            result = writePixmapModel(dynamic_cast<ScreeniePixmapModel &>(*screenieModel));
        }
    }
    return result;
}

bool XmlScreenieSceneDao::writeFilePathModel(const ScreenieFilePathModel &screenieFilePathModel)
{
    bool result;
    if (d->screenieFilePathModelDao == 0) {
        d->screenieFilePathModelDao = new XmlScreenieFilePathModelDao(*d->streamWriter);
    }
    d->streamWriter->writeStartElement("filepathmodel");
    {
        result = d->screenieFilePathModelDao->write(screenieFilePathModel);
    }
    d->streamWriter->writeEndElement();

    return result;
}

bool XmlScreenieSceneDao::writePixmapModel(const ScreeniePixmapModel &screeniePixmapModel)
{
    bool result;
    if (d->screeniePixmapModelDao == 0) {
        d->screeniePixmapModelDao = new XmlScreeniePixmapModelDao(*d->streamWriter);
    }
    d->streamWriter->writeStartElement("pixmapmodel");
    {
        result = d->screeniePixmapModelDao->write(screeniePixmapModel);
    }
    d->streamWriter->writeEndElement();

    return result;
}

void XmlScreenieSceneDao::cleanUp()
{
    if (d->screenieFilePathModelDao != 0) {
        delete d->screenieFilePathModelDao;
        d->screenieFilePathModelDao = 0;
    }
    if (d->screeniePixmapModelDao != 0) {
        delete d->screeniePixmapModelDao;
        d->screeniePixmapModelDao = 0;
    }
    if (d->streamWriter != 0) {
        delete d->streamWriter;
        d->streamWriter = 0;
    }
}
