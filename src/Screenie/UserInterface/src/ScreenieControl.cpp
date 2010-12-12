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
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QStringList>
#include <QtCore/QtAlgorithms>
#include <QtGui/QColor>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsItem>
#include <QtGui/QBrush>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QMainWindow>
#include <QtGui/QSlider>


#include "../../../Model/src/ScreenieScene.h"
#include "../../../Model/src/ScreenieModelInterface.h"
#include "../../../Model/src/ScreenieFilePathModel.h"
#include "../../../Model/src/ScreeniePixmapModel.h"
#include "../../../Kernel/src/Reflection.h"
#include "ScreenieGraphicsScene.h"
#include "ScreeniePixmapItem.h"

#include "ScreenieControl.h"

namespace
{
    bool zSort(ScreeniePixmapItem *item1, ScreeniePixmapItem *item2)
    {
        // closer items come first in the sorted list
        return item1->getScreenieModel().getDistance() > item2->getScreenieModel().getDistance();
    }
}; // anonymous

// public

ScreenieControl::ScreenieControl(ScreenieScene &screenieScene, ScreenieGraphicsScene &screenieGraphicsScene)
    : m_screenieScene(screenieScene),
      m_screenieGraphicsScene(screenieGraphicsScene),
      m_reflection(new Reflection())
{
    m_qualityTimer.setSingleShot(true);
    m_qualityTimer.setInterval(300);
    frenchConnection();
}

ScreenieControl::~ScreenieControl()
{
    delete m_reflection;
}

QList<ScreenieModelInterface *> ScreenieControl::getSelectedScreenieModels() const
{
    QList<ScreenieModelInterface *> result;
    foreach (QGraphicsItem *selectedItem, m_screenieGraphicsScene.selectedItems()) {
        if (selectedItem->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(selectedItem);
            result.append(&screeniePixmapItem->getScreenieModel());
        }
    }
    return result;
}

DefaultScreenieModel &ScreenieControl::getDefaultScreenieModel()
{
    return m_defaultScreenieModel;
}

// public slots

void ScreenieControl::addImage(QString filePath, QPointF position)
{
    QStringList filePaths(filePath);
    addImages(filePaths, position);
}

void ScreenieControl::addImages(QStringList filePaths, QPointF position)
{
    foreach (QString filePath, filePaths) {
        ScreenieModelInterface *screenieModel = new ScreenieFilePathModel(filePath);
        applyDefaultValues(*screenieModel);
        screenieModel->setPosition(position);
        qDebug("ScreenieControl: Rotation: %d, dist.: %d", screenieModel->getRotation(), screenieModel->getDistance());
        m_screenieScene.addModel(screenieModel);
    }
}

void ScreenieControl::addImage(QPixmap pixmap, QPointF position)
{
    QList<QPixmap> pixmaps;
    pixmaps.append(pixmap);
    addImages(pixmaps, position);
}

void ScreenieControl::addImages(QList<QPixmap> pixmaps, QPointF position)
{
    foreach (QPixmap pixmap, pixmaps) {
        ScreeniePixmapModel *screenieModel = new ScreeniePixmapModel(pixmap);
        applyDefaultValues(*screenieModel);
        screenieModel->setPosition(position);       
        qDebug("ScreenieControl: Rotation: %d, dist.: %d", screenieModel->getRotation(), screenieModel->getDistance());
        m_screenieScene.addModel(screenieModel);
    }
}

void ScreenieControl::setRotation(int angle)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setRotation(angle);
    }
    m_qualityTimer.start();
}

void ScreenieControl::rotate(int angle)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->rotate(angle);
    }
    m_qualityTimer.start();
}

void ScreenieControl::setPosition(QPointF position)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setPosition(position);
    }
    m_qualityTimer.start();
}

void ScreenieControl::setDistance(int distance)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setDistance(distance);
    }
    m_qualityTimer.start();
}

void ScreenieControl::addDistance(int distance)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->addDistance(distance);
    }
    m_qualityTimer.start();
}

void ScreenieControl::setReflectionEnabled(bool enable)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setReflectionEnabled(enable);
    }
    m_qualityTimer.start();
}

void ScreenieControl::setReflectionOffset(int reflectionOffset)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setReflectionOffset(reflectionOffset);
    }
    m_qualityTimer.start();
}

void ScreenieControl::setReflectionOpacity(int reflectionOpacity)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setReflectionOpacity(reflectionOpacity);
    }
    m_qualityTimer.start();
}

void ScreenieControl::setBackgroundEnabled(bool enable)
{
    m_screenieScene.setBackgroundEnabled(enable);
}

void ScreenieControl::setBackgroundColor(QColor color)
{
    m_screenieScene.setBackgroundColor(color);
}

void ScreenieControl::setRedBackgroundComponent(int red)
{
    QColor backgroundColor = m_screenieScene.getBackgroundColor();
    backgroundColor.setRed(red);
    m_screenieScene.setBackgroundColor(backgroundColor);
}

void ScreenieControl::setGreenBackgroundComponent(int green)
{
    QColor backgroundColor = m_screenieScene.getBackgroundColor();
    backgroundColor.setGreen(green);
    m_screenieScene.setBackgroundColor(backgroundColor);
}

void ScreenieControl::setBlueBackgroundComponent(int blue)
{
    QColor backgroundColor = m_screenieScene.getBackgroundColor();
    backgroundColor.setBlue(blue);
    m_screenieScene.setBackgroundColor(backgroundColor);
}

void ScreenieControl::removeItems()
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        m_screenieScene.removeModel(screenieModel);
    }
    m_qualityTimer.start();
}

// private

void ScreenieControl::frenchConnection()
{
    connect(&m_screenieScene, SIGNAL(distanceChanged()),
            this, SLOT(handleDistanceChanged()));
    connect(&m_screenieScene, SIGNAL(modelAdded(ScreenieModelInterface &)),
            this, SLOT(handleModelAdded(ScreenieModelInterface &)));
    connect(&m_screenieScene, SIGNAL(modelRemoved(const ScreenieModelInterface &)),
            this, SLOT(handleModelRemoved(const ScreenieModelInterface &)));
    connect(&m_screenieScene, SIGNAL(backgroundChanged()),
            this, SLOT(handleBackgroundChanged()));
    connect(&m_screenieGraphicsScene, SIGNAL(pixmapsAdded(QList<QPixmap>, QPointF)),
            this, SLOT(addImages(QList<QPixmap>, QPointF)));
    connect(&m_screenieGraphicsScene, SIGNAL(filePathsAdded(QStringList, QPointF)),
            this, SLOT(addImages(QStringList, QPointF)));
    connect(&m_screenieGraphicsScene, SIGNAL(removeItems()),
            this, SLOT(removeItems()));
    connect(&m_qualityTimer, SIGNAL(timeout()),
            this, SLOT(restoreRenderQuality()));
}

QList<ScreeniePixmapItem *> ScreenieControl::getScreeniePixmapItems() const
{
    QList<ScreeniePixmapItem *> result;
    foreach (QGraphicsItem *item, m_screenieGraphicsScene.items()) {
        if (item->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(item);
            result.append(screeniePixmapItem);
        }
    }
    return result;
}

void ScreenieControl::setRenderQuality(RenderQuality renderQuality)
{
    QList<ScreeniePixmapItem *> items = getScreeniePixmapItems();
    switch (renderQuality) {
    case Low:
        foreach (ScreeniePixmapItem *item, items) {
            item->setTransformationMode(Qt::FastTransformation);
        }
        foreach (QGraphicsView *view, m_screenieGraphicsScene.views()) {
            view->setRenderHints(QPainter::NonCosmeticDefaultPen);
        }
        break;
    case High:
        foreach (ScreeniePixmapItem *item, items) {
            item->setTransformationMode(Qt::SmoothTransformation);
        }
        foreach (QGraphicsView *view, m_screenieGraphicsScene.views()) {
            view->setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
        }
    default:
        break;
    }
}

void ScreenieControl::applyDefaultValues(ScreenieModelInterface &screenieModelInterface)
{
    screenieModelInterface.setDistance(m_defaultScreenieModel.getDistance());
    screenieModelInterface.setRotation(m_defaultScreenieModel.getRotation());
    screenieModelInterface.setReflectionEnabled(m_defaultScreenieModel.isReflectionEnabled());
    screenieModelInterface.setReflectionOffset(m_defaultScreenieModel.getReflectionOffset());
    screenieModelInterface.setReflectionOpacity(m_defaultScreenieModel.getReflectionOpacity());
}

// private slots

void ScreenieControl::handleDistanceChanged()
{
    QList<ScreeniePixmapItem *> screeniePixmapItems;
    foreach (QGraphicsItem *graphicsItem, m_screenieGraphicsScene.items()) {
        if (graphicsItem->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(graphicsItem);
            screeniePixmapItems.append(screeniePixmapItem);
        }
    }
    qSort(screeniePixmapItems.begin(), screeniePixmapItems.end(), zSort);
    int z = 0;
    foreach (QGraphicsItem *graphicsItem, screeniePixmapItems) {
        graphicsItem->setZValue(z++);
    }
}

void ScreenieControl::handleModelAdded(ScreenieModelInterface &screenieModel)
{
    ScreeniePixmapItem *screeniePixmapItem = new ScreeniePixmapItem(screenieModel, *this, *m_reflection);
    screeniePixmapItem->setPos(screenieModel.getPosition());
    m_screenieGraphicsScene.clearSelection();
    screeniePixmapItem->setSelected(true);
    m_screenieGraphicsScene.addItem(screeniePixmapItem);
    handleDistanceChanged();
}

void ScreenieControl::handleModelRemoved(const ScreenieModelInterface &screenieModel)
{
    foreach (QGraphicsItem *graphicsItem, m_screenieGraphicsScene.items()) {
        if (graphicsItem->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(graphicsItem);
            if (&screenieModel == &screeniePixmapItem->getScreenieModel()) {
                m_screenieGraphicsScene.removeItem(screeniePixmapItem);
                break;
            }
        }
    }
}

void ScreenieControl::handleBackgroundChanged()
{
    QBrush bgbrush;
    if (m_screenieScene.isBackgroundEnabled()) {
        QColor backgroundColor = m_screenieScene.getBackgroundColor();
        bgbrush = QBrush(backgroundColor);
    } else {
        if (m_checkerBoard.style() == Qt::NoBrush) {
            QImage checker(16, 16, QImage::Format_ARGB32_Premultiplied);
            QPainter painter(&checker);
            painter.fillRect(checker.rect(), QColor(153, 153, 153));
            painter.fillRect(0, 0, 8, 8, QColor(102, 102, 102));
            painter.fillRect(8, 8, 8, 8, QColor(102, 102, 102));
            painter.end();
            m_checkerBoard.setTextureImage(checker);
        }
        bgbrush = m_checkerBoard;
    }
    m_screenieGraphicsScene.setBackgroundBrush(bgbrush);
}

void ScreenieControl::restoreRenderQuality()
{
    setRenderQuality(High);
}

