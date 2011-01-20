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

#include "../../../Model/src/ScreenieModelInterface.h"
#include "BaseGeometryPropertiesWidget.h"
#include "ui_BaseGeometryPropertiesWidget.h"

class BaseGeometryPropertiesWidgetPrivate
{
public:
    BaseGeometryPropertiesWidgetPrivate(ScreenieModelInterface &theScreenieModel)
        : screenieModel(theScreenieModel)
    {}

    ScreenieModelInterface &screenieModel;
};

// public

BaseGeometryPropertiesWidget::BaseGeometryPropertiesWidget(ScreenieModelInterface &screenieModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseGeometryPropertiesWidget),
    d(new BaseGeometryPropertiesWidgetPrivate(screenieModel))
{
    ui->setupUi(this);
    updateUi();
    frenchConnection();
}

BaseGeometryPropertiesWidget::~BaseGeometryPropertiesWidget()
{
#ifdef DEBUG
    qDebug("BaseGeometryPropertiesWidget::~BaseGeometryPropertiesWidget(): called.");
#endif
    delete ui;
    delete d;
}

// private

void BaseGeometryPropertiesWidget::frenchConnection()
{
    connect(&d->screenieModel, SIGNAL(positionChanged()),
            this, SLOT(updateUi()));
    connect(&d->screenieModel, SIGNAL(rotationChanged()),
            this, SLOT(updateUi()));
    connect(&d->screenieModel, SIGNAL(distanceChanged()),
            this, SLOT(updateUi()));
}

// private slots

void BaseGeometryPropertiesWidget::updateUi()
{
    qreal x = d->screenieModel.getPosition().x();
    qreal y = d->screenieModel.getPosition().y();
    qreal z = d->screenieModel.getDistance();
    ui->positionXLineEdit->setText(QString::number(x));
    ui->positionYLineEdit->setText(QString::number(y));
    ui->distanceLineEdit->setText(QString::number(z));
    int rotation = d->screenieModel.getRotation();
    ui->rotationLineEdit->setText(QString::number(rotation));
}

void BaseGeometryPropertiesWidget::on_positionXLineEdit_editingFinished()
{
    bool ok;
    qreal x = ui->positionXLineEdit->text().toFloat(&ok);
    if (ok) {
        d->screenieModel.setPositionX(x);
    }
}

void BaseGeometryPropertiesWidget::on_positionYLineEdit_editingFinished()
{
    bool ok;
    qreal y = ui->positionYLineEdit->text().toFloat(&ok);
    if (ok) {
        d->screenieModel.setPositionY(y);
    }
}

void BaseGeometryPropertiesWidget::on_rotationLineEdit_editingFinished()
{
    bool ok;
    int angle = ui->rotationLineEdit->text().toInt(&ok);
    if (ok) {
        d->screenieModel.setRotation(angle);
    }
}

void BaseGeometryPropertiesWidget::on_distanceLineEdit_editingFinished()
{
    bool ok;
    qreal distance = ui->distanceLineEdit->text().toFloat(&ok);
    if (ok) {
        d->screenieModel.setDistance(distance);
    }
}
