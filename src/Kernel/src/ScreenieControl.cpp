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
#include <QtCore/QDir>
#include <QtGui/QColor>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsItem>
#include <QtGui/QBrush>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QMainWindow>
#include <QtGui/QSlider>

#include "../../Utils/src/PaintTools.h"
#include "../../Utils/src/SizeFitter.h"
#include "../../Utils/src/Settings.h"
#include "../../Model/src/ScreenieScene.h"
#include "../../Model/src/ScreenieModelInterface.h"
#include "../../Model/src/ScreenieFilePathModel.h"
#include "../../Model/src/ScreenieImageModel.h"
#include "../../Model/src/ScreenieTemplateModel.h"
#include "TemplateOrganizer.h"
#include "Reflection.h"
#include "ScreenieGraphicsScene.h"
#include "ScreeniePixmapItem.h"

#include "ScreenieControl.h"

namespace
{
    bool zSort(const ScreeniePixmapItem *item1, const ScreeniePixmapItem *item2)
    {
        // closer items come first in the sorted list
        return item1->getScreenieModel().getDistance() > item2->getScreenieModel().getDistance();
    }
}; // anonymous

class ScreenieControlPrivate
{
public:
    ScreenieControlPrivate(ScreenieScene &theScreenieScene, ScreenieGraphicsScene &theScreenieGraphicsScene)
        : screenieScene(theScreenieScene),
          screenieGraphicsScene(theScreenieGraphicsScene),
          reflection(new Reflection()),
          templateOrganizer(theScreenieScene)
    {}

    ~ScreenieControlPrivate()
    {
        delete reflection;
    }

    ScreenieScene &screenieScene;
    ScreenieGraphicsScene &screenieGraphicsScene;
    QBrush checkerBoardBrush;
    QTimer qualityTimer;
    Reflection *reflection; /*!\todo The Reflection effect does not belong here. Add an "FX Manager" which keeps track of effects instead */
    DefaultScreenieModel defaultScreenieModel;
    TemplateOrganizer templateOrganizer;
};

// public

ScreenieControl::ScreenieControl(ScreenieScene &screenieScene, ScreenieGraphicsScene &screenieGraphicsScene)
    : QObject(),
      d(new ScreenieControlPrivate(screenieScene, screenieGraphicsScene))
{
    d->qualityTimer.setSingleShot(true);
    d->qualityTimer.setInterval(300);
    frenchConnection();
}

ScreenieControl::~ScreenieControl()
{
    delete d;
}

QList<ScreenieModelInterface *> ScreenieControl::getSelectedScreenieModels() const
{
    QList<ScreenieModelInterface *> result;
    foreach (QGraphicsItem *selectedItem, d->screenieGraphicsScene.selectedItems()) {
        if (selectedItem->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(selectedItem);
            result.append(&screeniePixmapItem->getScreenieModel());
        }
    }
    return result;
}

QList<ScreenieTemplateModel *> ScreenieControl::getSelectedTemplateModels() const
{
    QList<ScreenieTemplateModel *> result;
    foreach (QGraphicsItem *selectedItem, d->screenieGraphicsScene.selectedItems()) {
        if (selectedItem->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(selectedItem);
            ScreenieModelInterface &screenieModel = screeniePixmapItem->getScreenieModel();
            ScreenieTemplateModel *screenieTemplateModel = qobject_cast<ScreenieTemplateModel *>(&screenieModel);
            if (screenieTemplateModel != 0) {
                result.append(screenieTemplateModel);
            }
        }
    }
    return result;
}

QList<ScreenieFilePathModel *> ScreenieControl::getSelectedFilePathModels() const
{
    QList<ScreenieFilePathModel *> result;
    foreach (QGraphicsItem *selectedItem, d->screenieGraphicsScene.selectedItems()) {
        if (selectedItem->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(selectedItem);
            ScreenieModelInterface &screenieModel = screeniePixmapItem->getScreenieModel();
            ScreenieFilePathModel *screenieFilePathModel = qobject_cast<ScreenieFilePathModel *>(&screenieModel);
            if (screenieFilePathModel != 0) {
                result.append(screenieFilePathModel);
            }
        }
    }
    return result;
}

DefaultScreenieModel &ScreenieControl::getDefaultScreenieModel()
{
    return d->defaultScreenieModel;
}

void ScreenieControl::updateScene()
{
   d->screenieGraphicsScene.clear();
   handleBackgroundChanged();
   foreach (ScreenieModelInterface *screenieModel, d->screenieScene.getModels()) {
       handleModelAdded(*screenieModel);
   }
}

void ScreenieControl::updateModel(const QMimeData *mimeData, ScreenieModelInterface &screenieModel)
{
    // prefer image data over file paths (URLs)
    if (mimeData->hasImage()) {
        QImage image = qvariant_cast<QImage>(mimeData->imageData());
        updateImageModel(image, screenieModel);
    } else {
        QString filePath = mimeData->urls().first().toLocalFile();
        updateFilePathModel(filePath, screenieModel);
    }
}

ScreenieScene &ScreenieControl::getScreenieScene() const
{
    return d->screenieScene;
}

ScreenieGraphicsScene &ScreenieControl::getScreenieGraphicsScene() const
{
    return d->screenieGraphicsScene;
}

// public slots

void ScreenieControl::addImage(QString filePath, QPointF centerPosition)
{
    QStringList filePaths(filePath);
    addImages(filePaths, centerPosition);
}

void ScreenieControl::addImages(QStringList filePaths, QPointF centerPosition)
{
    QPointF position = centerPosition;
    foreach (QString filePath, filePaths) {
        ScreenieModelInterface *screenieModel = new ScreenieFilePathModel(filePath);
        applyDefaultValues(*screenieModel);
        QPointF itemPosition = calculateItemPosition(*screenieModel, position);
        position += QPointF(20.0, 20.0);
        screenieModel->setPosition(itemPosition);
        d->screenieScene.addModel(screenieModel);
    }
}

void ScreenieControl::addImage(QImage image, QPointF centerPosition)
{
    QList<QImage> images;
    images.append(image);
    addImages(images, centerPosition);
}

void ScreenieControl::addImages(QList<QImage> images, QPointF centerPosition)
{
    QPointF position = centerPosition;
    foreach (QImage image, images) {
        ScreenieImageModel *screenieModel = new ScreenieImageModel(image);
        applyDefaultValues(*screenieModel);
        QPointF itemPosition = calculateItemPosition(*screenieModel, position);
        position += QPointF(20.0, 20.0);
        screenieModel->setPosition(itemPosition);
        d->screenieScene.addModel(screenieModel);
    }
}

void ScreenieControl::addTemplate(QPointF centerPosition)
{
    QSize size = Settings::getInstance().getTemplateSize();
    ScreenieTemplateModel *screenieModel = new ScreenieTemplateModel(size);
    applyDefaultValues(*screenieModel);
    screenieModel->setPosition(centerPosition);
    d->screenieScene.addModel(screenieModel);
}

void ScreenieControl::removeAll()
{
    QList<ScreenieModelInterface *> screenieModels = getSelectedScreenieModels();
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        d->screenieScene.removeModel(screenieModel);
    }
}

void ScreenieControl::selectAll()
{
    foreach(QGraphicsItem *item, d->screenieGraphicsScene.items()) {
        item->setSelected(true);
    }
}

void ScreenieControl::setPositionX(qreal x, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setPositionX(x);
    }
}

void ScreenieControl::setPositionY(qreal y, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setPositionY(y);
    }
}

void ScreenieControl::setPosition(QPointF position, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setPosition(position);
    }
}

void ScreenieControl::translate(qreal dx, qreal dy, ScreenieModelInterface *screenieModel)
{
    bool decreaseQuality = dx != 0.0 && dy != 0.0;
    if (decreaseQuality) {
        updateEditRenderQuality();
    }
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->translate(dx, dy);
    }
}

void ScreenieControl::setRotation(int angle, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setRotation(angle);
    }
}

void ScreenieControl::rotate(int angle, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->rotate(angle);
    }
}

void ScreenieControl::setDistance(qreal distance, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setDistance(distance);
    }
}

void ScreenieControl::addDistance(qreal distance, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->addDistance(distance);
    }
}

void ScreenieControl::setReflectionEnabled(bool enable, ScreenieModelInterface *screenieModel)
{
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setReflectionEnabled(enable);
    }
}

void ScreenieControl::setReflectionOffset(int reflectionOffset, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setReflectionOffset(reflectionOffset);
    }
}

void ScreenieControl::addReflectionOffset(int reflectionOffset, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->addReflectionOffset(reflectionOffset);
    }
}

void ScreenieControl::setReflectionOpacity(int reflectionOpacity, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->setReflectionOpacity(reflectionOpacity);
    }
}

void ScreenieControl::addReflectionOpacity(int reflectionOpacity, ScreenieModelInterface *screenieModel)
{
    updateEditRenderQuality();
    QList<ScreenieModelInterface *> screenieModels = getEditableModels(screenieModel);
    foreach (ScreenieModelInterface *screenieModel, screenieModels) {
        screenieModel->addReflectionOpacity(reflectionOpacity);
    }
}

void ScreenieControl::setBackgroundEnabled(bool enable)
{
    d->screenieScene.setBackgroundEnabled(enable);
}

void ScreenieControl::setBackgroundColor(QColor color)
{
    d->screenieScene.setBackgroundColor(color);
}

void ScreenieControl::setRedBackgroundComponent(int red)
{
    QColor backgroundColor = d->screenieScene.getBackgroundColor();
    backgroundColor.setRed(red);
    d->screenieScene.setBackgroundColor(backgroundColor);
}

void ScreenieControl::setGreenBackgroundComponent(int green)
{
    QColor backgroundColor = d->screenieScene.getBackgroundColor();
    backgroundColor.setGreen(green);
    d->screenieScene.setBackgroundColor(backgroundColor);
}

void ScreenieControl::setBlueBackgroundComponent(int blue)
{
    QColor backgroundColor = d->screenieScene.getBackgroundColor();
    backgroundColor.setBlue(blue);
    d->screenieScene.setBackgroundColor(backgroundColor);
}

void ScreenieControl::setFilePath(const QString &filePath, ScreenieFilePathModel *screenieFilePathModel)
{
    QList<ScreenieFilePathModel *> screenieFilePathModels = getEditableFilePathModels(screenieFilePathModel);
    QString qtFilePath = QDir::fromNativeSeparators(filePath);
    foreach (ScreenieFilePathModel *screenieFilePathModel, screenieFilePathModels) {
        screenieFilePathModel->setFilePath(qtFilePath);
    }
}

void ScreenieControl::setTargetWidth(int width, ScreenieTemplateModel *screenieTemplateModel)
{
    QList<ScreenieTemplateModel *> screenieTemplateModels = getEditableTemplateModels(screenieTemplateModel);
    foreach (ScreenieTemplateModel *screenieTemplateModel, screenieTemplateModels) {
        screenieTemplateModel->getSizeFitter().setTargetWidth(width);
    }
}

void ScreenieControl::setTargetHeight(int height, ScreenieTemplateModel *screenieTemplateModel)
{
    QList<ScreenieTemplateModel *> screenieTemplateModels = getEditableTemplateModels(screenieTemplateModel);
    foreach (ScreenieTemplateModel *screenieTemplateModel, screenieTemplateModels) {
        screenieTemplateModel->getSizeFitter().setTargetHeight(height);
    }
}

void ScreenieControl::setFitMode(SizeFitter::FitMode fitMode, ScreenieTemplateModel *screenieTemplateModel)
{
    QList<ScreenieTemplateModel *> screenieTemplateModels = getEditableTemplateModels(screenieTemplateModel);
    foreach (ScreenieTemplateModel *screenieTemplateModel, screenieTemplateModels) {
        screenieTemplateModel->getSizeFitter().setFitMode(fitMode);
    }
}

void ScreenieControl::setFitOptionEnabled(SizeFitter::FitOption fitOption, bool enable, ScreenieTemplateModel *screenieTemplateModel)
{
    QList<ScreenieTemplateModel *> screenieTemplateModels = getEditableTemplateModels(screenieTemplateModel);
    foreach (ScreenieTemplateModel *screenieTemplateModel, screenieTemplateModels) {
        screenieTemplateModel->getSizeFitter().setFitOptionEnabled(fitOption, enable);
    }
}

void ScreenieControl::convertItemsToTemplate(ScreenieScene &screenieScene)
{
    QList<ScreenieModelInterface *> templateItems;
    QList<ScreenieModelInterface *> nonTemplateItems;
    foreach (ScreenieModelInterface *screenieModel, screenieScene.getModels()) {
        if (!qobject_cast<ScreenieTemplateModel *>(screenieModel)) {
            ScreenieTemplateModel *templateModel = new ScreenieTemplateModel();
            templateModel->convert(*screenieModel);
            templateItems.append(templateModel);
            nonTemplateItems.append(screenieModel);
        }
    }
    foreach (ScreenieModelInterface *screenieModel, nonTemplateItems) {
        screenieScene.removeModel(screenieModel);
    }
    foreach (ScreenieModelInterface *screenieModel, templateItems) {
        screenieScene.addModel(screenieModel);
    }
}

void ScreenieControl::setRenderQuality(RenderQuality renderQuality)
{
    QList<ScreeniePixmapItem *> items = getScreeniePixmapItems();
    switch (renderQuality) {
    case LowQuality:
        foreach (ScreeniePixmapItem *item, items) {
            item->setTransformationMode(Qt::FastTransformation);
        }
        foreach (QGraphicsView *view, d->screenieGraphicsScene.views()) {
            view->setRenderHints(QPainter::NonCosmeticDefaultPen);
        }
        break;
    case MaximumQuality:
        foreach (ScreeniePixmapItem *item, items) {
            item->setTransformationMode(Qt::SmoothTransformation);
        }
        foreach (QGraphicsView *view, d->screenieGraphicsScene.views()) {
            view->setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
        }
    default:
        break;
    }
}

// private

void ScreenieControl::frenchConnection()
{
    connect(&d->screenieScene, SIGNAL(distanceChanged()),
            this, SLOT(handleDistanceChanged()));
    connect(&d->screenieScene, SIGNAL(modelAdded(ScreenieModelInterface &)),
            this, SLOT(handleModelAdded(ScreenieModelInterface &)));
    connect(&d->screenieScene, SIGNAL(modelRemoved(ScreenieModelInterface &)),
            this, SLOT(handleModelRemoved(ScreenieModelInterface &)));
    connect(&d->screenieScene, SIGNAL(backgroundChanged()),
            this, SLOT(handleBackgroundChanged()));
    connect(&d->screenieGraphicsScene, SIGNAL(imagesDropped(QList<QImage>, QPointF)),
            this, SLOT(handleImageDrop(QList<QImage>, QPointF)));
    connect(&d->screenieGraphicsScene, SIGNAL(filePathsDropped(QStringList, QPointF)),
            this, SLOT(handleFilePathsDrop(QStringList, QPointF)));
    connect(&d->screenieGraphicsScene, SIGNAL(rotate(int)),
            this, SLOT(rotate(int)));
    connect(&d->screenieGraphicsScene, SIGNAL(addDistance(qreal)),
            this, SLOT(addDistance(qreal)));
    connect(&d->screenieGraphicsScene, SIGNAL(translate(qreal, qreal)),
            this, SLOT(translate(qreal, qreal)));
    connect(&d->qualityTimer, SIGNAL(timeout()),
            this, SLOT(restoreRenderQuality()));
}

QList<ScreeniePixmapItem *> ScreenieControl::getScreeniePixmapItems() const
{
    QList<ScreeniePixmapItem *> result;
    foreach (QGraphicsItem *item, d->screenieGraphicsScene.items()) {
        if (item->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(item);
            result.append(screeniePixmapItem);
        }
    }
    return result;
}

void ScreenieControl::updateEditRenderQuality()
{
    switch (Settings::getInstance().getEditRenderQuality())
    {
    case Settings::LowQuality: // fall-thru intended (for now)
    case Settings::MediumQuality:
        setRenderQuality(LowQuality);
        d->qualityTimer.start();
        break;
    case Settings::HighQuality: // fall-thru intended (for now)
    case Settings::MaximumQuality:
       break;
    default:
#ifdef DEBUG
       qCritical("ScreenieControl::updateRenderQuality: UNSUPPORTED render quality: %d", Settings::getInstance().getEditRenderQuality());
#endif
       break;
    }
}

void ScreenieControl::applyDefaultValues(ScreenieModelInterface &screenieModelInterface)
{
    screenieModelInterface.setDistance(d->defaultScreenieModel.getDistance());
    screenieModelInterface.setRotation(d->defaultScreenieModel.getRotation());
    screenieModelInterface.setReflectionEnabled(d->defaultScreenieModel.isReflectionEnabled());
    screenieModelInterface.setReflectionOffset(d->defaultScreenieModel.getReflectionOffset());
    screenieModelInterface.setReflectionOpacity(d->defaultScreenieModel.getReflectionOpacity());
}

QPointF ScreenieControl::calculateItemPosition(const ScreenieModelInterface &screenieModel, const QPointF &centerPosition)
{
    QPointF result;
    QSize itemSize = screenieModel.getSize();
    result.setX(centerPosition.x() - itemSize.width()  / 2.0);
    result.setY(centerPosition.y() - itemSize.height() / 2.0);
    return result;
}

QImage ScreenieControl::scaleToTemplate(const ScreenieTemplateModel &templateModel, const QImage &image)
{
    QImage result;
    const SizeFitter &sizeFitter = templateModel.getSizeFitter();
    QSize fittedSize;
    QRect clippedRect;
    bool doResize = sizeFitter.fit(image.size(), fittedSize, &clippedRect);
    if (doResize) {
        if (needsClipping(image.size(), clippedRect.size())) {
            result = image.copy(clippedRect);
            result = result.scaled(fittedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
        else {
            result = image.scaled(fittedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    } else {
        result = image;
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

void ScreenieControl::updateImageModel(const QImage &image, ScreenieModelInterface &screenieModel)
{
    QImage actualImage = image;
    if (!actualImage.isNull()) {
        ScreenieImageModel *screenieImageModel = qobject_cast<ScreenieImageModel *>(&screenieModel);
        if (screenieImageModel != 0) {
            QPointF itemPosition = calculateItemPosition(screenieImageModel->getPosition(), screenieImageModel->getSize(), actualImage.size());
            screenieImageModel->setImage(actualImage);
            screenieImageModel->setPosition(itemPosition);
        } else {
            // convert to ScreenieImageModel
            ScreenieTemplateModel *screenieTemplateModel = qobject_cast<ScreenieTemplateModel *>(&screenieModel);
            if (screenieTemplateModel != 0) {
                actualImage = scaleToTemplate(*screenieTemplateModel, actualImage);
            }
            ScreenieImageModel *screenieImageModel = new ScreenieImageModel(actualImage);
            QPointF itemPosition = calculateItemPosition(screenieModel.getPosition(), screenieModel.getSize(), actualImage.size());
            screenieImageModel->convert(screenieModel);
            screenieImageModel->setPosition(itemPosition);
            d->screenieScene.removeModel(&screenieModel);
            d->screenieScene.addModel(screenieImageModel);
        }
    }
}

void ScreenieControl::updateFilePathModel(const QString &filePath, ScreenieModelInterface &screenieModel)
{
    ScreenieFilePathModel *filePathModel = qobject_cast<ScreenieFilePathModel *>(&screenieModel);
    if (filePathModel != 0) {
        QSize oldSize = filePathModel->getSize();
        filePathModel->setFilePath(filePath);
        QPointF itemPosition = calculateItemPosition(filePathModel->getPosition(), oldSize, filePathModel->getSize());
        filePathModel->setPosition(itemPosition);
    } else {
        SizeFitter sizeFitter;
        ScreenieFilePathModel *screenieFilePathModel;
        ScreenieTemplateModel *screenieTemplateModel = qobject_cast<ScreenieTemplateModel *>(&screenieModel);
        if (screenieTemplateModel != 0) {
            sizeFitter = screenieTemplateModel->getSizeFitter();
            screenieFilePathModel = new ScreenieFilePathModel(filePath, &sizeFitter);
        } else {
            screenieFilePathModel = new ScreenieFilePathModel(filePath);
        }
        QSize size = screenieFilePathModel->getSize();
        QPointF itemPosition = calculateItemPosition(screenieModel.getPosition(), screenieModel.getSize(), size);
        screenieFilePathModel->convert(screenieModel);
        screenieFilePathModel->setPosition(itemPosition);
        d->screenieScene.removeModel(&screenieModel);
        d->screenieScene.addModel(screenieFilePathModel);
    }
}

bool ScreenieControl::needsClipping(const QSize &originalSize, const QSize &clippedSize)
{
    int dw, dh;  // difference of original and clipped size (width/height)
    float pw, ph; // percentage of original size (width/height)
    bool result;

    dw = originalSize.width()  - clippedSize.width();
    dh = originalSize.height() - clippedSize.height();
    pw = (100.0f * dw) / originalSize.width();
    ph = (100.0f * dh) / originalSize.height();

    /*!\todo Make threshold (currently 2%) an (advanced) user setting? */
    result = qMax(pw, ph) > 2.0;
    return result;
}

QList<ScreenieModelInterface *> ScreenieControl::getEditableModels(ScreenieModelInterface *screenieModel)
{
    QList<ScreenieModelInterface *> result;
    if (screenieModel != 0) {
        result.append(screenieModel);
    } else {
        result = getSelectedScreenieModels();
    }
    return result;
}

QList<ScreenieFilePathModel *> ScreenieControl::getEditableFilePathModels(ScreenieFilePathModel *screenieFilePathModel)
{
    QList<ScreenieFilePathModel *> result;
    if (screenieFilePathModel != 0) {
        result.append(screenieFilePathModel);
    } else {
        result = getSelectedFilePathModels();
    }
    return result;
}

QList<ScreenieTemplateModel *> ScreenieControl::getEditableTemplateModels(ScreenieTemplateModel *screenieTemplateModel)
{
    QList<ScreenieTemplateModel *> result;
    if (screenieTemplateModel != 0) {
        result.append(screenieTemplateModel);
    } else {
        result = getSelectedTemplateModels();
    }
    return result;
}

// private slots

void ScreenieControl::handleFilePathsDrop(QStringList filePaths, QPointF centerPosition)
{
    ScreenieTemplateModel *templateModel;
    if (!d->screenieScene.hasTemplates()) {
        addImages(filePaths, centerPosition);
    } else {
        QList<ScreenieTemplateModel *> templateModels = d->templateOrganizer.getOrderedTemplates();
        int i = 0;
        int n = filePaths.count();
        QList<ScreenieTemplateModel *>::const_iterator it = templateModels.constBegin();
        while (i < n && it != templateModels.constEnd()) {
            templateModel = *it;
            QString filePath = filePaths.at(i);
            updateFilePathModel(filePath, *templateModel);
            i++;
            it++;
        }
    }
}

void ScreenieControl::handleImageDrop(QList<QImage> images, QPointF centerPosition)
{
    ScreenieTemplateModel *templateModel;
    if (!d->screenieScene.hasTemplates()) {
        addImages(images, centerPosition);
    } else {
        QList<ScreenieTemplateModel *> templateModels = d->templateOrganizer.getOrderedTemplates();
        int i = 0;
        int n = images.count();
        QList<ScreenieTemplateModel *>::const_iterator it = templateModels.constBegin();
        while (i < n && it != templateModels.constEnd()) {
            templateModel = *it;
            QImage image = images.at(i);
            updateImageModel(image, *templateModel);
            i++;
            it++;
        }
    }
}

void ScreenieControl::handleDistanceChanged()
{
    QList<ScreeniePixmapItem *> screeniePixmapItems;
    foreach (QGraphicsItem *graphicsItem, d->screenieGraphicsScene.items()) {
        if (graphicsItem->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(graphicsItem);
            screeniePixmapItems.append(screeniePixmapItem);
        }
    }
    ::qSort(screeniePixmapItems.begin(), screeniePixmapItems.end(), zSort);
    int z = 0;
    foreach (QGraphicsItem *graphicsItem, screeniePixmapItems) {
        graphicsItem->setZValue(z++);
    }
}

void ScreenieControl::handleModelAdded(ScreenieModelInterface &screenieModel)
{
    ScreeniePixmapItem *screeniePixmapItem = new ScreeniePixmapItem(screenieModel, *this, *d->reflection);
    screeniePixmapItem->setPos(screenieModel.getPosition());
    d->screenieGraphicsScene.clearSelection();
    screeniePixmapItem->setSelected(true);
    d->screenieGraphicsScene.addItem(screeniePixmapItem);
    handleDistanceChanged();
}

void ScreenieControl::handleModelRemoved(ScreenieModelInterface &screenieModel)
{
    foreach (QGraphicsItem *graphicsItem, d->screenieGraphicsScene.items()) {
        if (graphicsItem->type() == ScreeniePixmapItem::ScreeniePixmapType) {
            ScreeniePixmapItem *screeniePixmapItem = static_cast<ScreeniePixmapItem *>(graphicsItem);
            if (&screenieModel == &screeniePixmapItem->getScreenieModel()) {
                d->screenieGraphicsScene.removeItem(screeniePixmapItem);
                delete screeniePixmapItem;
                break;
            }
        }
    }
}

void ScreenieControl::handleBackgroundChanged()
{
    QBrush backgroundBrush;
    if (d->screenieScene.isBackgroundEnabled()) {
        QColor backgroundColor = d->screenieScene.getBackgroundColor();
        backgroundBrush = QBrush(backgroundColor);
    } else {
        if (d->checkerBoardBrush.style() == Qt::NoBrush) {
            d->checkerBoardBrush = PaintTools::createCheckerPattern();
        }
        backgroundBrush = d->checkerBoardBrush;
    }
    d->screenieGraphicsScene.setBackgroundBrush(backgroundBrush);
}

void ScreenieControl::restoreRenderQuality()
{
    setRenderQuality(MaximumQuality);
}

