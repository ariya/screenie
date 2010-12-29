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

#include <QtCore/QRectF>
#include <QtCore/QPointF>
#include <QtCore/QSizeF>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <QtGui/QImage>
#include <QtGui/QPainter>

#include "../../Model/src/ScreenieScene.h"
#include "ExportImage.h"

// public

ExportImage::ExportImage(const ScreenieScene &screenieScene, QGraphicsScene &graphicsScene)
    : m_screenieScene(screenieScene),
      m_graphicsScene(graphicsScene)
{
}

void ExportImage::exportImage(const QString &filePath, Selection selection) const
{
    /*!\todo Specify some margin etc. */

    QImage image = exportImage(selection);
    image.save(filePath, "PNG");
}

QImage ExportImage::exportImage(Selection selection) const
{
    QRectF sourceRect;
    QBrush oldBackgroundBrush;
    QList<QGraphicsItem *> selectedItems = m_graphicsScene.selectedItems();
    QList<QGraphicsItem *> items = m_graphicsScene.items();
    QList<QGraphicsItem *> invisibleItems;

    switch (selection) {
    case Scene:
        sourceRect =  m_graphicsScene.itemsBoundingRect();
        break;
    case Selected:
        foreach(QGraphicsItem *current, items) {
            if (current->isSelected()) {
                sourceRect = sourceRect.united(current->sceneBoundingRect());
            } else if (current->isVisible()) {
                invisibleItems.append(current);
                current->setVisible(false);
            }
        }
        break;
    default:
#ifdef DEBUG
        qWarning("ExportImage::exportImage: unsupported Selection: %d", selection);
#endif
        sourceRect = m_graphicsScene.itemsBoundingRect();
        break;
    }
    QImage result(qRound(sourceRect.width()), qRound(sourceRect.height()), QImage::Format_ARGB32);
    result.fill(0);
    QPainter painter(&result);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
    if (m_screenieScene.isBackgroundEnabled()) {
        painter.fillRect(0, 0, result.width(), result.height(), m_screenieScene.getBackgroundColor());
    } else {
        QBrush transparent(QColor(0, 0, 0, 0));
        oldBackgroundBrush = m_graphicsScene.backgroundBrush();
        m_graphicsScene.setBackgroundBrush(transparent);
    }
    m_graphicsScene.clearSelection();
    m_graphicsScene.render(&painter, QRectF(), sourceRect);
    // restore selection
    foreach(QGraphicsItem *current, selectedItems) {
        current->setSelected(true);
    }
    // restore visibility
    foreach(QGraphicsItem *current, invisibleItems) {
        current->setVisible(true);
    }
    // restore background
    if (oldBackgroundBrush != Qt::NoBrush) {
        m_graphicsScene.setBackgroundBrush(oldBackgroundBrush);
    }
    return result;
}
