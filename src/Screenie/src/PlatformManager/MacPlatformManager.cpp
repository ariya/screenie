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

#include <QtGui/QAction>
#include <QtGui/QKeySequence>

#include "ui_MainWindow.h"
#include "MacPlatformManager.h"

class MacPlatformManagerPrivate
{
public:
    MacPlatformManagerPrivate(Ui::MainWindow &theMainWindowUi)
        : mainWindowUi(theMainWindowUi)
    {}

    Ui::MainWindow &mainWindowUi;
};

// public

MacPlatformManager::MacPlatformManager()
    :d(0)
{}

MacPlatformManager::~MacPlatformManager()
{
    if (d != 0) {
        delete d;
    }
}

void MacPlatformManager::initialize(QMainWindow &mainWindow, Ui::MainWindow &mainWindowUi)
{
    d = new MacPlatformManagerPrivate(mainWindowUi);
    AbstractPlatformManager::initialize(mainWindow, mainWindowUi);
    mainWindow.installEventFilter(this);
    mainWindowUi.toggleFullScreenAction->setShortcut(QKeySequence(Qt::Key_F + Qt::CTRL));
}

bool MacPlatformManager::eventFilter(QObject *object, QEvent *event)
{
    bool result;
    const QMainWindow *mainWindow = qobject_cast<const QMainWindow *>(object);
    if (mainWindow != 0) {
        switch (event->type()) {
        case QEvent::ActivationChange:
            handleWindowActivation(mainWindow->isActiveWindow());
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

// private

void MacPlatformManager::handleWindowActivation(bool active)
{
    if (active) {
        d->mainWindowUi.sidePanel->setStyleSheet("#sidePanel {background-color: rgb(218, 223, 230); border-right: 1px solid rgb(187, 187, 187);}");
    } else {
        d->mainWindowUi.sidePanel->setStyleSheet("#sidePanel {background-color: rgb(237, 237, 237); border-right: 1px solid rgb(187, 187, 187);}");
    }
}
