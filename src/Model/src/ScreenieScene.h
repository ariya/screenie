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

#ifndef SCREENIESCENE_H
#define SCREENIESCENE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtGui/QPixmap>
#include <QtGui/QColor>

#include "ModelLib.h"

class ScreenieModelInterface;
class ScreenieScenePrivate;

/*!
 * The ScreenieScene model which contains all the instances of the ScreenieModelInterface.
 */
class ScreenieScene : public QObject
{
    Q_OBJECT

public:
    explicit ScreenieScene(QObject *parent = 0);
    virtual ~ScreenieScene();

    /*!
     * Adds the \p screenieModel to this ScreenieScene. This ScreenieScene takes ownership and
     * the \p screenieModel must have been allocated on the heap.
     *
     * \param screenieModel
     *        the ScreenieModel to be added
     * \sa #modelAdded(ScreenieModel &)
     * \sa #changed()
     */
    MODEL_API void addModel(ScreenieModelInterface *screenieModel);

    /*!
     * \sa #modelRemoved(const ScreenieModelInterface &)
     * \sa #changed()
     */
    MODEL_API void removeModel(ScreenieModelInterface *screenieModel);

    /*!
     * \sa #modelRemoved(const ScreenieModelInterface &)
     * \sa #changed()
     */
    MODEL_API void removeModel(int index);
    MODEL_API int count() const;

    MODEL_API bool isBackgroundEnabled() const;

    /*!
     * \sa #backgroundChanged()
     * \sa #changed()
     */
    MODEL_API void setBackgroundEnabled(bool enable);

    MODEL_API QColor getBackgroundColor() const;

    /*!
     * \sa #backgroundChanged()
     * \sa #changed()
     */
    MODEL_API void setBackgroundColor(QColor colour);

signals:
    /*!
     * Emitted whenever this ScreenieScene or one of the instances of the ScreenieModelInterface has changed.
     * This signal is emitted for <em>every</em> kind of change. Receivers should only do very cheap operations
     * when receiving this signal, such as updating widget states.
     *
     * For more costly operations connect to one of the specialised signals, either on this ScreenieScene, or
     * on the ScreenieModelInterface instance directly
     *
     * \sa ScreenieModelInterface#reflectionChanged()
     * \sa ScreenieModelInterface#changed()
     * \sa #modelAdded(ScreenieModelInterface &)
     * \sa #modelRemoved(ScreenieModelInterface &)
     * \sa #backgroundChanged()
     * \sa #distanceChanged
     */
    void changed();

    void modelAdded(ScreenieModelInterface &screenieModel);

    /*!
     * Emitted whenever the \p screenieModel has been removed from the scene. The \p screenieModel instance will be
     * \c deleted right after the signal has been emitted, so don't store and re-use the reference later on.
     *
     * This signal is emitted <em>in addition</em> to the #changed() signal.
     * Connect to this signal in order to remove the corresponding view items from the view, which refer to this
     * \p screenieModel, for example.
     */
    void modelRemoved(const ScreenieModelInterface &screenieModel);
    void backgroundChanged();
    void distanceChanged();

private:
    ScreenieScenePrivate *d;
};

#endif // SCREENIESCENE_H
