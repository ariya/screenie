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

#include "../UserInterface/src/MainWindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Resources);

    QApplication app(argc, argv);

    /* Load translation try from /usr/share/screenie, then ./l10n/screenie then ./ */
    QString locale = QLocale::system().name();
    QTranslator translator;
    if (translator.load(QString("screenie_") + locale, "/usr/share/screenie/") == false) {
      if (translator.load(QString("l10n/screenie_") + locale) == false) {
        translator.load(QString("screenie_") + locale);
      }
    }
    app.installTranslator(&translator);

    /* Load the system translator to get the Save dialog translated */
    QTranslator qtTranslator;
    qtTranslator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);
    MainWindow mainWindow;

    mainWindow.setWindowIcon(QIcon(":/img/ApplicationIcon.png"));
    mainWindow.show();
    return app.exec();
}

