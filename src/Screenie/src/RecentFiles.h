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

#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QtCore/QObject>

class QActionGroup;
class QAction;

/*!
 * \brief The recently opened files.
 *
 * Provides a menu with QActions which give access to the recently opened files.
 */
class RecentFiles : public QObject
{
    Q_OBJECT

public:
    RecentFiles();
    virtual ~RecentFiles();

    QActionGroup &getRecentFilesActionGroup() const;

signals:
    void openRecentFile(const QString &filePath);

private:
    QActionGroup *m_recentFilesActionGroup;
    bool m_ignoreUpdateSignals;
    QAction *m_clearRecentFilesAction;

    void initialise();
    void frenchConnection();

private slots:
    void updateRecentFilesActions();
    void handleRecentFilesAction();
    void clearRecentFilesMenu();
};

#endif // RECENTFILES_H
