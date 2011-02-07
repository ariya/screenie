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

#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtCore/QString>

#include "../../../Utils/src/Version.h"
#include "ScreenieMimeData.h"
#include "MimeHelper.h"

#ifdef DEBUG
#include <QtCore/QStringList>
#endif

// public

const QString MimeHelper::ScreenieMimeType = QString("application/x-") + Version::getApplicationName().toLower();
const QString MimeHelper::XmlMimeType = QString("text/xml");
const QString MimeHelper::TextMimeType = QString("text/plain");

MimeHelper::MimeHelper()
{
}

bool MimeHelper::accept(const QMimeData *mimeData, Mode mode)
{
    bool result;
    if (mimeData != 0) {
        const ScreenieMimeData *screenieMimeData = qobject_cast<const ScreenieMimeData *>(mimeData);
        if (screenieMimeData != 0) {
            result = screenieMimeData->hasScreenieModels();
        }
        else if (mimeData->hasImage()) {
            result = true;
        } else if (mimeData->hasUrls()) {
            result = false;
            foreach(QUrl url, mimeData->urls()) {
                if (!url.toLocalFile().isEmpty()) {
                    result = true;
                    if (mode == Relaxed) {
                        break;
                    }
                } else if (mode == Strict) {
                    result = false;
                    break;
                }
            }
        } else {
            result = false;
        }
    } else {
        result = false;
    }
    return result;
}
