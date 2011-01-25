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
#include <QtCore/QStringList>
#include <QtGui/QSlider>
#include <QtGui/QLineEdit>

#include "../../../Utils/src/SizeFitter.h"
#include "../../../Model/src/ScreenieTemplateModel.h"
#include "../ScreenieControl.h"
#include "TemplateModelPropertiesWidget.h"
#include "ui_TemplateModelPropertiesWidget.h"

class TemplateModelPropertiesWidgetPrivate
{
public:
    TemplateModelPropertiesWidgetPrivate(ScreenieTemplateModel &templateModel, ScreenieControl &theScreenieControl)
        : screenieTemplateModel(templateModel),
          screenieControl(theScreenieControl)
    {}

    ScreenieTemplateModel &screenieTemplateModel;
    ScreenieControl &screenieControl;
    bool ignoreUpdateSignals;
};

TemplateModelPropertiesWidget::TemplateModelPropertiesWidget(ScreenieTemplateModel &templateModel, ScreenieControl &screenieControl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemplateModelPropertiesWidget),
    d(new TemplateModelPropertiesWidgetPrivate(templateModel, screenieControl))
{
    ui->setupUi(this);
    initializeUi();
    updateUi();
    frenchConnection();
}

TemplateModelPropertiesWidget::~TemplateModelPropertiesWidget()
{
#ifdef DEBUG
    qDebug("TemplateModelPropertiesWidget::~TemplateModelPropertiesWidget(): called.");
#endif
    delete ui;
    delete d;
}

// private

void TemplateModelPropertiesWidget::initializeUi()
{
    QStringList items;
    items << tr("No Fit") << tr("Fit") << tr("Fit To Width") << tr("Fit To Height") << tr("Exact Fit");
    ui->fitModeComboBox->addItems(items);
}

void TemplateModelPropertiesWidget::frenchConnection()
{
    connect(&d->screenieTemplateModel, SIGNAL(changed()),
            this, SLOT(updateUi()));
}

// private slots

/*!\todo Use ScreenieControl to manipulate the model values! */
void TemplateModelPropertiesWidget::updateUi()
{
    int width = d->screenieTemplateModel.getSize().width();
    int height = d->screenieTemplateModel.getSize().height();
    ui->widthLineEdit->setText(QString::number(width));
    ui->heightLineEdit->setText(QString::number(height));
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
        qCritical("TemplateModelPropertiesDialog::updateUi: UNSUPPORTED FIT MODE: %d", d->screenieTemplateModel.getSizeFitter().getFitMode());
#endif
        break;
    }
    ui->respectOrientationCheckBox->setChecked(sizeFitter.isFitOptionEnabled(SizeFitter::RespectOrientation));
    ui->enlargeCheckBox->setChecked(sizeFitter.isFitOptionEnabled(SizeFitter::Enlarge));
    ui->idLineEdit->setText(QString::number(d->screenieTemplateModel.getOrder()));
}

void TemplateModelPropertiesWidget::on_widthLineEdit_editingFinished()
{
    bool ok;
    int width = ui->widthLineEdit->text().toInt(&ok);
    if (ok) {
        d->screenieTemplateModel.getSizeFitter().setTargetWidth(width);
    }
}

void TemplateModelPropertiesWidget::on_heightLineEdit_editingFinished()
{
    bool ok;
    int height = ui->heightLineEdit->text().toInt(&ok);
    if (ok) {
        d->screenieTemplateModel.getSizeFitter().setTargetHeight(height);
    }
}

void TemplateModelPropertiesWidget::on_fitModeComboBox_activated(int index)
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
        qCritical("TemplateModelPropertiesDialog::on_fitModeComboBox_activated: UNSUPPORTED FIT MODE: %d", index);
#endif
        break;
    }
}

void TemplateModelPropertiesWidget::on_respectOrientationCheckBox_toggled(bool checked)
{
    SizeFitter &sizeFitter = d->screenieTemplateModel.getSizeFitter();
    sizeFitter.setFitOptionEnabled(SizeFitter::RespectOrientation, checked);
}

void TemplateModelPropertiesWidget::on_enlargeCheckBox_toggled(bool checked)
{
    SizeFitter &sizeFitter = d->screenieTemplateModel.getSizeFitter();
    sizeFitter.setFitOptionEnabled(SizeFitter::Enlarge, checked);
}
