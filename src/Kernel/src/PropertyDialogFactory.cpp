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
#include <QtGui/QDialog>
#include <QtGui/QIcon>

#include "../../Model/src/ScreenieModelInterface.h"
#include "../../Model/src/ScreenieFilePathModel.h"
#include "../../Model/src/ScreenieImageModel.h"
#include "../../Model/src/ScreenieTemplateModel.h"
#include "Dialogs/TemplateModelPropertiesDialog.h"
#include "Dialogs/FilePathModelPropertiesDialog.h"
#include "Dialogs/ImageModelPropertiesDialog.h"
#include "PropertyDialogFactory.h"

class PropertyDialogFactoryPrivate
{
public:
    PropertyDialogFactoryPrivate(ScreenieControl &theScreenieControl)
        : screenieControl(theScreenieControl)
    {}

    ScreenieControl &screenieControl;
    static QDialog *lastDialog;
};

QDialog *PropertyDialogFactoryPrivate::lastDialog = 0;

// public

PropertyDialogFactory::PropertyDialogFactory(ScreenieControl &screenieControl, QObject *parent)
    : QObject(parent),
      d(new PropertyDialogFactoryPrivate(screenieControl))
{
}

PropertyDialogFactory::~PropertyDialogFactory()
{
    delete d;
}

QDialog *PropertyDialogFactory::createDialog(ScreenieModelInterface &screenieModel, QWidget *parent)
{
    QDialog *result = 0;
    if (screenieModel.inherits(ScreenieTemplateModel::staticMetaObject.className())) {
        ScreenieTemplateModel &screenieTemplateModel = static_cast<ScreenieTemplateModel &>(screenieModel);
        result = new TemplateModelPropertiesDialog(screenieTemplateModel, d->screenieControl, parent, Qt::WindowStaysOnTopHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
        result->setWindowTitle(QObject::tr("Template Properties"));
    } else if (screenieModel.inherits(ScreenieFilePathModel::staticMetaObject.className())) {
        ScreenieFilePathModel &screenieFilePathModel = static_cast<ScreenieFilePathModel &>(screenieModel);
        result = new FilePathModelPropertiesDialog(screenieFilePathModel, d->screenieControl, parent, Qt::WindowStaysOnTopHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
        result->setWindowTitle(QObject::tr("Image Properties"));
    } else if (screenieModel.inherits(ScreenieImageModel::staticMetaObject.className())) {
        ScreenieImageModel &screenieImageModel = static_cast<ScreenieImageModel &>(screenieModel);
        result = new ImageModelPropertiesDialog(screenieImageModel, d->screenieControl, parent, Qt::WindowStaysOnTopHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
        result->setWindowTitle(QObject::tr("Image Properties"));
    }
    if (result != 0) {
        connect(result, SIGNAL(destroyed()),
                this, SLOT(handlePropertyDialogDestroyed()));
        result->setAttribute(Qt::WA_DeleteOnClose);
        result->setWindowIcon(QIcon(":/img/application-icon.png"));
        if (PropertyDialogFactoryPrivate::lastDialog != 0) {
            delete PropertyDialogFactoryPrivate::lastDialog;
        }
        PropertyDialogFactoryPrivate::lastDialog = result;
    }
    return result;
}

// private slots

void PropertyDialogFactory::handlePropertyDialogDestroyed(){
    PropertyDialogFactoryPrivate::lastDialog = 0;
}

