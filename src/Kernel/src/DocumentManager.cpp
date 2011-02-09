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
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QEvent>
#include <QtCore/QSignalMapper>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>

#include "../../Model/src/ScreenieScene.h"
#include "DocumentInfo.h"
#include "DocumentManager.h"

class DocumentManagerPrivate
{
public:
    DocumentManagerPrivate()
        : windowActionGroup(new QActionGroup(0))
    {
        windowActionGroup->setExclusive(true);
    }

    ~DocumentManagerPrivate()
    {
        delete windowActionGroup;
    }

    QList<DocumentInfo *> documentInfos;
    QActionGroup *windowActionGroup;
    QSignalMapper windowMapper;
    static DocumentManager *instance;
    static int nextWindowId;
    static DocumentManager::CloseRequest closeRequest;
};

DocumentManager *DocumentManagerPrivate::instance = 0;
int DocumentManagerPrivate::nextWindowId = 0;
DocumentManager::CloseRequest DocumentManagerPrivate::closeRequest = DocumentManager::CloseCurrent;

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

void DocumentManager::add(DocumentInfo *documentInfo)
{
    QMainWindow *mainWindow = documentInfo->mainWindow;
    d->documentInfos.append(documentInfo);
    connect(mainWindow, SIGNAL(destroyed(QObject *)),
            this, SLOT(remove(QObject *)));
    // update object name ("window ID")
    mainWindow->setObjectName(mainWindow->objectName() + QString::number(d->nextWindowId));
    mainWindow->installEventFilter(this);
    /*!\todo This gets messy: make DocumentInfo a proper class. */
    documentInfo->id = d->nextWindowId;
    ++d->nextWindowId;
    documentInfo->windowTitle = tr("New %1", "New document title + ID").arg(d->nextWindowId);
    QAction *action = new QAction(d->windowActionGroup);
    action->setCheckable(true);
    action->setData(documentInfo->id);
    action->setText(documentInfo->windowTitle);
    d->windowMapper.setMapping(action, documentInfo->id);
    connect(action, SIGNAL(triggered()),
            &d->windowMapper, SLOT(map()));
    emit changed();
}

QString DocumentManager::getWindowTitle(const QMainWindow &mainWindow) const
{
    QString result;
    DocumentInfo *documentInfo = getDocumentInfo(mainWindow);
    if (documentInfo != 0) {
        result = documentInfo->windowTitle;
    }
    return result;
}

void DocumentManager::setWindowTitle(const QString &windowTitle, const QMainWindow &mainWindow)
{
    DocumentInfo *documentInfo = getDocumentInfo(mainWindow);
    if (documentInfo != 0) {
        documentInfo->windowTitle = windowTitle;
        QAction *action = getWindowAction(documentInfo->id);
        if (action != 0) {
            action->setText(windowTitle);
        }
    }
}

QActionGroup &DocumentManager::getActionGroup() const
{
    return *d->windowActionGroup;
}

int DocumentManager::count() const
{
    return d->documentInfos.count();
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

DocumentInfo::SaveStrategy DocumentManager::getSaveStrategy(const QMainWindow &mainWindow) const
{
    DocumentInfo::SaveStrategy result;
    const DocumentInfo *documentInfo = getDocumentInfo(mainWindow);
    if (documentInfo != 0) {
        result = documentInfo->saveStrategy;
    } else {
        result = DocumentInfo::Discard;
    }
    return result;
}

void DocumentManager::setSaveStrategy(const QMainWindow &mainWindow, DocumentInfo::SaveStrategy saveStrategy)
{
    DocumentInfo *documentInfo = getDocumentInfo(mainWindow);
    if (documentInfo != 0) {
        documentInfo->saveStrategy = saveStrategy;
    }
}

void DocumentManager::setSaveStrategyForAll(DocumentInfo::SaveStrategy saveStrategy)
{
    foreach (DocumentInfo *documentInfo, d->documentInfos) {
       documentInfo->saveStrategy = saveStrategy;
    }
}

DocumentManager::CloseRequest DocumentManager::getCloseRequest()
{
    return DocumentManagerPrivate::closeRequest;
}

void DocumentManager::setCloseRequest(CloseRequest closeRequest)
{
    DocumentManagerPrivate::closeRequest = closeRequest;
}

bool DocumentManager::eventFilter(QObject *object, QEvent *event)
{
    bool result;
    const QMainWindow *mainWindow = qobject_cast<const QMainWindow *>(object);
    if (mainWindow != 0) {
        switch (event->type()) {
        case QEvent::ActivationChange:
            updateActionGroup(*mainWindow);
            result = false;
            break;
        default:
            result = QObject::eventFilter(object, event);
            break;
        }
    } else {
        result = QObject::eventFilter(object, event);
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
    frenchConnection();
}

void DocumentManager::frenchConnection()
{
    connect(&d->windowMapper, SIGNAL(mapped(int)),
            this, SLOT(activate(int)));
}

void DocumentManager::updateActionGroup(const QMainWindow &mainWindow)
{
    const DocumentInfo *documentInfo = getDocumentInfo(mainWindow);
    if (documentInfo != 0) {
        QAction *action = getWindowAction(documentInfo->id);
        if (action != 0) {
            action->setChecked(mainWindow.isActiveWindow());
        }
    }
}

QAction *DocumentManager::getWindowAction(int id) const
{
    QAction *result = 0;
    foreach (QAction *action, d->windowActionGroup->actions()) {
        if (action->data().toInt() == id) {
            result = action;
            break;
        }
    }
    return result;
}

DocumentInfo *DocumentManager::getDocumentInfo(const QObject &object) const
{
    DocumentInfo *result = 0;
    foreach (DocumentInfo *documentInfo, d->documentInfos) {
        if (documentInfo->mainWindow->objectName() == object.objectName()) {
            result = documentInfo;
            break;
        }
    }
    return result;
}

// private slots

void DocumentManager::remove(QObject *object)
{
    DocumentInfo *documentInfo = getDocumentInfo(*object);
    if (documentInfo != 0) {
        foreach (QAction *action, d->windowActionGroup->actions()) {
            if (action->data().toInt() == documentInfo->id) {
                delete action;
                break;
            }
        }
        d->documentInfos.removeOne(documentInfo);
        emit changed();
    }
}

void DocumentManager::activate(int id) const
{
    foreach(DocumentInfo *documentInfo, d->documentInfos) {
        if (documentInfo->id == id) {
            documentInfo->mainWindow->activateWindow();
            documentInfo->mainWindow->raise();
            break;
        }
    }
}





