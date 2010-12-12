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
#include <QtGui/QPrinter>
#include <QtGui/QPainter>

#include "../../Model/src/ScreenieScene.h"

#include "ExportPDF.h"

// public

ExportPDF::ExportPDF(const ScreenieScene &screenieScene, QGraphicsScene &graphicsScene)
    : m_screenieScene(screenieScene),
      m_graphicsScene(graphicsScene)
{
}

void ExportPDF::exportPDF(const QString &filePath)
{
    QRectF targetRect(QPointF(0.0f, 0.0f), QSizeF(640.0, 480.0));
    QPrinter printer;
    printer.setPageSize(QPrinter::A4);
#ifdef Q_OS_MAC
    // Mac can render PDF natively
    printer.setOutputFormat(QPrinter::NativeFormat);
#else
    printer.setOutputFormat(QPrinter::PdfFormat);
#endif
    printer.setOutputFileName(filePath);
    QPainter painter(&printer);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
    if (m_screenieScene.isBackgroundEnabled()) {
        painter.fillRect(targetRect, m_screenieScene.getBackgroundColor());
    }
    m_graphicsScene.clearSelection();
    m_graphicsScene.render(&painter, targetRect);
}
