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

#include <QtCore/QString>
#include <QtGui/QWidget>
#include <QtGui/QDialog>
#include <QtGui/QTabWidget>

#include "../../../Utils/src/SizeFitter.h"
#include "../../../Model/src/ScreenieModelInterface.h"
#include "GeometryPropertiesWidget.h"
#include "ReflectionPropertiesWidget.h"
#include "ui_ScreenieModelPropertiesDialog.h"
#include "ScreenieModelPropertiesDialog.h"

// This could be a candidate class for applying the "Pimpl - Reloaded" pattern, as
// described here: http://www.heise.de/developer/artikel/C-Vor-und-Nachteile-des-d-Zeiger-Idioms-Teil-2-1136104.html
// (german article)
class ScreenieModelPropertiesDialogPrivate
{
public:
    ScreenieModelPropertiesDialogPrivate(ScreenieModelInterface &theScreenieModel)
        : screenieModel(theScreenieModel)
    {}

    ScreenieModelInterface &screenieModel;
};

// public

ScreenieModelPropertiesDialog::ScreenieModelPropertiesDialog(ScreenieModelInterface &screenieModel, QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent, flags),
    ui(new Ui::ScreenieModelPropertiesDialog),
    d(new ScreenieModelPropertiesDialogPrivate(screenieModel))
{
    ui->setupUi(this);
    frenchConnection();
}

ScreenieModelPropertiesDialog::~ScreenieModelPropertiesDialog()
{
#ifdef DEBUG
    qDebug("ScreenieModelPropertiesDialog::~ScreenieModelPropertiesDialog(): called.");
#endif
    delete d;
    delete ui;
}

// protected

void ScreenieModelPropertiesDialog::initializeUi()
{
    GeometryPropertiesWidget *geometryPropertiesWidget = new GeometryPropertiesWidget(d->screenieModel, this);
    ui->propertiesTabWidget->addTab(geometryPropertiesWidget, tr("&Geometry"));
    ReflectionPropertiesWidget *reflectionPropertiesWidget = new ReflectionPropertiesWidget(d->screenieModel, this);
    ui->propertiesTabWidget->addTab(reflectionPropertiesWidget, tr("&Reflection"));
}

// private

void ScreenieModelPropertiesDialog::frenchConnection()
{
    connect(&d->screenieModel, SIGNAL(destroyed()),
            this, SLOT(close()));
}





