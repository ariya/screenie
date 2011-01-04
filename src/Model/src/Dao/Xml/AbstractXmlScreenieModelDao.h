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

#ifndef ABSTRACTXMLSCREENIEMODELDAO_H
#define ABSTRACTXMLSCREENIEMODELDAO_H

class QXmlStreamWriter;
class QXmlStreamReader;
class QIODevice;

class AbstractScreenieModel;
class AbstractXmlScreenieModelDaoPrivate;

/*!
 * Internal class.
 */
class AbstractXmlScreenieModelDao
{
public:
    explicit AbstractXmlScreenieModelDao(QIODevice &device);
    explicit AbstractXmlScreenieModelDao(QXmlStreamWriter &xmlStreamWriter);
    explicit AbstractXmlScreenieModelDao(QXmlStreamReader &xmlStreamReader);
    virtual ~AbstractXmlScreenieModelDao();

    bool write(const AbstractScreenieModel &screenieModel);
    bool read(AbstractScreenieModel &screenieModel);

protected:
    /*!
     * \return the QXmlStreamWriter; may be 0 if a \c device has been given in the
     *         c'tor (device cannot be opened)
     * \sa AbstractXmlScreenieModelDao(QIODevice &)
     */
    QXmlStreamWriter *getStreamWriter() const;

    /*!
     * \return the QXmlStreamReader; may be 0 if a \c device has been given in the
     *         c'tor (device cannot be opened)
     * \sa AbstractXmlScreenieModelDao(QIODevice &)
     */
    QXmlStreamReader *getStreamReader() const;

    virtual bool writeSpecific() = 0;
    virtual bool readSpecific() = 0;

private:
    AbstractXmlScreenieModelDaoPrivate *d;

    void cleanUp();
};

#endif // ABSTRACTXMLSCREENIEMODELDAO_H
