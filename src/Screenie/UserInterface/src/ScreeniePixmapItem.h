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

#ifndef SCREENIEPIXMAPITEM_H
#define SCREENIEPIXMAPITEM_H

#include <QtGui/QGraphicsPixmapItem>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

class ScreenieModelInterface;
class ScreenieControl;
class Reflection;

/*!
 * The visual representation of a ScreenieModel in a QGraphicsView. Implements the
 * creation of the actual reflection.
 */
class ScreeniePixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    /*!
     * The type of this graphics item.
     */
    static const int ScreeniePixmapType;

    ScreeniePixmapItem(ScreenieModelInterface &screenieModel, ScreenieControl &screenieControl, Reflection &reflection);
    ~ScreeniePixmapItem();

    ScreenieModelInterface &getScreenieModel() const;

protected:
    virtual int type() const;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);

private:
    ScreenieModelInterface &m_screenieModel;
    ScreenieControl &m_screenieControl;
    Reflection &m_reflection;

    void frenchConnection();
    void rotate(int angle);
    void addDistance(int distance);

private slots:
    void updateReflection();
    void updateItem();
};

#endif // SCREENIEPIXMAPITEM_H
