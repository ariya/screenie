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
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
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
      m_reflection(reflection),
      m_transformPixmap(true)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setTransformationMode(Qt::SmoothTransformation);
    QPixmap pixmap = m_screenieModel.readPixmap();
    updatePixmap(pixmap);
    setAcceptDrops(true);
    frenchConnection();
}

ScreeniePixmapItem::~ScreeniePixmapItem()
{
#ifdef DEBUG
    qDebug("ScreeniePixmapItem::~ScreeniePixmapItem: called.");
#endif
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

void ScreeniePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   m_transformPixmap = isInsidePixmap(event->pos());
   event->accept();
}

void ScreeniePixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isInsidePixmap(event->pos()) && m_transformPixmap) {
        transformPixmap(event);
    } else if (!m_transformPixmap) {
        changeReflection(event);
    }
}

void ScreeniePixmapItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    int value = event->delta() / 12;
    if (isInsidePixmap(event->pos())) {
        addDistance(value);
    } else {
        addReflectionOpacity(value);
    }
    event->accept();
}

void ScreeniePixmapItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(event->mimeData()->hasUrls() || event->mimeData()->hasImage());
}

void ScreeniePixmapItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    m_screenieControl.updateData(event->mimeData(), m_screenieModel);
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
    connect(&m_screenieModel, SIGNAL(pixmapChanged(const QPixmap &)),
            this, SLOT(updatePixmap(const QPixmap &)));
    connect(&m_screenieModel, SIGNAL(filePathChanged(const QString &)),
            this, SLOT(updatePixmap()));
}

void ScreeniePixmapItem::rotate(int angle)
{
    selectExclusive();
    m_screenieControl.rotate(angle);
}

void ScreeniePixmapItem::addDistance(int distance)
{
    selectExclusive();
    m_screenieControl.addDistance(distance);
}

bool ScreeniePixmapItem::isInsidePixmap(QPointF itemPosition)
{
    bool result;
    if (m_screenieModel.isReflectionEnabled()) {
        QRectF boundingRect = this->boundingRect();
        result = itemPosition.y() < boundingRect.height() / 2.0;
    } else {
        result = true;
    }
    return result;
}

void ScreeniePixmapItem::transformPixmap(QGraphicsSceneMouseEvent *event)
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
        break;
     default:
        QGraphicsPixmapItem::mouseMoveEvent(event);
        break;
    }
}

void ScreeniePixmapItem::changeReflection(QGraphicsSceneMouseEvent *event)
{
    qreal height_2 = boundingRect().height() / 2.0;
    qreal factor  = (event->pos().y() - height_2) / height_2;
    int percent = qRound(factor * 100.0);
    switch (event->buttons()) {
    case Qt::LeftButton:
        if (percent < 1) {
            percent = 1;
        } else if (percent > 100) {
            percent = 100;
        }
        selectExclusive();
        m_screenieControl.setReflectionOffset(percent);
        event->accept();
        break;

    case Qt::RightButton:
        if (percent < 0) {
            percent = 0;
        } else if (percent > 100) {
            percent = 100;
        }
        selectExclusive();
        m_screenieControl.setReflectionOpacity(percent);
        event->accept();
        break;

    default:
        QGraphicsPixmapItem::mouseMoveEvent(event);
        break;
    }
}

void ScreeniePixmapItem::addReflectionOpacity(int reflectionOpacity)
{
    selectExclusive();
    m_screenieControl.addReflectionOpacity(reflectionOpacity);
}

void ScreeniePixmapItem::selectExclusive()
{
    if (!isSelected()) {
        scene()->clearSelection();
        setSelected(true);
    }
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

void ScreeniePixmapItem::updatePixmap(const QPixmap &pixmap)
{
    setPixmap(pixmap);
    updateReflection();
    updateItem();
}

void ScreeniePixmapItem::updatePixmap()
{
    QPixmap pixmap = m_screenieModel.readPixmap();
    updatePixmap(pixmap);
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
