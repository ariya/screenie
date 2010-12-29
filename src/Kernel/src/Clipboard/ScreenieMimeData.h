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

#ifndef SCREENIEMIMEDATA_H
#define SCREENIEMIMEDATA_H

#include <QtCore/QMimeData>
#include <QtCore/QList>

class ScreenieModelInterface;
class ScreenieMimeDataPrivate;

class ScreenieMimeData : public QMimeData
{
    Q_OBJECT
public:
    ScreenieMimeData();
    /*!
     * Constructs this ScreenieMimeData. Takes ownership over \p screenieModels
     * and its items.
     *
     * \param screenieModels
     *        a list containing the ScreenieModelInterface instances; ownership is taken over
     *        these instances
     */
    ScreenieMimeData(const QList<const ScreenieModelInterface *> screenieModels);
    virtual ~ScreenieMimeData();

    /*!
     * Returns the ScreenieModelInterface instances currently in this Clipboard. Ownership
     * remains with this Clipboard, hence make a copy of the instances if they are to
     * be inserted into a ScreenieScene.
     *
     * \return a list of the ScreenieModelInterface instances currently in the clipboard;
     *         an empty list else
     * \sa ScreenieModelInterface#copy()
     */
    const QList<const ScreenieModelInterface *> getScreenieModels() const;

    bool hasScreenieModels() const;

private:
    ScreenieMimeDataPrivate *d;
};

#endif // SCREENIEMIMEDATA_H
