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

#include <QtCore/QPointF>
#include <QtGui/QGraphicsPixmapItem>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneDragDropEvent;

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
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

private:
    ScreenieModelInterface &m_screenieModel;
    ScreenieControl &m_screenieControl;
    Reflection &m_reflection;
    bool m_transformPixmap;

    void frenchConnection();
    void moveTo(QPointF scenePosition);
    void rotate(int angle);
    void addDistance(int distance);
    bool isInsidePixmap(QPointF itemPosition);
    void transformPixmap(QGraphicsSceneMouseEvent *event);
    void changeReflection(QGraphicsSceneMouseEvent *event);
    void addReflectionOpacity(int reflectionOpacity);

    void selectExclusive();

private slots:
    void updateReflection();
    void updatePixmap(const QPixmap &pixmap);
    void updatePixmap();
    void updateItem();
};

#endif // SCREENIEPIXMAPITEM_H
