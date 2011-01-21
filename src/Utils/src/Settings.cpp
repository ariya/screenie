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

#include <QtCore/QSize>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtGui/QDesktopServices>
#include <QtGui/QMainWindow>

#include "Version.h"
#include "Settings.h"

class SettingsPrivate
{
public:
    static Settings *instance;

    static const QSize DefaultMaximumImageSize;
    static const QSize DefaultTemplateSize;
    static const QString DefaultLastImageDirectoryPath;
    static const QString DefaultLastExportDirectoryPath;
    static const QString DefaultLastDocumentDirectoryPath;
    static const qreal DefaultRotationGestureSensitivity;
    static const qreal DefaultDistanceGestureSensitivity;
    static const int DefaultMaxRecentFiles;
    static const bool DefaultFullScreen;
    static const QPoint DefaultMainWindowPosition;
    static const QSize DefaultMainWindowSize;

    Version version;
    QSize maximumImageSize;
    QSize templateSize;
    QString lastImageDirectoryPath;
    QString lastExportDirectoryPath;
    QString lastDocumenDirectoryPath;
    qreal rotationGestureSensitivity;
    qreal distanceGestureSensitivity;
    int maxRecentFiles;
    QStringList recentFiles;

    QSettings *settings;

    SettingsPrivate() : maximumImageSize(DefaultMaximumImageSize) {
#ifdef Q_OS_WIN
        // On Windows prefer INI format over Registry (= NativeFormat)
        QSettings::Format format = QSettings::IniFormat;
#else
        QSettings::Format format = QSettings::NativeFormat;
#endif
        settings = new QSettings(format, QSettings::UserScope, "till-art.net", Version::getApplicationName());
    }

    ~SettingsPrivate() {
        delete settings;
    }
};

Settings *SettingsPrivate::instance = 0;

const QSize SettingsPrivate::DefaultMaximumImageSize = QSize(1024, 1024);
const QSize SettingsPrivate::DefaultTemplateSize = QSize(400, 300);
// workaround for http://bugreports.qt.nokia.com/browse/QTBUG-3239: use fromNativeSeparators
const QString SettingsPrivate::DefaultLastImageDirectoryPath = QDir::fromNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::PicturesLocation));
const QString SettingsPrivate::DefaultLastExportDirectoryPath = SettingsPrivate::DefaultLastImageDirectoryPath;
const QString SettingsPrivate::DefaultLastDocumentDirectoryPath = QDir::fromNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
const qreal SettingsPrivate::DefaultRotationGestureSensitivity = 2.0; // these values work well on a MacBook Pro ;)
const qreal SettingsPrivate::DefaultDistanceGestureSensitivity = 10.0;
const int SettingsPrivate::DefaultMaxRecentFiles = 8;
const bool SettingsPrivate::DefaultFullScreen = false;
const QPoint SettingsPrivate::DefaultMainWindowPosition = QPoint();
const QSize SettingsPrivate::DefaultMainWindowSize = QSize(800, 600);

// public

Settings &Settings::getInstance()
{
    if (SettingsPrivate::instance == 0) {
        SettingsPrivate::instance = new Settings();
    }
    return *SettingsPrivate::instance;
}

void Settings::destroyInstance() {
    if (SettingsPrivate::instance != 0) {
        delete SettingsPrivate::instance;
        SettingsPrivate::instance = 0;
    }
}

const QSize &Settings::getMaximumImageSize() const {
    return d->maximumImageSize;
}

void Settings::setMaximumImageSize(const QSize &maximumImageSize)
{
    if (d->maximumImageSize != maximumImageSize) {
        d->maximumImageSize = maximumImageSize;
        emit changed();
    }
}

const QSize &Settings::getTemplateSize() const {
    return d->templateSize;
}

void Settings::setTemplateSize(const QSize &templateSize)
{
    if (d->templateSize != templateSize) {
        d->templateSize = templateSize;
        emit changed();
    }
}

const QString &Settings::getLastImageDirectoryPath() const
{
    return d->lastImageDirectoryPath;
}

void Settings::setLastImageDirectoryPath(const QString &lastImageDirectoryPath)
{
    QString qtPath = QDir::fromNativeSeparators(lastImageDirectoryPath);
    if (d->lastImageDirectoryPath != qtPath) {
        d->lastImageDirectoryPath = qtPath;
        emit changed();
    }
}

const QString &Settings::getLastExportDirectoryPath() const
{
    return d->lastExportDirectoryPath;
}

void Settings::setLastExportDirectoryPath(const QString &lastExportDirectoryPath)
{
    QString qtPath = QDir::fromNativeSeparators(lastExportDirectoryPath);
    if (d->lastExportDirectoryPath != qtPath) {
        d->lastExportDirectoryPath = qtPath;
        emit changed();
    }
}

const QString &Settings::getLastDocumentDirectoryPath() const
{
    return d->lastDocumenDirectoryPath;
}

void Settings::setLastDocumentDirectoryPath(const QString &lastDocumentDirectoryPath)
{
    QString qtPath = QDir::fromNativeSeparators(lastDocumentDirectoryPath);
    if (d->lastDocumenDirectoryPath != qtPath) {
        d->lastDocumenDirectoryPath = qtPath;
        emit changed();
    }
}

qreal Settings::getRotationGestureSensitivity() const
{
   return d->rotationGestureSensitivity;
}

void Settings::setRotationGestureSensitivity(qreal rotationGestureSensitivity)
{
    if (d->rotationGestureSensitivity != rotationGestureSensitivity) {
        d->rotationGestureSensitivity = rotationGestureSensitivity;
        emit changed();
    }
}

qreal Settings::getDistanceGestureSensitivity() const
{
   return d->distanceGestureSensitivity;
}

void Settings::setDistanceGestureSensitivity(qreal distanceGestureSensitivity)
{
    if (d->distanceGestureSensitivity != distanceGestureSensitivity) {
        d->distanceGestureSensitivity = distanceGestureSensitivity;
        emit changed();
    }
}

int Settings::getMaxRecentFiles() const {
    return d->maxRecentFiles;
}

void Settings::setMaxRecentFiles(int maxRecentFiles)
{
    if (d->maxRecentFiles != maxRecentFiles) {
        if (d->maxRecentFiles > maxRecentFiles) {
            while (d->recentFiles.count() > maxRecentFiles) {
                d->recentFiles.removeLast();
            }
        }
        d->maxRecentFiles = maxRecentFiles;
        emit changed();
    }
}

void Settings::setRecentFiles(const QStringList &recentFiles) {
    if (d->recentFiles != recentFiles) {
        d->recentFiles = recentFiles;
        emit changed();
    }
}

void Settings::addRecentFile(const QString &filePath)
{
    if (d->recentFiles.contains(filePath) == false) {
        d->recentFiles.prepend(filePath);
        if (d->recentFiles.count() > d->maxRecentFiles) {
            d->recentFiles.removeLast();
        }
        emit changed();
    }
}

void Settings::removeRecentFile(const QString &filePath)
{
    int index = d->recentFiles.indexOf(filePath);
    if (index != -1) {
        d->recentFiles.removeAt(index);
        emit changed();
    }
}

QStringList Settings::getRecentFiles() const
{
    return d->recentFiles;
}

Settings::WindowGeometry Settings::getWindowGeometry() const
{
    WindowGeometry result;
    d->settings->beginGroup("UI/MainWindow");
    {
        result.fullScreen = d->settings->value("FullScreen", SettingsPrivate::DefaultFullScreen).toBool();
        result.position = d->settings->value("Position", SettingsPrivate::DefaultMainWindowPosition).toPoint();
        result.size = d->settings->value("Size", SettingsPrivate::DefaultMainWindowSize).toSize();
    }
    d->settings->endGroup();
    return result;
}

void Settings::setWindowGeometry(const WindowGeometry windowGeometry)
{
    d->settings->beginGroup("UI/MainWindow");
    {
        d->settings->setValue("FullScreen", windowGeometry.fullScreen);
        d->settings->setValue("Position", windowGeometry.position);
        d->settings->setValue("Size", windowGeometry.size);
    }
    d->settings->endGroup();
}

// public slots

void Settings::store()
{
    d->settings->setValue("Version", d->version.toString());
    d->settings->setValue("Scene/MaximumImageSize", d->maximumImageSize);
    d->settings->setValue("Scene/TemplateSize", d->templateSize);
    d->settings->beginGroup("Paths");
    {
        d->settings->setValue("LastImageDirectoryPath", d->lastImageDirectoryPath);
        d->settings->setValue("LastExportDirectoryPath", d->lastExportDirectoryPath);
        d->settings->setValue("LastDocumentDirectoryPath", d->lastDocumenDirectoryPath);
        d->settings->beginGroup("Recent");
        {
            d->settings->setValue("MaxRecentFiles", d->maxRecentFiles);
            d->settings->setValue("RecentFiles", d->recentFiles);
        }
    }
    d->settings->endGroup();
    d->settings->beginGroup("UI/Gestures");
    {
        d->settings->setValue("RotationSensitivity", d->rotationGestureSensitivity);
        d->settings->setValue("DistanceSensitivity", d->distanceGestureSensitivity);
    }
    d->settings->endGroup();
}

void Settings::restore()
{
    QString version;
    Version appVersion;
    version = d->settings->value("Version", appVersion.toString()).toString();
    Version settingsVersion(version);
    if (settingsVersion < appVersion) {
#ifdef DEBUG
        qDebug("Settings::restore: app version: %s, settings version: %s, conversion might be necessary!",
               qPrintable(appVersion.toString()), qPrintable(settingsVersion.toString()));
        /*!\todo Settings conversion as necessary */
#endif
    }
    d->maximumImageSize = d->settings->value("Scene/MaximumImageSize", SettingsPrivate::DefaultMaximumImageSize).toSize();
    d->templateSize = d->settings->value("Scene/TemplateSize", SettingsPrivate::DefaultTemplateSize).toSize();
    d->settings->beginGroup("Paths");
    {
        d->lastImageDirectoryPath = d->settings->value("LastImageDirectoryPath", SettingsPrivate::DefaultLastImageDirectoryPath).toString();
        d->lastExportDirectoryPath = d->settings->value("LastExportDirectoryPath", SettingsPrivate::DefaultLastExportDirectoryPath).toString();
        d->lastDocumenDirectoryPath = d->settings->value("LastDocumentDirectoryPath", SettingsPrivate::DefaultLastDocumentDirectoryPath).toString();
        d->settings->beginGroup("Recent");
        {
            d->maxRecentFiles = d->settings->value("MaxRecentFiles", SettingsPrivate::DefaultMaxRecentFiles).toInt();
            d->recentFiles = d->settings->value("RecentFiles", QStringList()).toStringList();
        }
        d->settings->endGroup();
    }
    d->settings->endGroup();
    d->settings->beginGroup("UI/Gestures");
    {
        d->rotationGestureSensitivity = d->settings->value("RotationSensitivity", SettingsPrivate::DefaultRotationGestureSensitivity).toReal();
        d->distanceGestureSensitivity = d->settings->value("DistanceSensitivity", SettingsPrivate::DefaultDistanceGestureSensitivity).toReal();
    }
    d->settings->endGroup();
}

// protected

Settings::~Settings()
{
    store();
    delete d;
}

// private

Settings::Settings()
{
    d = new SettingsPrivate();
    restore();
}
