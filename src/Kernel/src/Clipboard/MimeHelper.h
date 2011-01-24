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

#ifndef MIMEHELPER_H
#define MIMEHELPER_H

#include <QtCore/QString>

class QMimeData;

/*!
 * Operations on MIME data.
 */
class MimeHelper
{
public:
    /*!
     * The MIME type for the application data.
     */
    static const QString ScreenieMimeType;

    /*!
     * The MIME type for generic XML data: 'text/xml'
     */
    static const QString XmlMimeType;

    /*!
     * The MIME type for plain text: 'text/plain'
     */
    static const QString TextMimeType;

    /*!
     * The accept mode for mime data which defines when a given
     * data is accepted.
     *
     * \sa #accept(const QMimeData *, Mode)
     */
    enum Mode {
        Strict, /*!< Every Url must be a valid Url */
        Relaxed /*!< At least one Url must be a valid Url */
    };

    MimeHelper();

    /*!
     * Returns whether the \p mimeData should be accepted during a
     * drag'n'drop operation.
     *
     * Implementation note: currently only local file paths (file://) are accepted
     *
     * \param mimeData
     *        the QMimeData to be validated
     * \param mode
     *        in case of Urls defines whether at least one or all Urls
     *        must be valid
     * \return \c true, if the \p mimeData contains either an image or
     *          valid Urls according to \p mode
     */
    static bool accept(const QMimeData *mimeData, Mode mode);
};

#endif // MIMEHELPER_H
