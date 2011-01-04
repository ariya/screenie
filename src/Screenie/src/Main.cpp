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

#include <QtCore/QTranslator>
#include <QtCore/QString>
#include <QtCore/QLocale>
#include <QtCore/QLibraryInfo>
#include <QtGui/QApplication>
#include <QtGui/QIcon>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Resources);

    // workaround for http://bugreports.qt.nokia.com/browse/QTBUG-15663: use
    // the "raster" paint engine on affected OSes (Mac and Linux, Qt 4.7.1).
    // Note that the command line argument -graphicssystem still takes precedence (which is good)
// #if defined Q_OS_MAC || defined Q_OS_LINUX
#ifdef Q_OS_LINUX
    /*!\todo File bug report on this in Qt issue tracker */
    // Doh! This uncovers another Qt bug, at least on Mac with Qt 4.7.1
    // (Linux with Qt 4.7.0 seems to work though): the selection borders in the
    // QGraphicsView are not always properly drawn/updated with multiple
    // selection (CTRL + left click): the first item is visually selected
    // properly, the 2nd not, the 3rd yes (also rendering the 2nd item
    // properply as selected, the 4th no, the 5th yes (again rendering all
    // selected items so far correct)... (note that the model itself is always
    // marked selected properly).
    //
    // So for now we live with the graphical artifact when rotating images,
    // which is less serious than broken selection.
    // Setting the QGraphicsView to OpenGL would probably also help (no artifacts
    // there either, selection is hopefully fine).
    QApplication::setGraphicsSystem("raster");
#endif
    QApplication app(argc, argv);

#ifdef Q_OS_MAC
    // Mac apps prefer not to have icons in menus
    QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    // Load translation try from /usr/share/screenie, then ./l10n/screenie then ./
    /*!\todo Move i18n stuff into separate Babelfish class */
    QString locale = QLocale::system().name();
    QTranslator translator;
    if (translator.load(QString("screenie_") + locale, "/usr/share/screenie/") == false) {
      if (translator.load(QString("l10n/screenie_") + locale) == false) {
        translator.load(QString("screenie_") + locale);
      }
    }
    app.installTranslator(&translator);

    // Load the system translator to get the Save dialog translated
    QTranslator qtTranslator;
    qtTranslator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}

