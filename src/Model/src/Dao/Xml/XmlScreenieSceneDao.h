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

#ifndef XMLSCREENIESCENEDAO_H
#define XMLSCREENIESCENEDAO_H

class QString;

#include "../../ModelLib.h"
#include "../ScreenieSceneDao.h"

class ScreenieScene;
class ScreenieFilePathModel;
class ScreeniePixmapModel;
class XmlScreenieSceneDaoPrivate;

/*!
 * This Data Access Object (DAO) implements an XML persistence and
 * is the public class to use for reading and writing
 * ScreenieScene objects from/to a given file path.
 */
class XmlScreenieSceneDao : public ScreenieSceneDao
{
public:
    /*!\todo Use a QIODevice instead of filePath */
    MODEL_API explicit XmlScreenieSceneDao(const QString &filePath);
    MODEL_API virtual ~XmlScreenieSceneDao();

    MODEL_API virtual bool write(const ScreenieScene &screenieScene);
    MODEL_API virtual ScreenieScene *read() const;

private:
    XmlScreenieSceneDaoPrivate *d;

    bool writeScreenieScene(const ScreenieScene &screenieScene);
    bool writeScreenieModels(const ScreenieScene &screenieScene);
    bool writeFilePathModel(const ScreenieFilePathModel &screenieFilePathModel);
    bool writePixmapModel(const ScreeniePixmapModel &screeniePixmapModel);

    ScreenieScene *readScreenieScene() const;
    ScreenieFilePathModel *readFilePathModel() const;
    ScreeniePixmapModel *readPixmapModel() const;

    void cleanUp() const;
};

#endif // XMLSCREENIESCENEDAO_H
