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
#include <QtCore/QStringList>
#include <QtCore/QLibraryInfo>
#include <QtGui/QIcon>
#include <QtCore/QEvent>
#include <QtGui/QFileOpenEvent>

#include "MainWindow.h"
#include "ScreenieApplication.h"

// public

ScreenieApplication::ScreenieApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
}

void ScreenieApplication::show()
{

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
    installTranslator(&translator);

    // Load the system translator to get the Save dialog translated
    QTranslator qtTranslator;
    qtTranslator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    installTranslator(&qtTranslator);

    m_mainWindow = new MainWindow();
    m_mainWindow->setAttribute(Qt::WA_DeleteOnClose, true);
    // simplistic command line parsing: first arg is assumed to be a file path
    QStringList args = arguments();
    if (args.count() > 1) {
        m_mainWindow->read(args.at(1));
    }
    m_mainWindow->show();
}

// protected

bool ScreenieApplication::event(QEvent *event)
{
    bool result;
    switch (event->type()) {
    case QEvent::FileOpen:
        result = true;
        m_mainWindow->read(static_cast<QFileOpenEvent *>(event)->file());
        event->accept();
        break;
    default:
        result = QApplication::event(event);
    }
    return result;
}
