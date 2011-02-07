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
#include "AbstractPlatformManager.h"

// public

void AbstractPlatformManager::initialize(Ui::MainWindow &mainWindow)
{
    // File
    mainWindow.newAction->setShortcut(QKeySequence::New);
    mainWindow.openAction->setShortcut(QKeySequence::Open);
    mainWindow.saveAction->setShortcut(QKeySequence::Save);
    mainWindow.saveAsAction->setShortcut(QKeySequence::SaveAs);
    mainWindow.exportAction->setShortcut(QKeySequence(Qt::Key_E + Qt::CTRL));
    mainWindow.closeAction->setShortcut(QKeySequence::Close);
    mainWindow.quitAction->setShortcut(QKeySequence::Quit);
    // Edit
    mainWindow.cutAction->setShortcut(QKeySequence::Cut);
    mainWindow.copyAction->setShortcut(QKeySequence::Copy);
    mainWindow.pasteAction->setShortcut(QKeySequence::Paste);
    mainWindow.deleteAction->setShortcut(QKeySequence::Delete);
    mainWindow.selectAllAction->setShortcut(QKeySequence::SelectAll);
    // Insert
    mainWindow.addImageAction->setShortcut(QKeySequence(Qt::Key_I + Qt::CTRL));
    mainWindow.addTemplateAction->setShortcut(QKeySequence(Qt::Key_T + Qt::CTRL));
}
