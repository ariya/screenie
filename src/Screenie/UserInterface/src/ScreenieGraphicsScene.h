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

#ifndef SCREENIEGRAPHICSSCENE_H
#define SCREENIEGRAPHICSSCENE_H

#include <QtCore/QList>
#include <QtGui/QPixmap>
#include <QtGui/QGraphicsScene>

class QGraphicsSceneDragDropEvent;
class QEvent;
class QGestureEvent;
class QPinchGesture;
class QPanGesture;
class QGesture;
class QObject;

/*!
 * Provides the drag'n'drop functionality for opening image and scene files.
 */
class ScreenieGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ScreenieGraphicsScene(QObject *parent = 0);
    virtual ~ScreenieGraphicsScene();

signals:
    void pixmapsAdded(QList<QPixmap> pixmaps, QPointF position);
    void filePathsAdded(QStringList filePaths, QPointF position);
    void rotate(int angle);
    void addDistance(int distance);
    void translate(qreal x, qreal y);

protected:
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
    virtual bool event(QEvent *event);

private:
    bool m_itemDragDrop;

    bool gestureEvent(const QGestureEvent *event);
    bool pinchTriggered(const QPinchGesture *gesture);
    bool panTriggered(const QPanGesture *gesture);
};

#endif // SCREENIEGRAPHICSSCENE_H
