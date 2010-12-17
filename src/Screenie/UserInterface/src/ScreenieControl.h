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

#ifndef SCREENIECONTROL_H
#define SCREENIECONTROL_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QPointF>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QPixmap>

class QStringList;
class QMimeData;

class ScreenieModelInterface;
class ScreenieScene;
class ScreenieGraphicsScene;
class ScreeniePixmapItem;
class Reflection;

#include "../../../Model/src/DefaultScreenieModel.h"

/*!
 * This is the controller of the ScreenieScene (model): it modifies the <em>selected</em> ScreenieModelInterface.
 */
class ScreenieControl : public QObject
{
    Q_OBJECT

public:
    /*!
     * Defines the QGraphicsView render quality.
     */
    enum RenderQuality
    {
        Low, /*!< Antialiasing disabled, fast pixmap transformation */
        High /*!< Antialiasing enabled, smooth pixmap transformation */
    };

    ScreenieControl(ScreenieScene &screenieScene, ScreenieGraphicsScene &screenieGraphicsScene);
    virtual ~ScreenieControl();

    QList<ScreenieModelInterface *> getSelectedScreenieModels() const;

    DefaultScreenieModel &getDefaultScreenieModel();

    void updateData(const QMimeData *mimeData, ScreenieModelInterface &screenieModel);

public slots:
    void addImage(QString filePath, QPointF position);
    void addImages(QStringList filePaths, QPointF position);
    void addImage(QPixmap pixmap, QPointF position);
    void addImages(QList<QPixmap> pixmaps, QPointF position);
    void addTemplate(QPointF position);

    void setRotation(int angle);
    void rotate(int angle);    
    void setPosition(QPointF position);
    void setDistance(int distance);
    void addDistance(int distance);

    void setReflectionEnabled(bool enable);
    void setReflectionOffset(int reflectionOffset);
    void addReflectionOffset(int reflectionOffset);
    void setReflectionOpacity(int reflectionOpacity);
    void addReflectionOpacity(int reflectionOpacity);

    void setBackgroundEnabled(bool enable);
    void setBackgroundColor(QColor color);
    void setRedBackgroundComponent(int red);
    void setGreenBackgroundComponent(int green);
    void setBlueBackgroundComponent(int blue);

private:
    ScreenieScene &m_screenieScene;
    ScreenieGraphicsScene &m_screenieGraphicsScene;
    QBrush m_checkerBoard;
    QTimer m_qualityTimer;
    Reflection *m_reflection; /*! \todo The Reflection effect does not belong here. Add an "FX Manager" which keeps track of effects instead */
    DefaultScreenieModel m_defaultScreenieModel;

    void frenchConnection();

    QList<ScreeniePixmapItem *> getScreeniePixmapItems() const;
    void setRenderQuality(RenderQuality renderQuality);
    void applyDefaultValues(ScreenieModelInterface &screenieModelInterface);

private slots:
    void handleDistanceChanged();
    void handleModelAdded(ScreenieModelInterface &screenieModel);
    void handleModelRemoved(const ScreenieModelInterface &screenieModel);
    void handleBackgroundChanged();
    void removeItems();
    void restoreRenderQuality();
};

#endif // SCREENIECONTROL_H
