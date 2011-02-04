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

#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QtCore/QObject>

#include "KernelLib.h"

struct DocumentInfo;
class DocumentManagerPrivate;

/*!
 * Manages the open documents (MainWindow).
 */
class DocumentManager : public QObject
{
    Q_OBJECT
public:
    KERNEL_API static DocumentManager &getInstance();
    KERNEL_API static void destroyInstance();

    /*!
     * Adds \p documentInfo to the managed documents. This DocumentManager takes ownership
     * of \p documentInfo which must be allocated on the heap. The \em object name of the
     * corresponding QMainWindow in \c documentInfo is updated. It is used to identify the
     * window later on.
     *
     * \param documentInfo
     *        the DocumentInfo to be managed; ownership is taken by this DocumentManager
     */
    KERNEL_API void add(const DocumentInfo *documentInfo);
    KERNEL_API const QList<const DocumentInfo *> &getDocumentInfos() const;
    KERNEL_API int count() const;
    KERNEL_API void activate(int id) const;
    KERNEL_API int getModifiedCount() const;

signals:
    void changed();

protected:
    virtual ~DocumentManager();

private:
    Q_DISABLE_COPY(DocumentManager)
    DocumentManagerPrivate *d;

    DocumentManager();

private slots:
    void remove(QObject *mainWindow);
};

#endif // DOCUMENTMANAGER_H
