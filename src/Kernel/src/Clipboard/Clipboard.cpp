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

#include <QtCore/QUrl>
#include <QtCore/QObject>
#include <QtCore/QList>
//#include <QtCore/QByteArray>
//#include <QtCore/QBuffer>
#include <QtGui/QClipboard>
#include <QtGui/QGraphicsScene>
#include <QtGui/QImage>
#include <QtGui/QApplication>

#include "../../../Model/src/ScreenieScene.h"
#include "../../../Model/src/ScreenieModelInterface.h"
#include "../../../Model/src/Dao/ScreenieSceneSerializer.h"
#include "../../../Model/src/Dao/Xml/XmlScreenieSceneSerializer.h"
#include "../ExportImage.h"
#include "../ScreenieControl.h"
#include "../ScreenieGraphicsScene.h"
#include "MimeHelper.h"
#include "ScreenieMimeData.h"
#include "Clipboard.h"

class ClipboardPrivate
{
public:
    ClipboardPrivate(ScreenieControl &control)
        : screenieControl(control),
          exportImage(control.getScreenieScene(), control.getScreenieGraphicsScene())
    {}

    ScreenieControl &screenieControl;
    ExportImage exportImage;
};

// public

Clipboard::Clipboard(ScreenieControl &screenieControl, QObject *parent) :
    QObject(parent),
    d(new ClipboardPrivate(screenieControl))
{
    frenchConnection();
}

Clipboard::~Clipboard()
{
#ifdef DEBUG
    qDebug("Clipboard::~Clipboard(): called.");
#endif
    delete d;
}

bool Clipboard::hasData() const
{
    bool result;
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    result = MimeHelper::accept(mimeData, MimeHelper::Relaxed);
    return result;
}

// public slots

void Clipboard::cut()
{
    storeMimeData();
    QList<ScreenieModelInterface *> selectedItems = d->screenieControl.getScreenieScene().getSelectedModels();
    foreach (ScreenieModelInterface *selectedItem, selectedItems) {
        d->screenieControl.getScreenieScene().removeModel(selectedItem);
    }    
}

void Clipboard::copy()
{
    storeMimeData();
}

void Clipboard::paste()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
#ifdef DEBUG
    foreach(QString format, mimeData->formats()) {
        qDebug("Clipboard::paste: MIME: %s", qPrintable(format));
    }
#endif

    if (MimeHelper::accept(mimeData, MimeHelper::Relaxed)) {
        // in order of preference
        const ScreenieMimeData *screenieMimeData = qobject_cast<const ScreenieMimeData *>(mimeData);
        if (screenieMimeData != 0) {
            // inside the same application instance
            const QList<const ScreenieModelInterface *> copies = screenieMimeData->getScreenieModels();
            foreach (const ScreenieModelInterface *clipboardModel, copies) {
                ScreenieModelInterface *copy = clipboardModel->copy();
                d->screenieControl.getScreenieScene().addModel(copy);
            }
        } else if (mimeData->hasFormat(MimeHelper::ScreenieMimeType)) {
            // across application instance boundaries
            ScreenieSceneSerializer *screenieSceneSerializer = new XmlScreenieSceneSerializer();
            QByteArray data = mimeData->data(MimeHelper::ScreenieMimeType);
            ScreenieScene *clipboardScreenieScene = screenieSceneSerializer->deserialize(data);
            if (clipboardScreenieScene != 0) {
                const QList<ScreenieModelInterface *> copies = clipboardScreenieScene->getModels();
                foreach (const ScreenieModelInterface *clipboardModel, copies) {
                    ScreenieModelInterface *copy = clipboardModel->copy();
                    d->screenieControl.getScreenieScene().addModel(copy);
                }
                delete clipboardScreenieScene;
            }
        } else if (mimeData->hasImage()) {
            // from different image application
            QImage image = qvariant_cast<QImage>(mimeData->imageData());
            d->screenieControl.addImage(image);
        } else if (mimeData->hasUrls()) {
            // from different file application
            QList<QUrl> urls = mimeData->urls();
            foreach(QUrl url, urls) {
                QString filePath = url.toLocalFile();
                if (!filePath.isNull()) {
                    d->screenieControl.addImage(filePath);
                }
            }
        }
    }
}

// private

void Clipboard::frenchConnection()
{
    QClipboard *clipboard = QApplication::clipboard();
    connect(clipboard, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
}

void Clipboard::storeMimeData()
{
    QList<const ScreenieModelInterface *> copies;
    foreach (ScreenieModelInterface *screenieModel, d->screenieControl.getScreenieScene().getSelectedModels()) {
        ScreenieModelInterface *copy = screenieModel->copy();
        copies.append(copy);
    }
    if (copies.count() > 0) {
        QClipboard *clipboard = QApplication::clipboard();
        ScreenieMimeData *screenieMimeData = new ScreenieMimeData(copies);
        // Image data
        QImage image = d->exportImage.exportImage(ExportImage::Selected);
        screenieMimeData->setImageData(image);
        ScreenieSceneSerializer *screenieSceneSerializer = new XmlScreenieSceneSerializer();
        // Serialized XML data
        QByteArray data = screenieSceneSerializer->serialize(d->screenieControl.getScreenieScene(), ScreenieSceneSerializer::SelectedItems);
        screenieMimeData->setData(MimeHelper::ScreenieMimeType, data);
        screenieMimeData->setData(MimeHelper::XmlMimeType, data);
        clipboard->setMimeData(screenieMimeData);
    }
}


