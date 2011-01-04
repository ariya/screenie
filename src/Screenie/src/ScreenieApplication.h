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

#ifndef SCREENIEAPPLICATION_H
#define SCREENIEAPPLICATION_H

#include <QtCore/QStringList>
#include <QtGui/QApplication>

class QEvent;

class MainWindow;

/*!
 * The Screenie application which handles file open events.
 */
class ScreenieApplication : public QApplication
{
    Q_OBJECT
public:
    explicit ScreenieApplication(int argc, char **argv);

    /*!
     * Shows the MainWindow.
     */
    void show();
protected:
    /*!
     * Handles FileOpen events.
     *
     * Implementation note: Qt (4.7) implements FileOpen events currently only on Mac.
     */
    bool event(QEvent *event);

private:
    MainWindow *m_mainWindow;
    QStringList m_arguments;
};

#endif // SCREENIEAPPLICATION_H
