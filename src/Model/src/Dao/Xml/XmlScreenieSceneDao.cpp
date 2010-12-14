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
#include "XmlScreenieSceneDao.h"

// public

class XmlScreenieSceneDaoPrivate
{
public:
    XmlScreenieSceneDaoPrivate(ScreenieScene &scene)
        : screenieScene(scene),
          version("1.0")
    {}

    ScreenieScene &screenieScene;
    QString version;
    QXmlStreamWriter *streamWriter;
};

XmlScreenieSceneDao::XmlScreenieSceneDao(ScreenieScene &screenieScene)
{
    d = new XmlScreenieSceneDaoPrivate(screenieScene);
}

XmlScreenieSceneDao::~XmlScreenieSceneDao()
{
    delete d;
}

bool XmlScreenieSceneDao::store(const QString &filePath)
{
    bool result;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        d->streamWriter = new QXmlStreamWriter(&file);
        d->streamWriter->setAutoFormatting(true);
        d->streamWriter->writeStartDocument(d->version, true);
        writeScreenieScene();
        d->streamWriter->writeEndDocument();
        delete d->streamWriter;
        file.close();
        result = QFile::NoError == file.error();
    } else {
        result = false;
    }
    return result;
}

bool XmlScreenieSceneDao::restore(const QString &filePath)
{
    bool result = true;
    /*!\todo Implement this */
    return result;
}

// private

void XmlScreenieSceneDao::writeScreenieScene()
{
    d->streamWriter->writeStartElement("screeniescene");
    d->streamWriter->writeTextElement("nofitems", QString::number(d->screenieScene.count()));
    d->streamWriter->writeEndElement();
}
