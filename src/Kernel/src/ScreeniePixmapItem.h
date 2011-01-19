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
#include <QtCore/QVariant>
#include <QtGui/QGraphicsPixmapItem>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneDragDropEvent;
class QImage;

#include "KernelLib.h"

class ScreenieModelInterface;
class ScreenieControl;
class Reflection;
class ScreeniePixmapItemPrivate;

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
    KERNEL_API static const int ScreeniePixmapType;

    KERNEL_API ScreeniePixmapItem(ScreenieModelInterface &screenieModel, ScreenieControl &screenieControl, Reflection &reflection);
    KERNEL_API virtual ~ScreeniePixmapItem();

    KERNEL_API ScreenieModelInterface &getScreenieModel() const;

protected:
    virtual int type() const;
    virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    ScreeniePixmapItemPrivate *d;

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
    void updatePixmap(const QImage &image);
    void updatePixmap();
    void updateItemGeometry();
    void updateItem();
    void updatePosition();
    void updateSelection();
    void handlePropertyDialogDestroyed();
};

#endif // SCREENIEPIXMAPITEM_H
