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
#include <QtCore/QStringRef>
#include <QtCore/QIODevice>
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QXmlStreamReader>
#include <QtGui/QColor>

#include "../../../../Utils/src/Version.h"
#include "../../ScreenieScene.h"
#include "../../ScreenieModelInterface.h"
#include "../../ScreenieFilePathModel.h"
#include "../../ScreenieImageModel.h"
#include "../../ScreenieTemplateModel.h"
#include "../ScreenieFilePathModelDao.h"
#include "../ScreenieImageModelDao.h"
#include "../ScreenieTemplateModelDao.h"
#include "../ScreenieSceneSerializer.h"
#include "XmlScreenieFilePathModelDao.h"
#include "XmlScreenieImageModelDao.h"
#include "XmlScreenieTemplateModelDao.h"
#include "XmlScreenieSceneDao.h"

// public

class XmlScreenieSceneDaoPrivate
{
public:
    XmlScreenieSceneDaoPrivate(QIODevice &theDevice)
        : device(theDevice),
          streamWriter(0),
          streamReader(0),
          screenieFilePathModelDao(0),
          screeniePixmapModelDao(0),
          screenieTemplateModelDao(0)
    {}

    QIODevice &device;
    Version version;
    QXmlStreamWriter *streamWriter;
    QXmlStreamReader *streamReader;
    ScreenieFilePathModelDao *screenieFilePathModelDao;
    ScreenieImageModelDao *screeniePixmapModelDao;
    ScreenieTemplateModelDao *screenieTemplateModelDao;
};

XmlScreenieSceneDao::XmlScreenieSceneDao(QIODevice &device)
    : d(new XmlScreenieSceneDaoPrivate(device))
{
}

XmlScreenieSceneDao::~XmlScreenieSceneDao()
{
    cleanUp();
    delete d;
}

bool XmlScreenieSceneDao::write(const ScreenieScene &screenieScene)
{
    return write(screenieScene, ScreenieSceneSerializer::FullScene);
}

ScreenieScene *XmlScreenieSceneDao::read() const
{
    ScreenieScene *result = 0;
    if (d->device.open(QIODevice::ReadOnly)) {
        d->streamReader = new QXmlStreamReader(&d->device);
        QXmlStreamReader::TokenType tokenType;
        while ((tokenType = d->streamReader->readNext()) != QXmlStreamReader::EndDocument) {
            if (tokenType == QXmlStreamReader::StartElement) {
                if (d->streamReader->name() == "screeniescene") {
                    QXmlStreamAttributes sceneAttributes = d->streamReader->attributes();
                    QString versionString = sceneAttributes.value("version").toString();
                    Version documentVersion(versionString);
                    if (documentVersion < d->version) {
                        /*!\todo Convert file to current version */
#ifdef DEBUG
                        qDebug("XmlScreenieSceneDao::read: CONVERSION NEEDED, document version: %s, app version: %s", qPrintable(documentVersion.toString()), qPrintable(d->version.toString()));
#endif
                    }
                    result = readScreenieScene();
                } else {
                    result = 0;
                }
            }
        }
        d->device.close();
    } else {
        result = 0;
    }
    cleanUp();
    return result;
}

bool XmlScreenieSceneDao::write(const ScreenieScene &screenieScene, ScreenieSceneSerializer::Mode mode)
{
    bool result;
    if (d->device.open(QIODevice::WriteOnly)) {
        d->streamWriter = new QXmlStreamWriter(&d->device);
        d->streamWriter->setAutoFormatting(true);
        d->streamWriter->writeStartDocument();
        result = writeScreenieScene(screenieScene, mode);
        d->streamWriter->writeEndDocument();
        d->device.close();
    } else {
        result = false;
    }
    cleanUp();
    return result;
}

// private

bool XmlScreenieSceneDao::writeScreenieScene(const ScreenieScene &screenieScene, ScreenieSceneSerializer::Mode mode)
{
    bool result = true;
    d->streamWriter->writeDTD("<!DOCTYPE screenie>");
    d->streamWriter->writeStartElement("screeniescene");

    QXmlStreamAttributes sceneAttributes;
    sceneAttributes.append("version", d->version.toString());
    sceneAttributes.append("template", screenieScene.isTemplate() ? "true" : "false");
    d->streamWriter->writeAttributes(sceneAttributes);
    {
        switch (mode) {
        case ScreenieSceneSerializer::FullScene:
            d->streamWriter->writeStartElement("background");
            {
                QXmlStreamAttributes backgroundAttributes;
                backgroundAttributes.append("enabled", screenieScene.isBackgroundEnabled() ? "true" : "false");
                backgroundAttributes.append("bgcolor", screenieScene.getBackgroundColor().name());
                d->streamWriter->writeAttributes(backgroundAttributes);
            }
            d->streamWriter->writeEndElement();
            result = writeScreenieModels(screenieScene, mode);
            break;
        case ScreenieSceneSerializer::SelectedItems:
            result = writeScreenieModels(screenieScene, mode);
            break;
        default:
#ifdef DEBUG
            qCritical("XmlScreenieSceneDao::writeScreenieScene: UNSUPPORTED mode: %d", mode);
#endif
            result = false;
        }
    }
    d->streamWriter->writeEndElement();
    return result;
}

bool XmlScreenieSceneDao::writeScreenieModels(const ScreenieScene &screenieScene, ScreenieSceneSerializer::Mode mode)
{
    bool result = true;

    QList<ScreenieModelInterface *> screenieModels;
    switch (mode) {
    case ScreenieSceneSerializer::FullScene:
        screenieModels = screenieScene.getModels();
        break;
    case ScreenieSceneSerializer::SelectedItems:
        screenieModels = screenieScene.getSelectedModels();
        break;
    default:
#ifdef DEBUG
        qCritical("XmlScreenieSceneDao::writeScreenieModels: UNSUPPORTED mode: %d", mode);
#endif
        break;
    }

    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        if (screenieModel->inherits(ScreenieFilePathModel::staticMetaObject.className())) {
            result = writeFilePathModel(static_cast<ScreenieFilePathModel &>(*screenieModel));
        } else if (screenieModel->inherits(ScreenieImageModel::staticMetaObject.className())) {
            result = writePixmapModel(static_cast<ScreenieImageModel &>(*screenieModel));
        } else if (screenieModel->inherits(ScreenieTemplateModel::staticMetaObject.className())) {
            result = writeTemplateModel(static_cast<ScreenieTemplateModel &>(*screenieModel));
        }
#ifdef DEBUG
        else {
            qCritical("XmlScreenieSceneDao::writeScreenieModels: UNSUPPORTED: %s", screenieModel->metaObject()->className());
        }
#endif
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

bool XmlScreenieSceneDao::writePixmapModel(const ScreenieImageModel &screeniePixmapModel)
{
    bool result;
    if (d->screeniePixmapModelDao == 0) {
        d->screeniePixmapModelDao = new XmlScreenieImageModelDao(*d->streamWriter);
    }
    d->streamWriter->writeStartElement("pixmapmodel");
    {
        result = d->screeniePixmapModelDao->write(screeniePixmapModel);
    }
    d->streamWriter->writeEndElement();

    return result;
}

bool XmlScreenieSceneDao::writeTemplateModel(const ScreenieTemplateModel &screenieTemplateModel)
{
    bool result;
    if (d->screenieTemplateModelDao == 0) {
        d->screenieTemplateModelDao = new XmlScreenieTemplateModelDao(*d->streamWriter);
    }
    d->streamWriter->writeStartElement("templatemodel");
    {
        result = d->screenieTemplateModelDao->write(screenieTemplateModel);
    }
    d->streamWriter->writeEndElement();

    return result;
}

ScreenieScene *XmlScreenieSceneDao::readScreenieScene() const
{
    ScreenieScene *result = new ScreenieScene();
    QXmlStreamAttributes sceneAttributes = d->streamReader->attributes();
    bool isTemplate = sceneAttributes.value("isTemplate") == "true" ? true : false;
    result->setTemplate(isTemplate);
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
            ScreenieImageModel *pixmapModel = readPixmapModel();
            if (pixmapModel != 0) {
                result->addModel(pixmapModel);
            } else {
                ok = false;
            }
        } else if (d->streamReader->name() == "templatemodel") {
            ScreenieTemplateModel *templateModel = readTemplateModel();
            if (templateModel != 0) {
                result->addModel(templateModel);
            } else {
                ok = false;
            }
        } else {
#ifdef DEBUG
            qCritical("XmlScreenieSceneDao::readScreenieScene: UNSUPPORTED: %s", qPrintable(d->streamReader->name().toString()));
#endif
            d->streamReader->skipCurrentElement();
        }

    }
    ok = ok && d->streamReader->error() == QXmlStreamReader::NoError;
#ifdef DEBUG
        qDebug("XmlScreenieSceneDao::readScreenieScene: streamReader error: %d, ok: %d", d->streamReader->error(), ok);
#endif
    if (ok) {
        result->setModified(false);
    } else {
        delete result;
        result = 0;
    }
    return result;
}

ScreenieFilePathModel *XmlScreenieSceneDao::readFilePathModel() const
{
    ScreenieFilePathModel *result;

    if (d->screenieFilePathModelDao == 0) {
        d->screenieFilePathModelDao = new XmlScreenieFilePathModelDao(*d->streamReader);
    }
    result = d->screenieFilePathModelDao->read();
    return result;
}

ScreenieImageModel *XmlScreenieSceneDao::readPixmapModel() const
{
    ScreenieImageModel *result;

    if (d->screeniePixmapModelDao == 0) {
        d->screeniePixmapModelDao = new XmlScreenieImageModelDao(*d->streamReader);
    }
    result = d->screeniePixmapModelDao->read();
    return result;
}

ScreenieTemplateModel *XmlScreenieSceneDao::readTemplateModel() const
{
    ScreenieTemplateModel *result;

    if (d->screenieTemplateModelDao == 0) {
        d->screenieTemplateModelDao = new XmlScreenieTemplateModelDao(*d->streamReader);
    }
    result = d->screenieTemplateModelDao->read();
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
    if (d->screenieTemplateModelDao != 0) {
        delete d->screenieTemplateModelDao;
        d->screenieTemplateModelDao = 0;
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
