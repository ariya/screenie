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

#ifndef EXPORTIMAGE_H
#define EXPORTIMAGE_H

class QGraphicsScene;
class QString;

class ScreenieScene;

class ExportImage
{
public:
    ExportImage(const ScreenieScene &screenieScene, QGraphicsScene &graphicsScene);

    void exportImage(const QString &filePath);

private:
    const ScreenieScene &m_screenieScene;
    QGraphicsScene &m_graphicsScene;
};

#endif // EXPORTIMAGE_H
