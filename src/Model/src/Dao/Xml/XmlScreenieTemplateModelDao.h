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

#ifndef XMLSCREENIETEMPLATEMODELDAO_H
#define XMLSCREENIETEMPLATEMODELDAO_H

#include <QtCore/QString>
#include <QtCore/QBitArray>

class QXmlStreamWriter;
class QXmlStreamReader;
class QIODevice;

#include "../ScreenieTemplateModelDao.h"
#include "AbstractXmlScreenieModelDao.h"

class ScreenieTemplateModel;
class SizeFitter;
class XmlScreenieTemplateModelDaoPrivate;

/*!
 * Internal class.
 */
class XmlScreenieTemplateModelDao : public AbstractXmlScreenieModelDao, public ScreenieTemplateModelDao
{
public:
    explicit XmlScreenieTemplateModelDao(QXmlStreamWriter &xmlStreamWriter);
    explicit XmlScreenieTemplateModelDao(QXmlStreamReader &xmlStreamReader);
    explicit XmlScreenieTemplateModelDao(QIODevice &device);
    virtual ~XmlScreenieTemplateModelDao();

    virtual bool write(const ScreenieTemplateModel &screenieTemplateModel);
    virtual ScreenieTemplateModel *read();

protected:
    virtual bool writeSpecific();
    virtual bool readSpecific();

private:
    XmlScreenieTemplateModelDaoPrivate *d;

    bool writeSizeFitter(const SizeFitter &sizeFitter);
    bool readSizeFitter(SizeFitter &sizeFitter);

    static QString serializeBitArray(const QBitArray &bitArray);
    static QBitArray deserializeBitArray(const QString &bitArrayString);
};

#endif // XMLSCREENIETEMPLATEMODELDAO_H
