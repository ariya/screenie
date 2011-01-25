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
#include <QtGui/QSlider>
#include <QtGui/QLineEdit>

#include "../../../Model/src/ScreenieModelInterface.h"
#include "../ScreenieControl.h"
#include "ReflectionPropertiesWidget.h"
#include "ui_ReflectionPropertiesWidget.h"

class ReflectionPropertiesWidgetPrivate
{
public:
    ReflectionPropertiesWidgetPrivate(ScreenieModelInterface &theScreenieModel, ScreenieControl &theScreenieControl)
        : screenieModel(theScreenieModel),
          screenieControl(theScreenieControl)
    {}

    ScreenieModelInterface &screenieModel;
    ScreenieControl &screenieControl;
};

// public

ReflectionPropertiesWidget::ReflectionPropertiesWidget(ScreenieModelInterface &screenieModel, ScreenieControl &screenieControl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReflectionPropertiesWidget),
    d(new ReflectionPropertiesWidgetPrivate(screenieModel, screenieControl))
{
    ui->setupUi(this);
    updateUi();
    frenchConnection();
}

ReflectionPropertiesWidget::~ReflectionPropertiesWidget()
{
#ifdef DEBUG
    qDebug("ReflectionPropertiesWidget::~ReflectionPropertiesWidget(): called.");
#endif
    delete ui;
    delete d;
}

// private

void ReflectionPropertiesWidget::frenchConnection()
{
    connect(&d->screenieModel, SIGNAL(reflectionChanged()),
            this, SLOT(updateUi()));
}

// private slots

void ReflectionPropertiesWidget::updateUi()
{
    bool enableReflection = d->screenieModel.isReflectionEnabled();
    ui->reflectionCheckBox->setChecked(enableReflection);
    int reflectionOffset = d->screenieModel.getReflectionOffset();
    ui->offsetLineEdit->setText(QString::number(reflectionOffset));
    ui->offsetLineEdit->setEnabled(enableReflection);
    ui->offsetSlider->setValue(reflectionOffset);
    ui->offsetSlider->setEnabled(enableReflection);
    int reflectionOpacity = d->screenieModel.getReflectionOpacity();
    ui->opacityLineEdit->setText(QString::number(reflectionOpacity));
    ui->opacityLineEdit->setEnabled(enableReflection);
    ui->opacitySlider->setValue(reflectionOpacity);
    ui->opacitySlider->setEnabled(enableReflection);
}

/*!\todo Use ScreenieControl to manipulate the model values! */
void ReflectionPropertiesWidget::on_reflectionCheckBox_toggled(bool checked)
{
    d->screenieModel.setReflectionEnabled(checked);
}

void ReflectionPropertiesWidget::on_offsetSlider_valueChanged(int value)
{
    d->screenieModel.setReflectionOffset(value);
}

void ReflectionPropertiesWidget::on_offsetLineEdit_editingFinished()
{
    bool ok;
    int offset = ui->offsetLineEdit->text().toInt(&ok);
    if (ok) {
        d->screenieModel.setReflectionOffset(offset);
    }
}

void ReflectionPropertiesWidget::on_opacitySlider_valueChanged(int value)
{
    d->screenieModel.setReflectionOpacity(value);
}

void ReflectionPropertiesWidget::on_opacityLineEdit_editingFinished()
{
    bool ok;
    int opacity = ui->opacityLineEdit->text().toInt(&ok);
    if (ok) {
        d->screenieModel.setReflectionOpacity(opacity);
    }
}

