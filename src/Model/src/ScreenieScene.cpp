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
#include <QtGui/QColor>

#include "ScreenieModelInterface.h"
#include "SceneDefaults.h"
#include "ScreenieScene.h"

class ScreenieScenePrivate
{
public:
    ScreenieScenePrivate()
        : backgroundEnabled(true),
          backgroundColor(QColor(255, 255, 255)),
          modified(false),
          isTemplate(false)
    {}

    bool backgroundEnabled;
    QColor backgroundColor;
    QList<ScreenieModelInterface *> screenieModels;
    bool modified;
    bool isTemplate;
};

// public

ScreenieScene::ScreenieScene(QObject *parent)
    : QObject(parent), d(new ScreenieScenePrivate())
{
    frenchConnection();
}

ScreenieScene::~ScreenieScene()
{
#ifdef DEBUG
    qDebug("ScreenieScene d'tor called, now deleting %d items...", d->screenieModels.count());
#endif
    qDeleteAll(d->screenieModels);
    delete d;
}

void ScreenieScene::addModel(ScreenieModelInterface *screenieModel)
{
    d->screenieModels.append(screenieModel);
    // Note: by contract we send BOTH distanceChanged() and changed() signals,
    //       when the 'screenieModel' changes distance
    connect(screenieModel, SIGNAL(distanceChanged()),
            this, SIGNAL(distanceChanged()));
    connect(screenieModel, SIGNAL(distanceChanged()),
            this, SIGNAL(changed()));
    connect(screenieModel, SIGNAL(positionChanged()),
            this, SIGNAL(changed()));
    connect(screenieModel, SIGNAL(rotationChanged()),
            this, SIGNAL(changed()));
    connect(screenieModel, SIGNAL(reflectionChanged()),
            this, SIGNAL(changed()));
    connect(screenieModel, SIGNAL(changed()),
            this, SIGNAL(changed()));
    connect(screenieModel, SIGNAL(selectionChanged()),
            this, SIGNAL(selectionChanged()));
    emit modelAdded(*screenieModel);
}

void ScreenieScene::removeModel(ScreenieModelInterface *screenieModel)
{
    int index = d->screenieModels.indexOf(screenieModel);
    removeModel(index);
}

void ScreenieScene::removeModel(int index)
{
    ScreenieModelInterface *screenieModel = d->screenieModels.at(index);
    d->screenieModels.removeAt(index);
    emit modelRemoved(*screenieModel);
    delete screenieModel;
}

ScreenieModelInterface *ScreenieScene::getModel(int index) const
{
    ScreenieModelInterface *result;
    int n = count();
    if (index >= 0 && index < n) {
        result = d->screenieModels.at(index);
    } else {
        result = 0;
    }
    return result;
}

const QList<ScreenieModelInterface *> &ScreenieScene::getModels() const
{
    return d->screenieModels;
}

QList<ScreenieModelInterface *> ScreenieScene::getSelectedModels() const
{
    QList<ScreenieModelInterface *> result;
    foreach (ScreenieModelInterface *screenieModel, d->screenieModels) {
        if (screenieModel->isSelected()) {
            result.append(screenieModel);
        }
    }
#ifdef DEBUG
    qDebug(" ScreenieScene::getSelectedModels: count: %d", result.count());
#endif
    return result;
}

int ScreenieScene::count() const
{
    return d->screenieModels.count();
}

bool ScreenieScene::isBackgroundEnabled() const
{
    return d->backgroundEnabled;
}

void ScreenieScene::setBackgroundEnabled(bool enable) {
    if (d->backgroundEnabled != enable) {
        d->backgroundEnabled = enable;
        emit backgroundChanged();
    }
}

QColor ScreenieScene::getBackgroundColor() const {
     return d->backgroundColor;
}

void ScreenieScene::setBackgroundColor(QColor color) {
    if (d->backgroundColor != color) {
        d->backgroundColor = color;
        emit backgroundChanged();
    }
}

bool ScreenieScene::hasTemplates() const
{
    bool result = false;
    foreach (ScreenieModelInterface *screenieModel, d->screenieModels) {
        if (screenieModel->isTemplate()) {
            result = true;
            break;
        }
    }
    return result;
}

bool ScreenieScene::hasTemplatesExclusively() const
{
    bool result = true;
    foreach (ScreenieModelInterface *screenieModel, d->screenieModels) {
        if (!screenieModel->isTemplate()) {
            result = false;
            break;
        }
    }
    return result;
}

bool ScreenieScene::isTemplate() const
{
    return d->isTemplate;
}

void ScreenieScene::setTemplate(bool enable)
{
    d->isTemplate = enable;
}

bool ScreenieScene::isModified() const
{
    return d->modified;
}

void ScreenieScene::setModified(bool modified)
{
    d->modified = modified;
}

bool ScreenieScene::isDefault() const
{
    bool result;
    result = d->screenieModels.count() == 0 &&
             d->backgroundColor == SceneDefaults::BackgroundColor &&
             d->backgroundEnabled == SceneDefaults::BackgroundEnabled;
    return result;
}

// private

void ScreenieScene::frenchConnection()
{
    connect(this, SIGNAL(backgroundChanged()),
            this, SIGNAL(changed()));
    connect(this, SIGNAL(modelAdded(ScreenieModelInterface&)),
            this, SIGNAL(changed()));
    connect(this, SIGNAL(modelRemoved(ScreenieModelInterface&)),
            this, SIGNAL(changed()));
    connect(this, SIGNAL(changed()),
            this, SLOT(handleChanged()));
}

// private slots

void ScreenieScene::handleChanged()
{
    setModified(true);
}
