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

#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtGui/QWidget>
#include <QtGui/QFileDialog>

#include "../../../Model/src/ScreenieFilePathModel.h"
#include "../ScreenieControl.h"
#include "FilePathModelPropertiesWidget.h"
#include "ui_FilePathModelPropertiesWidget.h"

class FilePathModelPropertiesWidgetPrivate
{
public:
    FilePathModelPropertiesWidgetPrivate(ScreenieFilePathModel &filePathModel, ScreenieControl &theScreenieControl)
        : screenieFilePathModel(filePathModel),
          screenieControl(theScreenieControl)
    {}

    ScreenieFilePathModel &screenieFilePathModel;
    ScreenieControl &screenieControl;
};

// public

FilePathModelPropertiesWidget::FilePathModelPropertiesWidget(ScreenieFilePathModel &filePathModel, ScreenieControl &screenieControl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilePathModelPropertiesWidget),
    d(new FilePathModelPropertiesWidgetPrivate(filePathModel, screenieControl))
{
    ui->setupUi(this);
    updateUi();
    frenchConnection();
}

FilePathModelPropertiesWidget::~FilePathModelPropertiesWidget()
{
#ifdef DEBUG
    qDebug("FilePathModelPropertiesWidget::~FilePathModelPropertiesWidget(): called.");
#endif
    delete ui;
    delete d;
}

// private

void FilePathModelPropertiesWidget::frenchConnection()
{
    connect(&d->screenieFilePathModel, SIGNAL(changed()),
            this, SLOT(updateUi()));
}

// private slots

void FilePathModelPropertiesWidget::updateUi()
{
    ui->filePathLineEdit->setText(QDir::convertSeparators(d->screenieFilePathModel.getFilePath()));
}

void FilePathModelPropertiesWidget::on_filePathLineEdit_editingFinished()
{
    QString filePath = ui->filePathLineEdit->text();
    d->screenieControl.setFilePath(filePath, &d->screenieFilePathModel);
}

void FilePathModelPropertiesWidget::on_filePathPushButton_clicked()
{
    QString directory = QFileInfo(d->screenieFilePathModel.getFilePath()).absolutePath();
    QFileDialog *fileDialog = new QFileDialog(this, tr("Select Image"), directory);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->open(this, SLOT(handleFileSelected(QString)));
}

void FilePathModelPropertiesWidget::handleFileSelected(QString filePath)
{
    if (!filePath.isNull()) {
        ui->filePathLineEdit->setText(QDir::convertSeparators(filePath));
        d->screenieControl.setFilePath(filePath, &d->screenieFilePathModel);
    }
}
