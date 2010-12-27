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
#include <QtCore/QRectF>
#include <QtCore/QStringList>
#include <QtCore/QtAlgorithms>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtGui/QColor>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsItem>
#include <QtGui/QBrush>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QMainWindow>
#include <QtGui/QSlider>

#include "../../../Utils/src/PaintTools.h"
#include "../../../Utils/src/SizeFitter.h"
#include "../../../Model/src/ScreenieScene.h"
#include "../../../Model/src/ScreenieModelInterface.h"
#include "../../../Model/src/ScreenieFilePathModel.h"
#include "../../../Model/src/ScreeniePixmapModel.h"
#include "../../../Model/src/ScreenieTemplateModel.h"
#include "Reflection.h"
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
    : QObject(),
      m_screenieScene(screenieScene),
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

void ScreenieControl::updateModel(const QMimeData *mimeData, ScreenieModelInterface &screenieModel)
{
    // prefer image data over file paths (URLs)
    if (mimeData->hasImage()) {
        updatePixmapModel(mimeData, screenieModel);
    } else {
        updateFilePathModel(mimeData, screenieModel);
    }
}

ScreenieScene &ScreenieControl::getScreenieScene() const
{
    return m_screenieScene;
}
ScreenieGraphicsScene &ScreenieControl::getScreenieGraphicsScene() const
{
    return m_screenieGraphicsScene;
}

// public slots

void ScreenieControl::addImage(QString filePath, QPointF centerPosition)
{
    QStringList filePaths(filePath);
    addImages(filePaths, centerPosition);
}

void ScreenieControl::addImages(QStringList filePaths, QPointF centerPosition)
{
    foreach (QString filePath, filePaths) {
        ScreenieModelInterface *screenieModel = new ScreenieFilePathModel(filePath);
        applyDefaultValues(*screenieModel);
        QPointF itemPosition = calculateItemPosition(*screenieModel, centerPosition);
        screenieModel->setPosition(itemPosition);
        m_screenieScene.addModel(screenieModel);
    }
}

void ScreenieControl::addImage(QPixmap pixmap, QPointF centerPosition)
{
    QList<QPixmap> pixmaps;
    pixmaps.append(pixmap);
    addImages(pixmaps, centerPosition);
}

void ScreenieControl::addImages(QList<QPixmap> pixmaps, QPointF centerPosition)
{
    foreach (QPixmap pixmap, pixmaps) {
        ScreeniePixmapModel *screenieModel = new ScreeniePixmapModel(pixmap);
        applyDefaultValues(*screenieModel);
        QPointF itemPosition = calculateItemPosition(*screenieModel, centerPosition);
        screenieModel->setPosition(itemPosition);
        m_screenieScene.addModel(screenieModel);
    }
}

void ScreenieControl::addTemplate(QPointF centerPosition)
{
    /*!\todo Make the template size configurable in some UI dialog */
    QSize size(400, 400);
    ScreenieTemplateModel *screenieModel = new ScreenieTemplateModel(size);
    applyDefaultValues(*screenieModel);
    screenieModel->setPosition(centerPosition);
    m_screenieScene.addModel(screenieModel);
}

void ScreenieControl::removeAll()
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        m_screenieScene.removeModel(screenieModel);
    }
    m_qualityTimer.start();
}

void ScreenieControl::selectAll()
{
    foreach(QGraphicsItem *item, m_screenieGraphicsScene.items()) {
        item->setSelected(true);
    }
}

void ScreenieControl::translate(qreal dx, qreal dy)
{
    bool decreaseQuality = dx != 0.0 && dy != 0.0;
    if (decreaseQuality) {
        setRenderQuality(Low);
        m_qualityTimer.start();
    }
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->translate(dx, dy);
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

void ScreenieControl::addReflectionOffset(int reflectionOffset)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->addReflectionOffset(reflectionOffset);
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

void ScreenieControl::addReflectionOpacity(int reflectionOpacity)
{
    setRenderQuality(Low);
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->addReflectionOpacity(reflectionOpacity);
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
    connect(&m_screenieGraphicsScene, SIGNAL(rotate(int)),
            this, SLOT(rotate(int)));
    connect(&m_screenieGraphicsScene, SIGNAL(addDistance(int)),
            this, SLOT(addDistance(int)));
    connect(&m_screenieGraphicsScene, SIGNAL(translate(qreal, qreal)),
            this, SLOT(translate(qreal, qreal)));
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

QPointF ScreenieControl::calculateItemPosition(const ScreenieModelInterface &screenieModel, const QPointF &centerPosition)
{
    QPointF result;
    QSize itemSize = screenieModel.getSize();
    result.setX(centerPosition.x() - itemSize.width()  / 2.0);
    result.setY(centerPosition.y() - itemSize.height() / 2.0);
    return result;
}

QPixmap ScreenieControl::scaleToTemplate(const ScreenieTemplateModel &templateModel, const QPixmap &pixmap)
{
    QPixmap result;
    const SizeFitter &sizeFitter = templateModel.getSizeFitter();
    QSize fittedSize;
    bool doResize = sizeFitter.fit(pixmap.size(), fittedSize);
    if (doResize) {
        result = pixmap.scaled(fittedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    } else {
        result = pixmap;
    }
    return result;
}

QPointF ScreenieControl::calculateItemPosition(const QPointF &sourcePosition, const QSize &sourceSize, const QSize &targetSize)
{
    QPointF result;
    result.setX(sourcePosition.x() + sourceSize.width()  / 2.0 - targetSize.width()  / 2.0);
    result.setY(sourcePosition.y() + sourceSize.height() / 2.0 - targetSize.height() / 2.0);
    return result;
}

void ScreenieControl::updatePixmapModel(const QMimeData *mimeData, ScreenieModelInterface &screenieModel)
{
    QPixmap pixmap;
    pixmap = qvariant_cast<QPixmap>(mimeData->imageData());
    if (!pixmap.isNull()) {
        if (screenieModel.inherits(ScreeniePixmapModel::staticMetaObject.className())) {
            ScreeniePixmapModel &screeniePixmapModel = static_cast<ScreeniePixmapModel &>(screenieModel);
            QPointF itemPosition = calculateItemPosition(screeniePixmapModel.getPosition(), screeniePixmapModel.getSize(), pixmap.size());
            screeniePixmapModel.setPixmap(pixmap);
            screeniePixmapModel.setPosition(itemPosition);
        } else {
            // convert to ScreeniePixmapModel
            if (screenieModel.inherits(ScreenieTemplateModel::staticMetaObject.className())) {
                pixmap = scaleToTemplate(static_cast<ScreenieTemplateModel &>(screenieModel), pixmap);
            }
            ScreeniePixmapModel *screeniePixmapModel = new ScreeniePixmapModel(pixmap);
            QPointF itemPosition = calculateItemPosition(screenieModel.getPosition(), screenieModel.getSize(), pixmap.size());
            screeniePixmapModel->convert(screenieModel);
            screeniePixmapModel->setPosition(itemPosition);
            m_screenieScene.removeModel(&screenieModel);
            m_screenieScene.addModel(screeniePixmapModel);
        }
    }
}

void ScreenieControl::updateFilePathModel(const QMimeData *mimeData, ScreenieModelInterface &screenieModel)
{
    QString filePath = mimeData->urls().first().toLocalFile();
    if (screenieModel.inherits(ScreenieFilePathModel::staticMetaObject.className())) {
        ScreenieFilePathModel &filePathModel = static_cast<ScreenieFilePathModel &>(screenieModel);
        QSize oldSize = filePathModel.getSize();
        filePathModel.setFilePath(filePath);
        QPointF itemPosition = calculateItemPosition(screenieModel.getPosition(), oldSize, screenieModel.getSize());
        filePathModel.setPosition(itemPosition);
    } else {
        SizeFitter sizeFitter;
        ScreenieFilePathModel *screenieFilePathModel;
        if (screenieModel.inherits(ScreenieTemplateModel::staticMetaObject.className())) {
            sizeFitter = static_cast<ScreenieTemplateModel &>(screenieModel).getSizeFitter();
            screenieFilePathModel = new ScreenieFilePathModel(filePath, &sizeFitter);
        } else {
            screenieFilePathModel = new ScreenieFilePathModel(filePath);
        }
        QSize size = screenieFilePathModel->getSize();
        QPointF itemPosition = calculateItemPosition(screenieModel.getPosition(), screenieModel.getSize(), size);
        screenieFilePathModel->convert(screenieModel);
        screenieFilePathModel->setPosition(itemPosition);
        m_screenieScene.removeModel(&screenieModel);
        m_screenieScene.addModel(screenieFilePathModel);
    }
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
                delete screeniePixmapItem;
                break;
            }
        }
    }
}

void ScreenieControl::handleBackgroundChanged()
{
    QBrush backgroundBrush;
    if (m_screenieScene.isBackgroundEnabled()) {
        QColor backgroundColor = m_screenieScene.getBackgroundColor();
        backgroundBrush = QBrush(backgroundColor);
    } else {
        if (m_checkerBoardBrush.style() == Qt::NoBrush) {
            m_checkerBoardBrush = PaintTools::createCheckerPattern();
        }
        backgroundBrush = m_checkerBoardBrush;
    }
    m_screenieGraphicsScene.setBackgroundBrush(backgroundBrush);
}

void ScreenieControl::restoreRenderQuality()
{
    setRenderQuality(High);
}

