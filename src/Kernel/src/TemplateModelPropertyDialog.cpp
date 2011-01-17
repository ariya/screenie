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

#include <QtGui/QDialog>

#include "../../Model/src/ScreenieTemplateModel.h"
#include "TemplateModelPropertyDialog.h"
#include "ui_TemplateModelPropertyDialog.h"

class TemplateModelPropertyDialogPrivate
{
public:
    TemplateModelPropertyDialogPrivate(ScreenieTemplateModel &theScreenieTemplateModel)
        : screenieTemplateModel(theScreenieTemplateModel)
    {}

    ScreenieTemplateModel &screenieTemplateModel;
};

// public

TemplateModelPropertyDialog::TemplateModelPropertyDialog(ScreenieTemplateModel &screenieTemplateModel, QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent, flags),
    ui(new Ui::TemplateModelPropertyDialog),
    d(new TemplateModelPropertyDialogPrivate(screenieTemplateModel))
{
    ui->setupUi(this);
    initializeUi();
    updateUi();
    frenchConnection();
}

TemplateModelPropertyDialog::~TemplateModelPropertyDialog()
{
#ifdef DEBUG
    qDebug("TemplateModelPropertyDialog::~TemplateModelPropertyDialog(): called.");
#endif
    delete d;
    delete ui;
}

// private

void TemplateModelPropertyDialog::initializeUi()
{
    int width = d->screenieTemplateModel.getSize().width();
    int height = d->screenieTemplateModel.getSize().height();
    ui->widthLineEdit->setText(QString::number(width));
    ui->heightLineEdit->setText(QString::number(height));
}

void TemplateModelPropertyDialog::frenchConnection()
{

}

// private slots

void TemplateModelPropertyDialog::updateUi()
{

}


