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

#include <QtGui/QPalette>
#include <QtGui/QWidget>

#include "PropertyValidatorWidget.h"

class PropertyValidatorWidgetPrivate
{
public:
    PropertyValidatorWidgetPrivate()
    {
        validPalette.setColor(QPalette::Text, Qt::black);
        invalidPalette.setColor(QPalette::Text, Qt::red);
    }

    QPalette validPalette;
    QPalette invalidPalette;
};

// public

PropertyValidatorWidget::PropertyValidatorWidget(QWidget *parent) :
    QWidget(parent),
    d(new PropertyValidatorWidgetPrivate())
{
}

PropertyValidatorWidget::~PropertyValidatorWidget()
{
    delete d;
}

void PropertyValidatorWidget::validate(QWidget &widget, bool valid)
{
    if (valid) {
        widget.setPalette(d->validPalette);
    } else {
        widget.setPalette(d->invalidPalette);
    }
}
