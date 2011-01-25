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

#ifndef SCREENIEMODELPROPERTIESDIALOG_H
#define SCREENIEMODELPROPERTIESDIALOG_H

#include <QtGui/QDialog>

class ScreenieModelInterface;
class ScreenieControl;
class ScreenieModelPropertiesDialogPrivate;

namespace Ui {
    class ScreenieModelPropertiesDialog;
}

/*!
 * Implements the base dialog for geometry and reflection properties.
 * Inherit from this class as to create specific property dialogs.
 */
class ScreenieModelPropertiesDialog : public QDialog
{
    Q_OBJECT
public:
    ScreenieModelPropertiesDialog(ScreenieModelInterface &screenieModel, ScreenieControl &screenieControl, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~ScreenieModelPropertiesDialog();

protected:
    Ui::ScreenieModelPropertiesDialog *ui;

    /*!
     * Adds the Geometry and Reflection properties to the propery
     * tab widget. Override this method and call it before or
     * after the custom implementation, as to add the new tab
     * after or before the Geometry and Reflection tabs.
     */
    virtual void initializeUi();

private:
    ScreenieModelPropertiesDialogPrivate *d;

    void frenchConnection();
};

#endif // SCREENIEMODELPROPERTIESDIALOG_H
