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

#ifndef PROPERTYDIALOGFACTORY_H
#define PROPERTYDIALOGFACTORY_H

#include <QtCore/QObject>

class QDialog;
class QWidget;

#include "KernelLib.h"

class ScreenieModelInterface;
class ScreenieControl;
class PropertyDialogFactoryPrivate;

/*!
 * Creates property dialogs for the given ScreenieModelInterface.
 */
class PropertyDialogFactory : public QObject
{
    Q_OBJECT
public:
    PropertyDialogFactory(ScreenieControl &screenieControl, QObject *parent = 0);
    virtual ~PropertyDialogFactory();

    /*!
     * Creates a property dialog for the given \p screenieModel. The dialog has the flag \c Qt::WA_DeleteOnClose
     * set.
     *
     * \param screenieModel
     *        the ScreenieModelInterface for which a property dialog is to be created
     * \param parent
     *        the parent QWidget
     * \return a QDialog for the given \p screenieModel; can be \c deleted by the
     *         caller; has \c Qt::WA_DeleteOnClose set; may be 0 (no properties)
     */
    QDialog *createDialog(ScreenieModelInterface &screenieModel, QWidget *parent = 0);

private:
    PropertyDialogFactoryPrivate *d;

private slots:
    void handlePropertyDialogDestroyed();
};

#endif // PROPERTYDIALOGFACTORY_H
