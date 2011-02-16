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
#include <QtCore/QPointF>
#include <QtCore/QTimer>
#include <QtGui/QImage>
#include <QtGui/QGraphicsSceneDragDropEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QWidget>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPinchGesture>
#include <QtGui/QGraphicsView>

#include "../../Utils/src/Settings.h"
#include "Clipboard/MimeHelper.h"
#include "ScreenieGraphicsScene.h"

class ScreenieGraphicsScenePrivate
{
public:
    ScreenieGraphicsScenePrivate()
        : itemDragDrop(false)
    {}

    bool itemDragDrop;
};

// public

ScreenieGraphicsScene::ScreenieGraphicsScene(QObject *parent)
    : QGraphicsScene(parent),
      d(new ScreenieGraphicsScenePrivate())
{
}

ScreenieGraphicsScene::~ScreenieGraphicsScene()
{
    delete d;
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
        d->itemDragDrop = false;
    } else {
        QGraphicsScene::dragMoveEvent(event);
        d->itemDragDrop = true;
    }
}

void ScreenieGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (!d->itemDragDrop) {
        QList<QUrl> urls = event->mimeData()->urls();
        QList<QImage> images;
        QStringList filePaths;

        // prefer image data over filepaths
        if (event->mimeData()->hasImage()) {
            QImage image;
            image = qvariant_cast<QImage>(event->mimeData()->imageData());
            images.append(image);
        } else {
            foreach (QUrl url, urls) {
                /*!\todo Support for "http:// images"? For now we assume the paths can be converted to local paths. */
                QString filePath = url.toLocalFile();
                if (!filePath.isEmpty()) {
                    filePaths.append(filePath);
                }
            }
        }
        if (images.size() > 0) {
            emit imagesDropped(images, event->scenePos());
        }
        if (filePaths.size() > 0) {
            emit filePathsDropped(filePaths, event->scenePos());
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
    bool result = false;
    if (selectedItems().size() > 0) {
        if (QGesture *pinch = event->gesture(Qt::PinchGesture)) {
            result = pinchTriggered(static_cast<QPinchGesture *>(pinch));
        }
    } else {
        result = false;
    }

    return result;
}

bool ScreenieGraphicsScene::pinchTriggered(const QPinchGesture *gesture)
{
    bool result;
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::RotationAngleChanged) {
        qreal rotation = gesture->rotationAngle();
        qreal lastRotation = gesture->lastRotationAngle();
        qreal angle = rotation - lastRotation;
        qreal rotationSensitivity = Settings::getInstance().getRotationGestureSensitivity();
        emit rotate(qRound(angle * rotationSensitivity));
        result = true;
    } else {
        result = false;
    }
    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
        qreal distance = 1.0 - gesture->scaleFactor();
        qreal distanceSensitivity = Settings::getInstance().getDistanceGestureSensitivity();
        emit addDistance(distance * distanceSensitivity);
        result = true;
    }
    return result;
}


