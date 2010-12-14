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

#ifndef SCREENIESCENEDAO_H
#define SCREENIESCENEDAO_H

class QString;

class ScreenieSceneDao
{
public:

    virtual ~ScreenieSceneDao() {}

    /*!
     * Stores the ScreenieScene into the file specified by \p filePath.
     *
     * \param filePath
     *        the path of the file to be written; existing files are overwritten
     * \return \c true if stored succesfully; \c false else (no file permission, disk full, write error)
     */
    virtual bool store(const QString &filePath) = 0;

    /*!
     * Restores the ScreenieScene from the file specified by \p filePath.
     *
     * \param filePath
     *        the path of the file to be read from
     * \return \c true if restored succesfully; \c false else (no file permission, read error, not a valid document)
     */
    virtual bool restore(const QString &filePath) = 0;
};

#endif // SCREENIESCENEDAO_H
