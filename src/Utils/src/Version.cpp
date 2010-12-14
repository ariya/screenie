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

#include <QtCore/QString>
#include "Version.h"

class VersionPrivate
{
public:
    static const int Major;
    static const int Minor;
    static const int SubMinor;
    static const QString CodeName;
    static const QString ApplicationTitle;
};

const int VersionPrivate::Major = 0;
const int VersionPrivate::Minor = 1;
const int VersionPrivate::SubMinor = 0;
const QString VersionPrivate::CodeName = QString ("Anarchic Amoeba");
const QString VersionPrivate::ApplicationTitle = "Screenie";

// public

int Version::getMajor()
{
    return VersionPrivate::Major;
}

int Version::getMinor()
{
    return VersionPrivate::Minor;
}

int Version::getSubMinor()
{
    return VersionPrivate::SubMinor;
}

QString Version::getCodeName()
{
    return VersionPrivate::CodeName;
}

QString Version::getApplicationName()
{
    return VersionPrivate::ApplicationTitle;
}

QString Version::toString()
{
    return QString("%1.%2.%3").arg(VersionPrivate::Major).arg(VersionPrivate::Minor).arg(VersionPrivate::SubMinor);
}
