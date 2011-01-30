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

#include <QtCore/QList>
#include <QtGui/QMainWindow>

#include "../../Model/src/ScreenieScene.h"
#include "DocumentInfo.h"
#include "DocumentManager.h"

class DocumentManagerPrivate
{
public:
    DocumentManagerPrivate() {}
    QList<const DocumentInfo *> documentInfos;

    static DocumentManager *instance;
};

DocumentManager *DocumentManagerPrivate::instance = 0;

// public

DocumentManager &DocumentManager::getInstance()
{
    if (DocumentManagerPrivate::instance == 0) {
        DocumentManagerPrivate::instance = new DocumentManager();
    }
    return *DocumentManagerPrivate::instance;
}

void DocumentManager::destroyInstance()
{
    if (DocumentManagerPrivate::instance != 0) {
        delete DocumentManagerPrivate::instance;
        DocumentManagerPrivate::instance = 0;
    }
}

void DocumentManager::add(const DocumentInfo *documentInfo)
{
    d->documentInfos.append(documentInfo);
    connect(documentInfo->mainWindow, SIGNAL(destroyed(QObject*)),
            this, SLOT(remove(QObject*)));
}

const QList<const DocumentInfo *> &DocumentManager::getDocumentInfos() const
{
    return d->documentInfos;
}

int DocumentManager::count() const
{
    return d->documentInfos.count();
}

void DocumentManager::toFront(int id) const
{
    const DocumentInfo *documentInfo = d->documentInfos.at(id);
    if (documentInfo != 0) {
        documentInfo->mainWindow->show();
    }
}

int DocumentManager::getModifiedCount() const
{
    int result = 0;
    foreach (const DocumentInfo *documentInfo, d->documentInfos) {
        if (documentInfo->screenieScene->isModified()) {
            result++;
        }
    }
    return result;
}

// protected

DocumentManager::~DocumentManager()
{
    delete d;
}

// private

DocumentManager::DocumentManager()
    : d(new DocumentManagerPrivate())
{
}

// private slots:

void DocumentManager::remove(QObject *object)
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(object);
    if (mainWindow != 0) {
        foreach(const DocumentInfo *documentInfo, d->documentInfos) {
            if (documentInfo->mainWindow->winId() == mainWindow->winId()) {
                emit documentRemoved(*documentInfo);
                d->documentInfos.removeOne(documentInfo);
                break;
            }
        }
    }
}





