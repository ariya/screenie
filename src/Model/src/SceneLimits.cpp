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

#include <QtCore/QSize>

#include "SceneLimits.h"

const qreal SceneLimits::MinDistance = 0.0;
const qreal SceneLimits::MaxDistance = 200.0;
// size chosen as big as possible such that a generated pixmap
// does not blow up the RAM
const QSize SceneLimits::MaxTemplateSize = QSize(4096, 4096);
const int SceneLimits::MinReflectionOffset = 0;
const int SceneLimits::MaxReflectionOffset = 100;
const int SceneLimits::MinReflectionOpacity = 0;
const int SceneLimits::MaxReflectionOpacity = 100;
