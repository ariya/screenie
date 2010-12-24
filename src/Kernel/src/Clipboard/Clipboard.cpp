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

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtGui/QClipboard>
#include <QtGui/QGraphicsScene>
#include <QtGui/QApplication>

#include "../../../Model/src/ScreenieScene.h"
#include "../../../Model/src/ScreenieModelInterface.h"
#include "ScreenieMimeData.h"
#include "Clipboard.h"

class ClipboardPrivate
{
public:
    ClipboardPrivate(const QGraphicsScene &theGraphicsScene, ScreenieScene &theScreenieScene)
        : graphicsScene(theGraphicsScene),
          screenieScene(theScreenieScene)
    {}

    const QGraphicsScene &graphicsScene;
    ScreenieScene &screenieScene;
};

// public

Clipboard::Clipboard(const QGraphicsScene &graphicsScene, ScreenieScene &screenieScene, QObject *parent) :
    QObject(parent),
    d(new ClipboardPrivate(graphicsScene, screenieScene))
{
    frenchConnection();
}

Clipboard::~Clipboard()
{
    delete d;
}

bool Clipboard::hasData() const
{
    bool result = false;
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData != 0) {
        if (mimeData->inherits(ScreenieMimeData::staticMetaObject.className())) {
            const ScreenieMimeData *screenieMimeData = static_cast<const ScreenieMimeData *>(mimeData);
            result = screenieMimeData->hasScreenieModels();
        }
    }
    return result;
}

// public slots

void Clipboard::cut()
{
    QList<const ScreenieModelInterface *> copies;
    foreach (ScreenieModelInterface *screenieModel, d->screenieScene.getSelectedModels()) {
        ScreenieModelInterface *copy = screenieModel->copy();
        d->screenieScene.removeModel(screenieModel);
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
    foreach (ScreenieModelInterface *screenieModel, d->screenieScene.getSelectedModels()) {
        ScreenieModelInterface *copy = screenieModel->copy();
        copies.append(copy);
    }
#ifdef DEBUG
    qDebug("Clipboard::copy: Copies count: %d", copies.count());
#endif
    if (copies.count() > 0) {
        QClipboard *clipboard = QApplication::clipboard();
        ScreenieMimeData *screenieMimeData = new ScreenieMimeData(copies);
        clipboard->setMimeData(screenieMimeData);
    }
}

void Clipboard::paste()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData != 0) {
        if (mimeData->inherits(ScreenieMimeData::staticMetaObject.className())) {
            const ScreenieMimeData *screenieMimeData = static_cast<const ScreenieMimeData *>(mimeData);
            const QList<const ScreenieModelInterface *> copies = screenieMimeData->getScreenieModels();
            foreach (const ScreenieModelInterface *clipboardModel, copies) {
                ScreenieModelInterface *copy = clipboardModel->copy();
                d->screenieScene.addModel(copy);
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


