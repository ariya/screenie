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

#ifndef DEFAULTSCREENIEMODEL_H
#define DEFAULTSCREENIEMODEL_H

#include "ModelLib.h"

class DefaultScreenieModelPrivate;

/*!
 * Provides default values for creating new instances of ScreenieModelInterface. The values
 * of the last selected item define the default values for the next item to be newly created.
 * The modified default values can be reset to the <em>default</em> \c Distance, \c Rotation,
 * \c ReflectionEnabled etc. values.
 */
class DefaultScreenieModel
{
public:
    static const qreal Distance;
    static const int Rotation;
    static const bool ReflectionEnabled;
    static const int ReflectionOffset;
    static const int ReflectionOpacity;

    MODEL_API DefaultScreenieModel();
    MODEL_API ~DefaultScreenieModel();

    MODEL_API qreal getDistance() const;
    MODEL_API void setDistance(qreal distance);

    MODEL_API int getRotation() const;
    MODEL_API void setRotation(int rotation);

    MODEL_API bool isReflectionEnabled() const;
    MODEL_API void setReflectionEnabled(bool enable);

    MODEL_API int getReflectionOffset() const;
    MODEL_API void setReflectionOffset(int reflectionOffset);

    MODEL_API int getReflectionOpacity() const;
    MODEL_API void setReflectionOpacity(int reflectionOpacity);

    /*!
     * Resets all values to the default values.
     */
    MODEL_API void reset();

private:
    DefaultScreenieModelPrivate *d;
};

#endif // DEFAULTSCREENIEMODEL_H
