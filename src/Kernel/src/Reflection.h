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

#ifndef REFLECTION_H
#define REFLECTION_H

class ReflectionPrivate;

#include <QtGui/QPixmap>

#include "KernelLib.h"

/*!
 * Glass reflection effect.
 */
class Reflection
{
public:
    KERNEL_API Reflection();
    KERNEL_API virtual ~Reflection();

    /*!
     * Creates a reflected version of the \p pixmap, along with gradient translucency
     * defined by \p opacity and \p offset.
     *
     * \param pixmap
     *        the QPixmap to which the reflection is to be added
     * \param opacity
     *        the opacity (translucency) of the reflection in percent [0, 100];
     *        0: fully translucent; 1: fully opaque
     * \param offset
     *        the offset of the gradient in percent [1, 100] of the \c pixmap's height
     */
    KERNEL_API QPixmap addReflection(const QPixmap &pixmap, int opacity, int offset);

private:
    ReflectionPrivate *d;

    QPixmap reflect(const QPixmap &pixmap, int offset);
};

#endif // PAINTTOOLS_H
