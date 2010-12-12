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

#include <QtCore/QSize>

#include "Settings.h"

class SettingsPrivate
{
public:
    static Settings *instance;

    QSize maximumImageSize;

    SettingsPrivate() : maximumImageSize(QSize(640, 640)) {}
};

Settings *SettingsPrivate::instance = 0;

// public

Settings &Settings::getInstance()
{
    if (SettingsPrivate::instance == 0) {
        SettingsPrivate::instance = new Settings();
    }
    return *SettingsPrivate::instance;
}

void Settings::destroyInstance() {
    if (SettingsPrivate::instance != 0) {
        delete SettingsPrivate::instance;
        SettingsPrivate::instance = 0;
    }
}

QSize Settings::getMaximumImageSize() const {
    return d->maximumImageSize;
}

// protected

Settings::~Settings()
{
    delete d;
}

// private

Settings::Settings()
{
    d = new SettingsPrivate();
}
