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

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QtCore/QString>

#include "UtilsLib.h"

class FileUtils
{ 
public:

    UTILS_API static const QString SceneExtension;
    UTILS_API static const QString TemplateExtension;

    UTILS_API QString static getOpenImageFileFilter();
    UTILS_API QString static getSaveImageFileFilter();
};

#endif // FILEUTILS_H

