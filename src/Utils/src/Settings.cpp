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
#include <QtCore/QString>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtGui/QDesktopServices>

#include "Version.h"
#include "Settings.h"

class SettingsPrivate
{
public:
    static Settings *instance;

    static const QSize DefaultMaximumImageSize;
    static const QString DefaultLastImageDirectoryPath;
    static const QString DefaultLastExportDirectoryPath;

    QSize maximumImageSize;
    QString lastImageDirectoryPath;
    QString lastExportDirectoryPath;
    Version version;

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

const QSize SettingsPrivate::DefaultMaximumImageSize = QSize(640, 640);
const QString SettingsPrivate::DefaultLastImageDirectoryPath = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
const QString SettingsPrivate::DefaultLastExportDirectoryPath = SettingsPrivate::DefaultLastImageDirectoryPath;

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

// public slots

void Settings::store()
{
    d->settings->setValue("Version", d->version.toString());
    d->settings->setValue("Scene/MaximumImageSize", d->maximumImageSize);
    d->settings->setValue("Paths/LastImageDirectoryPath", d->lastImageDirectoryPath);
    d->settings->setValue("Paths/LastExportDirectoryPath", d->lastExportDirectoryPath);
}

void Settings::restore()
{
    QString version;
    Version appVersion;
    version = d->settings->value("Version", appVersion.toString()).toString();
    Version settingsVersion(version);
    if (settingsVersion < appVersion) {
#ifdef DEBUG
        qDebug("Settings::restore: app version: %s, settings version: %s, conversion necessary!",
               qPrintable(appVersion.toString()), qPrintable(settingsVersion.toString()));
        /*!\todo Settings conversion as necessary */
#endif
    }
    d->maximumImageSize = d->settings->value("Scene/maximumImageSize", SettingsPrivate::DefaultMaximumImageSize).toSize();
    d->lastImageDirectoryPath = d->settings->value("Paths/LastImageDirectoryPath", SettingsPrivate::DefaultLastImageDirectoryPath).toString();
    d->lastExportDirectoryPath = d->settings->value("Paths/LastExportDirectoryPath", SettingsPrivate::DefaultLastExportDirectoryPath).toString();
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
