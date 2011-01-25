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

#include <QtGui/QTabWidget>

#include "../../../Model/src/ScreenieFilePathModel.h"
#include "../ScreenieControl.h"
#include "FilePathModelPropertiesWidget.h"
#include "ScreenieModelPropertiesDialog.h"
#include "ui_ScreenieModelPropertiesDialog.h"
#include "FilePathModelPropertiesDialog.h"

class FilePathModelPropertiesDialogPrivate
{
public:
    FilePathModelPropertiesDialogPrivate(ScreenieFilePathModel &filePathModel, ScreenieControl &theScreenieControl)
        : screenieFilePathModel(filePathModel),
          screenieControl(theScreenieControl)
    {}

    ScreenieFilePathModel &screenieFilePathModel;
    ScreenieControl &screenieControl;
};

// public

FilePathModelPropertiesDialog::FilePathModelPropertiesDialog(ScreenieFilePathModel &screenieFilePathModel, ScreenieControl &screenieControl, QWidget *parent, Qt::WindowFlags flags) :
    ScreenieModelPropertiesDialog(screenieFilePathModel, screenieControl, parent, flags),
    d(new FilePathModelPropertiesDialogPrivate(screenieFilePathModel, screenieControl))
{
    initializeUi();
}

FilePathModelPropertiesDialog::~FilePathModelPropertiesDialog()
{
#ifdef DEBUG
    qDebug("FilePathModelPropertiesDialog::~FilePathModelPropertiesDialog(): called.");
#endif
    delete d;
}

// protected

void FilePathModelPropertiesDialog::initializeUi()
{
    FilePathModelPropertiesWidget *filePathModelPropertiesWidget = new FilePathModelPropertiesWidget(d->screenieFilePathModel, d->screenieControl, this);
    ui->propertiesTabWidget->addTab(filePathModelPropertiesWidget, tr("&Filepath"));

    ScreenieModelPropertiesDialog::initializeUi();
}
