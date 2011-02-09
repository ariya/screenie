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

#ifndef DOCUMENTINFO_H
#define DOCUMENTINFO_H

#include <QtCore/QString>

class QMainWindow;
class ScreenieScene;

struct DocumentInfo
{
    /*!
     * Defines how to deal with unsaved documents before closing the window.
     */
    enum SaveStrategy
    {
        Discard, /*!< Discard the modifications, close the document immediatelly */
        Save, /*!< Save the modifications before closing the document */
        Ask /*!< Ask the user whether to discard or save the modifications before closing the document */
    };

    int id;
    QMainWindow *mainWindow;
    ScreenieScene *screenieScene;
    QString windowTitle;
    SaveStrategy saveStrategy;
};

#endif // DOCUMENTINFO_H
