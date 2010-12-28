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
#include <QtCore/QRegExp>
#include "Version.h"

class VersionPrivate
{
public:
    VersionPrivate()
        : major(Major), minor(Minor), subMinor(SubMinor)
    {}

    VersionPrivate(int theMajor, int theMinor, int theSubMinor)
        : major(theMajor), minor(theMinor), subMinor(theSubMinor)
    {}

    int major;
    int minor;
    int subMinor;

    static const int Major;
    static const int Minor;
    static const int SubMinor;
    static const QString CodeName;
    static const QString ApplicationTitle;
};

const int VersionPrivate::Major = 0;
const int VersionPrivate::Minor = 1;
const int VersionPrivate::SubMinor = 0;
const QString VersionPrivate::CodeName = QString("Anarchic Amoeba");
const QString VersionPrivate::ApplicationTitle = "Screenie"; // note: no translation here (i18n)

// public

Version::Version()
    : d(new VersionPrivate())
{

}

Version::Version(int major, int minor, int subMinor)
    : d(new VersionPrivate(major, minor, subMinor))
{

}

Version::Version(const QString &version)
    : d(new VersionPrivate())
{
    QRegExp versionRegExp("^(\\d+)\\.(\\d+)\\.(\\d+)$");
    if (versionRegExp.indexIn(version) != -1) {
        d->major = versionRegExp.cap(1).toInt();
        d->minor = versionRegExp.cap(2).toInt();
        d->subMinor = versionRegExp.cap(3).toInt();
    }
}

Version::~Version()
{
    delete d;
}

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

QString Version::toString()
{
    return QString("%1.%2.%3").arg(d->major).arg(d->minor).arg(d->subMinor);
}

bool Version::operator==(const Version &other)
{
    bool result;
    result = d->major == other.d->major && d->minor == other.d->minor && d->subMinor == other.d->subMinor;
    return result;
}

bool Version::operator>=(const Version &other)
{
    bool result;
    if (d->major > other.d->major) {
        result = true;
    } else if (d->major < other.d->major) {
        result = false;
    } else {
        if (d->minor > other.d->minor) {
            result = true;
        } else if (d->minor < other.d->minor) {
            result = false;
        } else {
            if (d->subMinor >= other.d->subMinor) {
                result = true;
            } else {
                result = false;
            }
        }
    }
    return result;
}

bool Version::operator<(const Version &other)
{
    return !(*this >= other);
}

QString Version::getCodeName()
{
    return VersionPrivate::CodeName;
}

QString Version::getApplicationName()
{
    return VersionPrivate::ApplicationTitle;
}
