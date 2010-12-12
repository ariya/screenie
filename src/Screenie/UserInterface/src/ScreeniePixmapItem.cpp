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

#include <QtCore/QPoint>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>

#include "../../../Utils/src/PaintTools.h"
#include "../../../Model/src/ScreenieModelInterface.h"
#include "../../../Kernel/src/Reflection.h"
#include "ScreenieControl.h"
#include "ScreeniePixmapItem.h"

const int ScreeniePixmapItem::ScreeniePixmapType = QGraphicsItem::UserType + 1;

// public

ScreeniePixmapItem::ScreeniePixmapItem(ScreenieModelInterface &screenieModel, ScreenieControl &screenieControl, Reflection &reflection)
    : m_screenieModel(screenieModel),
      m_screenieControl(screenieControl),
      m_reflection(reflection)
{
    qDebug("ScreeniePixmapItem: Rotation: %d, dist.: %d, refl: %d, off: %d, opacity: %d",
            screenieModel.getRotation(), screenieModel.getDistance(), screenieModel.isReflectionEnabled(), screenieModel.getReflectionOffset(), screenieModel.getReflectionOpacity());
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setTransformationMode(Qt::SmoothTransformation);
    QPixmap pixmap = screenieModel.readPixmap();
    setPixmap(pixmap);
    updateReflection();
    updateItem();
    frenchConnection();
}

ScreeniePixmapItem::~ScreeniePixmapItem()
{
}

ScreenieModelInterface &ScreeniePixmapItem::getScreenieModel() const
{
    return m_screenieModel;
}

// protected

int ScreeniePixmapItem::type() const
{
    return ScreeniePixmapType;
}

void ScreeniePixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int deltaX, deltaY;

    switch (event->buttons()) {
    case Qt::RightButton:
        deltaX = event->lastScreenPos().x() - event->screenPos().x();
        rotate(deltaX);
        event->accept();
        break;

     case Qt::MiddleButton:
        deltaY = event->lastScreenPos().y() - event->screenPos().y();
        addDistance(deltaY);
        event->accept();
        break;

    case Qt::LeftButton:
        QGraphicsPixmapItem::mouseMoveEvent(event);
        m_screenieModel.setPosition(scenePos());
        event->accept();
#ifdef DEBUG
        qDebug("ScreeniePixmapItem::mouseMoveEvent: x/y: %f/%f", scenePos().x(), scenePos().y());
#endif
        break;
     default:
        QGraphicsPixmapItem::mouseMoveEvent(event);
        break;
    }
}

void ScreeniePixmapItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    int distance = event->delta() / 12;
    addDistance(distance);
    event->accept();
}

// private

void ScreeniePixmapItem::frenchConnection()
{
    connect(&m_screenieModel, SIGNAL(changed()),
            this, SLOT(updateItem()));
    connect(&m_screenieModel, SIGNAL(distanceChanged()),
            this, SLOT(updateItem()));
    connect(&m_screenieModel, SIGNAL(reflectionChanged()),
            this, SLOT(updateReflection()));
}

void ScreeniePixmapItem::rotate(int angle)
{
    if (!isSelected()) {
        scene()->clearSelection();
        setSelected(true);
    }
    m_screenieControl.rotate(angle);
}

void ScreeniePixmapItem::addDistance(int distance)
{
    if (!isSelected()) {
        scene()->clearSelection();
        setSelected(true);
    }
    m_screenieControl.addDistance(distance);
}

// private slots

void ScreeniePixmapItem::updateReflection()
{
    QPixmap pixmap = this->pixmap();
    if (m_screenieModel.isReflectionEnabled()) {
        if (m_screenieModel.getSize() != pixmap.size()) {
            // the pixmap already has a reflection (height must be 2x original height),
            // so just take the upper half (the original pixmap)
            pixmap = PaintTools::upperHalf(pixmap);
        }
        pixmap = m_reflection.addReflection(pixmap, m_screenieModel.getReflectionOpacity(), m_screenieModel.getReflectionOffset());
    } else {
        if (m_screenieModel.getSize() != pixmap.size()) {
            pixmap = PaintTools::upperHalf(pixmap);
        }
    }
    setPixmap(pixmap);
}

void ScreeniePixmapItem::updateItem()
{
    QTransform transform;
    QTransform scale;
    QTransform translateBack;

    qreal centerScale = 1.0 - 0.9 * m_screenieModel.getDistance() / ScreenieModelInterface::MaxDistance;
    scale = QTransform().scale(centerScale, centerScale);

    QPixmap pixmap = this->pixmap();
    qreal dx = pixmap.width() / 2.0;
    qreal dy;
    if (m_screenieModel.isReflectionEnabled()) {
        dy =  pixmap.height() / 4.0;
    } else {
        dy = pixmap.height() / 2.0;
    }
    transform.translate(dx, dy);
    transform.rotate(m_screenieModel.getRotation(), Qt::YAxis);
    translateBack.translate(-dx, -dy);
    transform = translateBack * scale * transform;
    setTransform(transform, false);
}
