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

#ifndef REFLECTIONPROPERTIESWIDGET_H
#define REFLECTIONPROPERTIESWIDGET_H

#include <QtGui/QWidget>

#include "PropertyValidatorWidget.h"

class ScreenieModelInterface;
class ScreenieControl;
class ReflectionPropertiesWidgetPrivate;

namespace Ui {
    class ReflectionPropertiesWidget;
}

class ReflectionPropertiesWidget : public PropertyValidatorWidget
{
    Q_OBJECT
public:
    ReflectionPropertiesWidget(ScreenieModelInterface &screenieModel, ScreenieControl &screenieControl, QWidget *parent = 0);
    virtual ~ReflectionPropertiesWidget();

private:
    Ui::ReflectionPropertiesWidget *ui;
    ReflectionPropertiesWidgetPrivate *d;

    void initializeUi();
    void frenchConnection();

private slots:
    void updateUi();

    void on_reflectionCheckBox_toggled(bool checked);
    void on_offsetSlider_valueChanged(int value);
    void on_offsetLineEdit_editingFinished();
    void on_opacitySlider_valueChanged(int value);
    void on_opacityLineEdit_editingFinished();
};

#endif // REFLECTIONPROPERTIESWIDGET_H
