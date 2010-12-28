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
#include <QtCore/QStringRef>
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QXmlStreamReader>
#include <QtGui/QColor>

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
    XmlScreenieSceneDaoPrivate(const QString &filePath)
        : file(filePath),
          version("1.0"),
          streamWriter(0),
          streamReader(0),
          screenieFilePathModelDao(0),
          screeniePixmapModelDao(0)
    {
    }
    QFile file;
    QString version;
    QXmlStreamWriter *streamWriter;
    QXmlStreamReader *streamReader;
    ScreenieFilePathModelDao *screenieFilePathModelDao;
    ScreeniePixmapModelDao *screeniePixmapModelDao;
};

XmlScreenieSceneDao::XmlScreenieSceneDao(const QString &filePath)
{
    d = new XmlScreenieSceneDaoPrivate(filePath);
}

XmlScreenieSceneDao::~XmlScreenieSceneDao()
{
    cleanUp();
    delete d;
}

bool XmlScreenieSceneDao::write(const ScreenieScene &screenieScene)
{
    bool result;
    if (d->file.open(QIODevice::WriteOnly)) {
        d->streamWriter = new QXmlStreamWriter(&d->file);
        d->streamWriter->setAutoFormatting(true);
        d->streamWriter->writeStartDocument();
        result = writeScreenieScene(screenieScene);
        d->streamWriter->writeEndDocument();
        d->file.close();
        result = result && QFile::NoError == d->file.error();
    } else {
        result = false;
    }
    cleanUp();
    return result;
}

ScreenieScene *XmlScreenieSceneDao::read() const
{
    ScreenieScene *result = 0;
    if (d->file.open(QIODevice::ReadOnly)) {
        d->streamReader = new QXmlStreamReader(&d->file);
        QXmlStreamReader::TokenType tokenType;
        while ((tokenType = d->streamReader->readNext()) != QXmlStreamReader::EndDocument) {
            if (tokenType == QXmlStreamReader::StartElement) {
                if (d->streamReader->name() == "screeniescene") {
    #ifdef DEBUG
                    qDebug("XmlScreenieSceneDao::read: version: %s", qPrintable(d->streamReader->attributes().value("version").toString()));
    #endif
                    if (d->streamReader->attributes().value("version") != d->version) {
                        /*!\todo Convert file to current version */
                    }
                    result = readScreenieScene();
                } else {
                    result = 0;
                }
            }
        }
        d->file.close();
    } else {
        result = 0;
    }
    cleanUp();
    return result;
}

// private

bool XmlScreenieSceneDao::writeScreenieScene(const ScreenieScene &screenieScene)
{
    bool result = true;
    d->streamWriter->writeDTD("<!DOCTYPE screenie>");
    d->streamWriter->writeStartElement("screeniescene");
    d->streamWriter->writeAttribute("version", d->version);
    {
        d->streamWriter->writeStartElement("background");
        {
            QXmlStreamAttributes backgroundAttributes;
            backgroundAttributes.append("enabled", screenieScene.isBackgroundEnabled() ? "true" : "false");
            backgroundAttributes.append("bgcolor", screenieScene.getBackgroundColor().name());
            d->streamWriter->writeAttributes(backgroundAttributes);
        }
        d->streamWriter->writeEndElement();
        result = writeScreenieModels(screenieScene);
    }
    d->streamWriter->writeEndElement();
    return result;
}

bool XmlScreenieSceneDao::writeScreenieModels(const ScreenieScene &screenieScene)
{
    bool result;
    int n = screenieScene.count();
    result = true;
    for (int i = 0; result && i < n; ++i) {
        ScreenieModelInterface *screenieModel = screenieScene.getModel(i);
        if (screenieModel->inherits(ScreenieFilePathModel::staticMetaObject.className())) {
            result = writeFilePathModel(dynamic_cast<ScreenieFilePathModel &>(*screenieModel));
        } else if (screenieModel->inherits(ScreeniePixmapModel::staticMetaObject.className())) {
            result = writePixmapModel(dynamic_cast<ScreeniePixmapModel &>(*screenieModel));
        }
#ifdef DEBUG
        else {
            qDebug("XmlScreenieSceneDao::writeScreenieModels: UNSUPPORTED: %s", screenieModel->metaObject()->className());
        }
#endif
    }
    return result;
}

bool XmlScreenieSceneDao::writeFilePathModel(const ScreenieFilePathModel &screenieFilePathModel)
{
    bool result;
    if (d->screenieFilePathModelDao == 0) {
        d->screenieFilePathModelDao = new XmlScreenieFilePathModelDao(d->streamWriter);
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
        d->screeniePixmapModelDao = new XmlScreeniePixmapModelDao(d->streamWriter);
    }
    d->streamWriter->writeStartElement("pixmapmodel");
    {
        result = d->screeniePixmapModelDao->write(screeniePixmapModel);
    }
    d->streamWriter->writeEndElement();

    return result;
}

ScreenieScene *XmlScreenieSceneDao::readScreenieScene() const
{
    ScreenieScene *result = new ScreenieScene();
    bool ok = true;
    while (ok && d->streamReader->readNextStartElement()) {

        if (d->streamReader->name() == "background") {
            QXmlStreamAttributes backgroundAttributes = d->streamReader->attributes();
            bool enabled = backgroundAttributes.value("enabled") == "true" ? true : false;
            result->setBackgroundEnabled(enabled);
            QColor color(backgroundAttributes.value("bgcolor").toString());
            result->setBackgroundColor(color);
            d->streamReader->skipCurrentElement();
        } else if (d->streamReader->name() == "filepathmodel") {
            ScreenieFilePathModel *filePathModel = readFilePathModel();
            if (filePathModel != 0) {
                result->addModel(filePathModel);
            } else {
                ok = false;
            }
        } else if (d->streamReader->name() == "pixmapmodel") {
            ScreeniePixmapModel *pixmapModel = readPixmapModel();
            if (pixmapModel != 0) {
                result->addModel(pixmapModel);
            } else {
                ok = false;
            }
        } else {
            d->streamReader->skipCurrentElement();
        }

    }
    ok = ok && d->streamReader->error() == QXmlStreamReader::NoError;
#ifdef DEBUG
        qDebug("XmlScreenieSceneDao::readScreenieScene: streamReader error: %d, ok: %d", d->streamReader->error(), ok);
#endif
    if (!ok) {
        delete result;
        result = 0;
    }
    return result;
}

ScreenieFilePathModel *XmlScreenieSceneDao::readFilePathModel() const
{
    ScreenieFilePathModel *result;

    if (d->screenieFilePathModelDao == 0) {
        d->screenieFilePathModelDao = new XmlScreenieFilePathModelDao(d->streamReader);
    }
    result = d->screenieFilePathModelDao->read();
    return result;
}

ScreeniePixmapModel *XmlScreenieSceneDao::readPixmapModel() const
{
    ScreeniePixmapModel *result;

    if (d->screeniePixmapModelDao == 0) {
        d->screeniePixmapModelDao = new XmlScreeniePixmapModelDao(d->streamReader);
    }
    result = d->screeniePixmapModelDao->read();
    return result;
}


void XmlScreenieSceneDao::cleanUp() const
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
    if (d->streamReader != 0) {
        delete d->streamReader;
        d->streamReader = 0;
    }
}
