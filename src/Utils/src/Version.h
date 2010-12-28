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

#ifndef VERSION_H
#define VERSION_H

#include <QtCore/QString>
#include <QtCore/QDate>

#include "UtilsLib.h"

class VersionPrivate;

/*!
 * Application version: [major].[minor].[subminor]
 */
class Version
{
public:
    /*!
     * Creates this Version with the current values of the application version.
     */
    UTILS_API Version();

    /*!
     * Creates this Version with \p major, \p minor and \p subMinor values. This
     * Version can then be compared against the application version, for example.
     *
     * \param major
     *        major version number
     * \param minor
     *        minor version number
     * \param subMinor
     *        sub-minor version number
     */
    UTILS_API Version(int major, int minor, int subMinor);

    /*!
     * Creates this Version by parsing the \p version string value.
     *
     * \param version
     *        a QString containing the version of the expected [major].[minor].[subminor]
     */
    UTILS_API Version(const QString &version);
    UTILS_API ~Version();

    /*!
     * Returns the major version number.
     *
     * \return the major version number
     */
    UTILS_API static int getMajor();

    /*!
     * Returns the minor version number.
     *
     * \return the minor version number
     */
    UTILS_API static int getMinor();

    /*!
     * Returns the subminor version number.
     *
     * \return the subminor version number
     */
    UTILS_API static int getSubMinor();

    /*!
     * Returns a user-friendly version string.
     *
     * \return a QString containing a user-friendly version value
     */
    UTILS_API QString toString();

    UTILS_API bool operator==(const Version &other);

    UTILS_API bool operator>=(const Version &other);

    UTILS_API bool operator<(const Version &other);

    /*!
     * A cool code name - every application needs this ;)
     *
     * \return a QString containing a cool code name
     */
    UTILS_API static QString getCodeName();

    /*!
     * Returns the application name which can be displayed in dialog captions.
     *
     * \return a QString containing the application name
     */
    UTILS_API static QString getApplicationName();


private:
    VersionPrivate *d;
};

#endif // VERSION_H
