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
#include <QtGui/QClipboard>
#include <QtGui/QGraphicsScene>
#include <QtGui/QImage>
#include <QtGui/QApplication>

#include "../../../Model/src/ScreenieScene.h"
#include "../../../Model/src/ScreenieModelInterface.h"
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
    QList<const ScreenieModelInterface *> copies;
    foreach (ScreenieModelInterface *screenieModel, d->screenieControl.getScreenieScene().getSelectedModels()) {
        ScreenieModelInterface *copy = screenieModel->copy();
        d->screenieControl.getScreenieScene().removeModel(screenieModel);
        copies.append(copy);
    }

    if (copies.count() > 0) {
        QClipboard *clipboard = QApplication::clipboard();
        ScreenieMimeData *screenieMimeData = new ScreenieMimeData(copies);
        clipboard->setMimeData(screenieMimeData);
    }
}

void Clipboard::copy()
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
        clipboard->setMimeData(screenieMimeData);
    }
}

void Clipboard::paste()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (MimeHelper::accept(mimeData, MimeHelper::Relaxed)) {
        if (mimeData->inherits(ScreenieMimeData::staticMetaObject.className())) {
            const ScreenieMimeData *screenieMimeData = static_cast<const ScreenieMimeData *>(mimeData);
            const QList<const ScreenieModelInterface *> copies = screenieMimeData->getScreenieModels();
            foreach (const ScreenieModelInterface *clipboardModel, copies) {
                ScreenieModelInterface *copy = clipboardModel->copy();
                d->screenieControl.getScreenieScene().addModel(copy);
            }
        } else if (mimeData->hasImage()) {
            QPixmap pixmap = qvariant_cast<QPixmap>(mimeData->imageData());
            d->screenieControl.addImage(pixmap);
        } else if (mimeData->hasUrls()) {
            QList<QUrl> urls = mimeData->urls();
            foreach(QUrl url, urls) {
                QString filePath = url.toLocalFile();
#ifdef DEBUG
                qDebug("Clipboard::paste: filepath: %s", qPrintable(filePath));
#endif
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


