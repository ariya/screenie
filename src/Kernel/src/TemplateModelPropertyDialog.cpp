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

#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtGui/QDialog>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>

#include "../../Utils/src/SizeFitter.h"
#include "../../Model/src/ScreenieTemplateModel.h"
#include "TemplateModelPropertyDialog.h"
#include "ui_TemplateModelPropertyDialog.h"

class TemplateModelPropertyDialogPrivate
{
public:
    TemplateModelPropertyDialogPrivate(ScreenieTemplateModel &theScreenieTemplateModel)
        : screenieTemplateModel(theScreenieTemplateModel),
          ignoreUpdateSignals(false)
    {}

    ScreenieTemplateModel &screenieTemplateModel;
    bool ignoreUpdateSignals;
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
    QStringList items;
    items << tr("No Fit") << tr("Fit") << tr("Fit To Width") << tr("Fit To Height") << tr("Exact Fit");
    ui->fitModeComboBox->addItems(items);
}

void TemplateModelPropertyDialog::frenchConnection()
{
    connect(&d->screenieTemplateModel, SIGNAL(changed()),
            this, SLOT(updateUi()));
    connect(&d->screenieTemplateModel, SIGNAL(positionChanged()),
            this, SLOT(updateUi()));
    connect(&d->screenieTemplateModel, SIGNAL(distanceChanged()),
            this, SLOT(updateUi()));
    connect(&d->screenieTemplateModel, SIGNAL(destroyed()),
            this, SLOT(close()));
}

// private slots

void TemplateModelPropertyDialog::updateUi()
{
    if (!d->ignoreUpdateSignals) {
        int width = d->screenieTemplateModel.getSize().width();
        int height = d->screenieTemplateModel.getSize().height();
        ui->widthLineEdit->setText(QString::number(width));
        ui->heightLineEdit->setText(QString::number(height));
        qreal x = d->screenieTemplateModel.getPosition().x();
        qreal y = d->screenieTemplateModel.getPosition().y();
        qreal z = d->screenieTemplateModel.getDistance();
        ui->positionXLineEdit->setText(QString::number(x));
        ui->positionYLineEdit->setText(QString::number(y));
        ui->distanceLineEdit->setText(QString::number(z));
        int rotation = d->screenieTemplateModel.getRotation();
        ui->rotationLineEdit->setText(QString::number(rotation));
        SizeFitter &sizeFitter = d->screenieTemplateModel.getSizeFitter();
        switch (sizeFitter.getFitMode()) {
        case SizeFitter::NoFit:
            ui->fitModeComboBox->setCurrentIndex(0);
            break;
        case SizeFitter::Fit:
            ui->fitModeComboBox->setCurrentIndex(1);
            break;
        case SizeFitter::FitToWidth:
            ui->fitModeComboBox->setCurrentIndex(2);
            break;
        case SizeFitter::FitToHeight:
            ui->fitModeComboBox->setCurrentIndex(3);
            break;
        case SizeFitter::ExactFit:
            ui->fitModeComboBox->setCurrentIndex(4);
            break;
        default:
#ifdef DEBUG
            qWarning("TemplateModelPropertyDialog::updateUi: UNSUPPORTED FIT MODE: %d", d->screenieTemplateModel.getSizeFitter().getFitMode());
#endif
            break;
        }
        ui->respectOrientationCheckBox->setChecked(sizeFitter.isFitOptionEnabled(SizeFitter::RespectOrientation));
        ui->enlargeCheckBox->setChecked(sizeFitter.isFitOptionEnabled(SizeFitter::Enlarge));
    }
}

void TemplateModelPropertyDialog::on_widthLineEdit_editingFinished()
{
    /*!\todo Implement this */
}

void TemplateModelPropertyDialog::on_heightLineEdit_editingFinished()
{
    /*!\todo Implement this */
}

void TemplateModelPropertyDialog::on_positionXLineEdit_editingFinished()
{
    bool ok;
    qreal x = ui->positionXLineEdit->text().toFloat(&ok);
    if (ok) {
        d->screenieTemplateModel.setPositionX(x);
    }
}

void TemplateModelPropertyDialog::on_positionYLineEdit_editingFinished()
{
    bool ok;
    qreal y = ui->positionYLineEdit->text().toFloat(&ok);
    if (ok) {
        d->screenieTemplateModel.setPositionY(y);
    }
}

void TemplateModelPropertyDialog::on_rotationLineEdit_editingFinished()
{
    bool ok;
    int angle = ui->rotationLineEdit->text().toInt(&ok);
    if (ok) {
        d->screenieTemplateModel.setRotation(angle);
    }
}

void TemplateModelPropertyDialog::on_distanceLineEdit_editingFinished()
{
    bool ok;
    qreal distance = ui->distanceLineEdit->text().toFloat(&ok);
    if (ok) {
        d->screenieTemplateModel.setDistance(distance);
    }
}

void TemplateModelPropertyDialog::on_fitModeComboBox_activated(int index)
{
    SizeFitter &sizeFitter = d->screenieTemplateModel.getSizeFitter();
    switch (index) {
    case 0:
        sizeFitter.setFitMode(SizeFitter::NoFit);
        break;
    case 1:
        sizeFitter.setFitMode(SizeFitter::Fit);
        break;
    case 2:
        sizeFitter.setFitMode(SizeFitter::FitToWidth);
        break;
    case 3:
        sizeFitter.setFitMode(SizeFitter::FitToHeight);
        break;
    case 4:
        sizeFitter.setFitMode(SizeFitter::ExactFit);
        break;
    default:
#ifdef DEBUG
        qWarning("TemplateModelPropertyDialog::on_fitModeComboBox_activated: UNSUPPORTED FIT MODE: %d", index);
#endif
        break;
    }
}

void TemplateModelPropertyDialog::on_respectOrientationCheckBox_toggled(bool checked)
{
    SizeFitter &sizeFitter = d->screenieTemplateModel.getSizeFitter();
    sizeFitter.setFitOptionEnabled(SizeFitter::RespectOrientation, checked);
}

void TemplateModelPropertyDialog::on_enlargeCheckBox_toggled(bool checked)
{
    SizeFitter &sizeFitter = d->screenieTemplateModel.getSizeFitter();
    sizeFitter.setFitOptionEnabled(SizeFitter::Enlarge, checked);
}


