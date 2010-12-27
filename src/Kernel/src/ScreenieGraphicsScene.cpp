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

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtGui/QPixmap>
#include <QtGui/QGraphicsSceneDragDropEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QWidget>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPinchGesture>
#include <QtGui/QGraphicsView>

#include "Clipboard/MimeHelper.h"
#include "ScreenieGraphicsScene.h"

// public

ScreenieGraphicsScene::ScreenieGraphicsScene(QObject *parent)
    : QGraphicsScene(parent),
      m_itemDragDrop(false)
{
}

ScreenieGraphicsScene::~ScreenieGraphicsScene()
{
#ifdef DEBUG
    qDebug("ScreenieGraphicsScene::~QGraphicsScene: called.");
#endif
}

// protected

void ScreenieGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    bool accept = MimeHelper::accept(event->mimeData(), MimeHelper::Relaxed);
    event->setAccepted(accept);
}

void ScreenieGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (this->itemAt(event->scenePos().x(), event->scenePos().y()) == 0) {
        event->setAccepted(event->mimeData()->hasUrls());
        m_itemDragDrop = false;
    } else {
        QGraphicsScene::dragMoveEvent(event);
        m_itemDragDrop = true;
    }
}

void ScreenieGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (!m_itemDragDrop) {
        QList<QUrl> urls = event->mimeData()->urls();
        QList<QPixmap> pixmaps;
        QStringList filePaths;

        // prefer image data over filepaths
        if (event->mimeData()->hasImage()) {
            QPixmap pixmap;
            pixmap = qvariant_cast<QPixmap>(event->mimeData()->imageData());
            pixmaps.append(pixmap);
        } else {
            foreach (QUrl url, urls) {
                /*!\todo Support for "http:// images"? For now we assume the paths can be converted to local paths. */
                QString filePath = url.toLocalFile();
                if (!filePath.isEmpty()) {
                    filePaths.append(filePath);
                }
            }
        }
        if (pixmaps.size() > 0) {
            emit pixmapsAdded(pixmaps, event->scenePos());
        }
        if (filePaths.size() > 0) {
            emit filePathsAdded(filePaths, event->scenePos());
        }
        event->accept();
    } else {
        QGraphicsScene::dropEvent(event);
    }
}

bool ScreenieGraphicsScene::event(QEvent *event)
{
    bool result;
    if (event->type() == QEvent::Gesture) {
        result = gestureEvent(static_cast<QGestureEvent *>(event));
        event->setAccepted(result);
    } else {
        result = QGraphicsScene::event(event);
    }
    return result;
}

// private

bool ScreenieGraphicsScene::gestureEvent(const QGestureEvent *event)
{
    bool result;
    if (selectedItems().size() > 0) {
        if (QGesture *pinch = event->gesture(Qt::PinchGesture)) {
            result = pinchTriggered(static_cast<QPinchGesture *>(pinch));
        } else if (QGesture *pan = event->gesture(Qt::PanGesture)) {
            result = panTriggered(static_cast<QPanGesture *>(pan));
        } else {
            result = false;
        }
    } else {
        result = false;
    }

    return result;
}

bool ScreenieGraphicsScene::pinchTriggered(const QPinchGesture *gesture)
{
    bool result;

    /*!\todo Values 2.0 and 10.0 are magic factors - make them configurable (they work well though on a MacBook Pro ;) */
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::RotationAngleChanged) {
        qreal rotation = gesture->rotationAngle();
        qreal lastRotation = gesture->lastRotationAngle();
        qreal angle = rotation - lastRotation;
        emit rotate(qRound(angle * 2.0));
        result = true;
    } else {
        result = false;
    }
    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
        qreal distance = 1.0 - gesture->scaleFactor();
        emit addDistance(qRound(distance * 10.0));
        result = true;
    }
    return result;
}

bool ScreenieGraphicsScene::panTriggered(const QPanGesture *gesture)
{
    bool result = true;
    QPointF delta = gesture->delta();
    qreal x = delta.x();
    qreal y = delta.y();
    emit translate(x, y);
    return result;
}
