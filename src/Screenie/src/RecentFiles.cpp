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
#include <QtCore/QFileInfo>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QKeySequence>

#include "../../Utils/src/Settings.h"
#include "RecentFiles.h"

namespace
{
    const Qt::Key ActionKeys[] = {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4, Qt::Key_5, Qt::Key_6, Qt::Key_7, Qt::Key_8, Qt::Key_9, Qt::Key_0};
}

// **************
// public methods
// **************

RecentFiles::RecentFiles()
    : m_recentFilesActionGroup(new QActionGroup(this)),
    m_ignoreUpdateSignals(false)
{
    initialise();
    frenchConnection();
}

RecentFiles::~RecentFiles()
{}

QActionGroup &RecentFiles::getRecentFilesActionGroup() const
{
    return *m_recentFilesActionGroup;
}

// ***************
// private methods
// ***************

void RecentFiles::initialise()
{
    QAction *action;
    Settings &settings = Settings::getInstance();
    for (int i = 0; i < settings.getMaxRecentFiles(); i++) {
        action = new QAction(this);
        action->setVisible(false);
        connect(action, SIGNAL(triggered()),
                this, SLOT(handleRecentFilesAction()));
        m_recentFilesActionGroup->addAction(action);
    }
    updateRecentFilesActions();

    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    m_recentFilesActionGroup->addAction(separator);

    m_clearRecentFilesAction = new QAction(tr("Clear &menu"), this);
    m_recentFilesActionGroup->addAction(m_clearRecentFilesAction);
}

void RecentFiles::frenchConnection()
{
    Settings &settings = Settings::getInstance();

    connect(&settings, SIGNAL(changed()),
            this, SLOT(updateRecentFilesActions()));
    connect(m_clearRecentFilesAction, SIGNAL(triggered()),
            this, SLOT(clearRecentFilesMenu()));

}

// *************
// private slots
// *************

void RecentFiles::updateRecentFilesActions()
{
    Settings &settings = Settings::getInstance();
    int i;

    if (m_ignoreUpdateSignals) {
        return;
    }

    QStringList recentFiles = settings.getRecentFiles();

    const int originalSize = recentFiles.count();
    int nofRecentFiles = qMin(recentFiles.count(), settings.getMaxRecentFiles());
    const QList<QAction *> recentFilesActions = m_recentFilesActionGroup->actions();

    for (i = 0; i < nofRecentFiles; ++i) {
        const QFileInfo fileInfo(recentFiles[i]);

        if (fileInfo.exists() == false) {
            recentFiles.removeAt(i);
            --i;
            nofRecentFiles = qMin(recentFiles.count(), settings.getMaxRecentFiles());
            continue;
        }
        const QString text = fileInfo.fileName();
        recentFilesActions[i]->setText(text);
        recentFilesActions[i]->setData(recentFiles[i]);
        recentFilesActions[i]->setVisible(true);
        recentFilesActions[i]->setShortcut(QKeySequence(ActionKeys[i] | Qt::CTRL));
    }

    // set all not yet used actions to invisible
    for (i = nofRecentFiles; i < settings.getMaxRecentFiles(); ++i) {
        recentFilesActions[i]->setVisible(false);
    }

    // If there's been a change, write it back
    if (originalSize != recentFiles.count()) {
        m_ignoreUpdateSignals = true;
        settings.setRecentFiles(recentFiles);
        m_ignoreUpdateSignals = false;
    }
}

void RecentFiles::handleRecentFilesAction()
{
    Settings &settings = Settings::getInstance();
    if (const QAction *action = qobject_cast<const QAction *>(sender())) {
        QString recentFilePath = action->data().toString();
        m_ignoreUpdateSignals = true;
        settings.removeRecentFile(recentFilePath);
        m_ignoreUpdateSignals = false;
        settings.addRecentFile(recentFilePath);
#ifdef DEBUG
        qDebug("RecentFiles::handleRecentFilesAction: %s", qPrintable(recentFilePath));
#endif
        emit openRecentFile(recentFilePath);
    }
}

void RecentFiles::clearRecentFilesMenu()
{
    Settings &settings = Settings::getInstance();
    settings.setRecentFiles(QStringList());
}
