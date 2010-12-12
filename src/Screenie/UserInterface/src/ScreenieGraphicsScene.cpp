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
#include <QtGui/QGraphicsScene>

#include "ScreenieGraphicsScene.h"

// public

ScreenieGraphicsScene::ScreenieGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

// protected

void ScreenieGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->accept();
    }
}

void ScreenieGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->accept();
    }
}

void ScreenieGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    QList<QPixmap> pixmaps;
    QStringList filePaths;
    foreach (QUrl url, urls) {
        QPixmap pixmap;
        QString filePath = url.toLocalFile();

        // prefer image data over filepaths
        if (event->mimeData()->hasImage()) {
            pixmap = qvariant_cast<QPixmap>(event->mimeData()->imageData());
            pixmaps.append(pixmap);
        }
        else {
            filePaths.append(filePath);
        }       
    }
    if (pixmaps.size() > 0) {
        emit pixmapsAdded(pixmaps, event->scenePos());
    }
    if (filePaths.size() > 0) {
        emit filePathsAdded(filePaths, event->scenePos());
    }
    event->accept();
}

void ScreenieGraphicsScene::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Delete:
        emit removeItems();
        event->accept();
        break;
    default:
        break;
    }
}
