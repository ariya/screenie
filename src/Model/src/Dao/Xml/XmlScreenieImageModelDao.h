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

#ifndef XMLSCREENIEIMAGEMODELDAO_H
#define XMLSCREENIEIMAGEMODELDAO_H

class QXmlStreamWriter;
class QXmlStreamReader;

#include "../ScreenieImageModelDao.h"
#include "AbstractXmlScreenieModelDao.h"

class ScreenieImageModel;
class XmlScreenieImageModelDaoPrivate;

/*!
 * Internal class.
 */
class XmlScreenieImageModelDao : public AbstractXmlScreenieModelDao, public ScreenieImageModelDao
{
public:
    explicit XmlScreenieImageModelDao(QXmlStreamWriter &xmlStreamWriter);
    explicit XmlScreenieImageModelDao(QXmlStreamReader &xmlStreamReader);
    virtual ~XmlScreenieImageModelDao();

    virtual bool write(const ScreenieImageModel &screeniePixmapModel);
    virtual ScreenieImageModel *read();

protected:
    virtual bool writeSpecific();
    virtual bool readSpecific();

private:
    XmlScreenieImageModelDaoPrivate *d;
};

#endif // XMLSCREENIEIMAGEMODELDAO_H
