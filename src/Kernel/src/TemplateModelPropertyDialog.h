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

#ifndef TEMPLATEMODELPROPERTYDIALOG_H
#define TEMPLATEMODELPROPERTYDIALOG_H

#include <QtGui/QDialog>

class ScreenieTemplateModel;
class TemplateModelPropertyDialogPrivate;

namespace Ui {
    class TemplateModelPropertyDialog;
}

class TemplateModelPropertyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TemplateModelPropertyDialog(ScreenieTemplateModel &templateModel, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~TemplateModelPropertyDialog();

private:
    Ui::TemplateModelPropertyDialog *ui;
    TemplateModelPropertyDialogPrivate *d;

    void initializeUi();
    void frenchConnection();

private slots:
    void updateUi();

    void on_widthLineEdit_editingFinished();
    void on_heightLineEdit_editingFinished();
    void on_positionXLineEdit_editingFinished();
    void on_positionYLineEdit_editingFinished();
    void on_rotationLineEdit_editingFinished();
    void on_distanceLineEdit_editingFinished();
    void on_fitModeComboBox_activated(int index);
    void on_respectOrientationCheckBox_toggled(bool checked);
    void on_enlargeCheckBox_toggled(bool checked);
};

#endif // TEMPLATEMODELPROPERTYDIALOG_H
