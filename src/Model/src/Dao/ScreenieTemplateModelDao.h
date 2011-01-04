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

#ifndef SCREENIETEMPLATEMODELDAO_H
#define SCREENIETEMPLATEMODELDAO_H

class ScreenieTemplateModel;

class ScreenieTemplateModelDao
{
public:
    virtual ~ScreenieTemplateModelDao() {}

    /*!
     * Writes the \p screeniePixmapModel.
     *
     * \param screenieTemplateModel
     *        the ScreenieTemplateModel to be written
     * \return \c true if written succesfully; \c false else (no file permission, disk full, write error)
     */
    virtual bool write(const ScreenieTemplateModel &screenieTemplateModel) = 0;

    /*!
     * Creates and reads the \em current ScreenieTemplateModel. The caller is then the owner
     * of the returned instance.
     *
     * \return the ScreenieTemplateModel read from the \em current position in the persistence stream;
     *         must be \c deleted by the caller; may be 0 on error
     */
    virtual ScreenieTemplateModel *read() = 0;
};

#endif // SCREENIETEMPLATEMODELDAO_H
