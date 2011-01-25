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

#include "../../../Model/src/ScreenieFilePathModel.h"
#include "../ScreenieControl.h"
#include "FilePathModelPropertiesWidget.h"
#include "ui_FilePathModelPropertiesWidget.h"

class FilePathModelPropertiesWidgetPrivate
{
public:
    FilePathModelPropertiesWidgetPrivate(ScreenieFilePathModel &filePathModel, ScreenieControl &theScreenieControl)
        : screenieFilePathModel(filePathModel),
          screenieControl(theScreenieControl)
    {}

    ScreenieFilePathModel &screenieFilePathModel;
    ScreenieControl &screenieControl;
};

// public

FilePathModelPropertiesWidget::FilePathModelPropertiesWidget(ScreenieFilePathModel &filePathModel, ScreenieControl &screenieControl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilePathModelPropertiesWidget),
    d(new FilePathModelPropertiesWidgetPrivate(filePathModel, screenieControl))
{
    ui->setupUi(this);
}

FilePathModelPropertiesWidget::~FilePathModelPropertiesWidget()
{
#ifdef DEBUG
    qDebug("FilePathModelPropertiesWidget::~FilePathModelPropertiesWidget(): called.");
#endif
    delete ui;
    delete d;
}
