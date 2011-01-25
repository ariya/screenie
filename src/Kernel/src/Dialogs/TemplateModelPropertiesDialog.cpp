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

#include "../../../Model/src/ScreenieTemplateModel.h"
#include "../ScreenieControl.h"
#include "TemplateModelPropertiesWidget.h"
#include "ScreenieModelPropertiesDialog.h"
#include "ui_ScreenieModelPropertiesDialog.h"
#include "TemplateModelPropertiesDialog.h"

class TemplateModelPropertiesDialogPrivate
{
public:
    TemplateModelPropertiesDialogPrivate(ScreenieTemplateModel &templateModel, ScreenieControl &theScreenieControl)
        : screenieTemplateModel(templateModel),
          screenieControl(theScreenieControl)
    {}

    ScreenieTemplateModel &screenieTemplateModel;
    ScreenieControl &screenieControl;
};

// public

TemplateModelPropertiesDialog::TemplateModelPropertiesDialog(ScreenieTemplateModel &screenieTemplateModel, ScreenieControl &screenieControl, QWidget *parent, Qt::WindowFlags flags) :
    ScreenieModelPropertiesDialog(screenieTemplateModel, screenieControl, parent, flags),
    d(new TemplateModelPropertiesDialogPrivate(screenieTemplateModel, screenieControl))
{
    initializeUi();
}

TemplateModelPropertiesDialog::~TemplateModelPropertiesDialog()
{
#ifdef DEBUG
    qDebug("TemplateModelPropertiesDialog::~TemplateModelPropertiesDialog(): called.");
#endif
    delete d;
}

// protected

void TemplateModelPropertiesDialog::initializeUi()
{
    TemplateModelPropertiesWidget *templateModelPropertiesWidget = new TemplateModelPropertiesWidget(d->screenieTemplateModel, d->screenieControl, this);
    ui->propertiesTabWidget->addTab(templateModelPropertiesWidget, tr("&Template"));

    ScreenieModelPropertiesDialog::initializeUi();
}


