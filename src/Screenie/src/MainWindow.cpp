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

#include <QtCore/QString>
#include <QtCore/QPointF>
#include <QtCore/QFileInfo>
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtGui/QColor>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QFileDialog>
#include <QtGui/QSlider>
#include <QtGui/QMessageBox>
#include <QtGui/QShortcut>
#include <QtGui/QKeySequence>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFormat>

#include "../../Utils/src/Settings.h"
#include "../../Model/src/ScreenieScene.h"
#include "../../Model/src/ScreenieModelInterface.h"
#include "../../Model/src/ScreenieFilePathModel.h"
#include "../../Model/src/Dao/ScreenieSceneDao.h"
#include "../../Model/src/Dao/Xml/XmlScreenieSceneDao.h"
#include "../../Kernel/src/ExportImage.h"
#include "../../Kernel/src/Clipboard/Clipboard.h"
#include "../../Kernel/src/ScreenieControl.h"
#include "../../Kernel/src/ScreenieGraphicsScene.h"
#include "../../Kernel/src/ScreeniePixmapItem.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

// public

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_ignoreUpdateSignals(false)
{
    ui->setupUi(this);
    ui->distanceSlider->setMaximum(ScreenieModelInterface::MaxDistance);

    /*!\todo Move shortcut assignment to separate class, make use of "standard platform shortcuts",
             as provided by Qt: http://doc.trolltech.com/latest/qkeysequence.html#StandardKey-enum */
#ifdef Q_OS_MAC
    // Also refer to http://en.wikipedia.org/wiki/Table_of_keyboard_shortcuts
    // (or: http://doc.trolltech.com/latest/qkeysequence.html#standard-shortcuts)
    // For Mac there does not seem to be a common shortcut for fullscreen
    // (unlike F11 for other platforms), but iTunes uses Meta + F
    // Note: by default on Mac Qt swaps CTRL and META
    ui->toggleFullScreenAction->setShortcut(QKeySequence(Qt::Key_F + Qt::CTRL));
#endif
    QShortcut *shortcut = new QShortcut(QKeySequence(tr("Backspace", "Edit|Delete")), this);
    connect(shortcut, SIGNAL(activated()),
            ui->deleteAction, SIGNAL(triggered()));

    m_screenieGraphicsScene = new ScreenieGraphicsScene(this);
    ui->graphicsView->setScene(m_screenieGraphicsScene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->graphicsView->setAcceptDrops(true);

    // Gesture support
    ui->graphicsView->viewport()->grabGesture(Qt::PinchGesture);
    ui->graphicsView->viewport()->grabGesture(Qt::PanGesture);

    // later: OpenGL support (configurable)
    // ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

    m_screenieScene = new ScreenieScene();
    m_screenieControl = new ScreenieControl(*m_screenieScene, *m_screenieGraphicsScene);
    m_clipboard = new Clipboard(*m_screenieControl, this);

    initializeUi();
    updateUi();
    setUnifiedTitleAndToolBarOnMac(true);
    frenchConnection();
}

MainWindow::~MainWindow()
{
    delete m_screenieScene;
    delete m_screenieControl;
    delete ui;

    // destroy singletons
    Settings::destroyInstance();
}

// private

void MainWindow::frenchConnection()
{
    connect(m_screenieGraphicsScene, SIGNAL(changed(QList<QRectF>)),
            this, SLOT(handleGraphicsSceneChanged()));
    connect(m_screenieGraphicsScene, SIGNAL(selectionChanged()),
            this, SLOT(updateUi()));
    connect(m_screenieScene, SIGNAL(changed()),
            this, SLOT(updateUi()));
    connect(m_clipboard, SIGNAL(dataChanged()),
            this, SLOT(updateUi()));
}

bool MainWindow::save(const QString &filePath)
{
    ScreenieSceneDao *screenieSceneDao = new XmlScreenieSceneDao(filePath);
    return screenieSceneDao->write(*m_screenieScene);
}

void MainWindow::updateTransformationUi()
{
    QList<ScreenieModelInterface *> screenieModels = m_screenieControl->getSelectedScreenieModels();
    if (screenieModels.size() > 0) {
        ScreenieModelInterface *lastSelection = screenieModels.last();
        // update GUI elements according to last selected item. In case of multiple selections
        // we don't want to take the values take effect on the previously selected items while
        // selecting more items (CTRL + left-click), hence the signals are blocked
        ui->rotationSlider->blockSignals(true);
        ui->rotationSlider->setValue(lastSelection->getRotation());
        ui->rotationSlider->blockSignals(false);
        ui->rotationSlider->setEnabled(true);

        ui->distanceSlider->blockSignals(true);
        ui->distanceSlider->setValue(lastSelection->getDistance());
        ui->distanceSlider->blockSignals(false);
        ui->distanceSlider->setEnabled(true);
    } else {
        ui->rotationSlider->setEnabled(false);
        ui->distanceSlider->setEnabled(false);
    }
}

void MainWindow::updateReflectionUi()
{
    QList<ScreenieModelInterface *> screenieModels = m_screenieControl->getSelectedScreenieModels();
    if (screenieModels.size() > 0) {
        ScreenieModelInterface *lastSelection = screenieModels.last();
        // update GUI elements according to last selected item. In case of multiple selections
        // we don't want to take the values take effect on the previously selected items while
        // selecting more items (CTRL + left-click), hence the signals are blocked
        ui->reflectionGroupBox->blockSignals(true);
        ui->reflectionGroupBox->setChecked(lastSelection->isReflectionEnabled());
        ui->reflectionGroupBox->blockSignals(false);
        ui->reflectionGroupBox->setEnabled(true);

        ui->reflectionOffsetSlider->blockSignals(true);
        ui->reflectionOffsetSlider->setValue(lastSelection->getReflectionOffset());
        ui->reflectionOffsetSlider->blockSignals(false);
        ui->reflectionOffsetSlider->setEnabled(true);

        ui->reflectionOpacitySlider->blockSignals(true);
        ui->reflectionOpacitySlider->setValue(lastSelection->getReflectionOpacity());
        ui->reflectionOpacitySlider->blockSignals(false);
        ui->reflectionOpacitySlider->setEnabled(true);
    } else {
        ui->reflectionGroupBox->setEnabled(false);
        ui->reflectionOffsetSlider->setEnabled(false);
        ui->reflectionOpacitySlider->setEnabled(false);
    }
}

void MainWindow::updateColorUi()
{
    ui->backgroundColorGroupBox->blockSignals(true);
    ui->backgroundColorGroupBox->setChecked(m_screenieScene->isBackgroundEnabled());
    ui->backgroundColorGroupBox->blockSignals(false);
    QColor backgroundColor = m_screenieScene->getBackgroundColor();
    ui->redSlider->blockSignals(true);
    ui->redSlider->setValue(backgroundColor.red());
    ui->redSlider->blockSignals(false);
    ui->greenSlider->blockSignals(true);
    ui->greenSlider->setValue(backgroundColor.green());
    ui->greenSlider->blockSignals(false);
    ui->blueSlider->blockSignals(true);
    ui->blueSlider->setValue(backgroundColor.blue());
    ui->blueSlider->blockSignals(false);
}

void MainWindow::updateEditActions()
{
    bool hasItems = m_screenieGraphicsScene->items().size() > 0;
    bool hasSelection = m_screenieGraphicsScene->selectedItems().size() > 0;
    ui->cutAction->setEnabled(hasSelection);
    ui->copyAction->setEnabled(hasSelection);
    ui->pasteAction->setEnabled(m_clipboard->hasData());
    ui->deleteAction->setEnabled(hasSelection);
    ui->selectAllAction->setEnabled(hasItems);
}

void MainWindow::initializeUi()
{
    DefaultScreenieModel &defaultScreenieModel = m_screenieControl->getDefaultScreenieModel();
    ui->distanceSlider->setValue(defaultScreenieModel.getDistance());
    ui->rotationSlider->setValue(defaultScreenieModel.getRotation());
    ui->reflectionGroupBox->setChecked(defaultScreenieModel.isReflectionEnabled());
    ui->reflectionOffsetSlider->setValue(defaultScreenieModel.getReflectionOffset());
    ui->reflectionOpacitySlider->setValue(defaultScreenieModel.getReflectionOpacity());
}

// private slots

void MainWindow::on_saveAsAction_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save As"), QString(), "*.xsc");
    if (!filePath.isNull()) {
        bool ok = save(filePath);
#ifdef DEBUG
        qDebug("MainWindow::on_saveAsAction_triggered: ok: %d", ok);
#endif
    }
}

void MainWindow::on_exportAction_triggered()
{
    Settings &settings = Settings::getInstance();
    QString lastExportDirectoryPath = settings.getLastExportDirectoryPath();
    QString filter = tr("Portable Network Graphics (*.png)");
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export Image"), lastExportDirectoryPath, filter);
    if (!filePath.isNull()) {
        ExportImage exportImage(*m_screenieScene, *m_screenieGraphicsScene);
        exportImage.exportImage(filePath);
        lastExportDirectoryPath = QFileInfo(filePath).absolutePath();
        settings.setLastExportDirectoryPath(lastExportDirectoryPath);
    }
}

void MainWindow::on_cutAction_triggered()
{
    m_clipboard->cut();
}

void MainWindow::on_copyAction_triggered()
{
    m_clipboard->copy();
}

void MainWindow::on_pasteAction_triggered()
{
    m_clipboard->paste();
}

void MainWindow::on_deleteAction_triggered()
{
    m_screenieControl->removeAll();
}

void MainWindow::on_selectAllAction_triggered()
{
    m_screenieControl->selectAll();
}

void MainWindow::on_addImageAction_triggered()
{
    Settings &settings = Settings::getInstance();
    QString lastImageDirectoryPath = settings.getLastImageDirectoryPath();
    QString filePath = QFileDialog::getOpenFileName(this, tr("Add Image"), lastImageDirectoryPath);
    if (!filePath.isNull()) {
        m_screenieControl->addImage(filePath, QPointF(0.0, 0.0));
        lastImageDirectoryPath = QFileInfo(filePath).absolutePath();
        settings.setLastImageDirectoryPath(lastImageDirectoryPath);
    }
}

void MainWindow::on_toggleFullScreenAction_triggered()
{
    if (!isFullScreen()) {
        /*!\todo Settings which control what becomes invisible in fullscreen mode */
        ui->toolBar->setVisible(false);
        ui->sidePanel->setVisible(false);
        ui->statusbar->setVisible(false);
        // Note: Qt crashes when we don't disable the unified toolbar before going
        // fullscreen (when we switch back to normal view, that is)!
        // But since for now we hide it anyway that does not make any visible difference.
        // The Qt documentation recommends anyway to do so.
        setUnifiedTitleAndToolBarOnMac(false);
        showFullScreen();
    } else {
        showNormal();
        ui->toolBar->setVisible(true);
        ui->sidePanel->setVisible(true);
        ui->statusbar->setVisible(true);
        setUnifiedTitleAndToolBarOnMac(true);
    }
}

void MainWindow::on_addTemplateAction_triggered()
{
    m_screenieControl->addTemplate(QPointF(0.0, 0.0));
}

void MainWindow::on_rotationSlider_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setRotation(value);
    m_ignoreUpdateSignals = false;
}

void MainWindow::on_distanceSlider_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setDistance(value);
    m_ignoreUpdateSignals = false;
}

void MainWindow::on_reflectionGroupBox_toggled(bool enable)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setReflectionEnabled(enable);
    m_ignoreUpdateSignals = false;
}

void MainWindow::on_reflectionOffsetSlider_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setReflectionOffset(value);
    m_ignoreUpdateSignals = false;
}

void MainWindow::on_reflectionOpacitySlider_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setReflectionOpacity(value);
    m_ignoreUpdateSignals = false;
}

void MainWindow::on_backgroundColorGroupBox_toggled(bool enable)
{
    m_ignoreUpdateSignals = true;
    this->m_screenieScene->setBackgroundEnabled(enable);
    m_ignoreUpdateSignals = false;
}

void MainWindow::on_redSlider_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setRedBackgroundComponent(value);
    m_ignoreUpdateSignals = false;
}

void MainWindow::on_greenSlider_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setGreenBackgroundComponent(value);
    m_ignoreUpdateSignals = false;
}

void MainWindow::on_blueSlider_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setBlueBackgroundComponent(value);
    m_ignoreUpdateSignals = false;
}

void MainWindow::on_aboutQtAction_triggered() {
    QMessageBox::aboutQt(this);
}

void MainWindow::handleGraphicsSceneChanged()
{
    if (m_screenieGraphicsScene->selectedItems().count() > 0) {
        QGraphicsItem *selected = m_screenieGraphicsScene->selectedItems().first();
        ui->statusbar->showMessage(QString("Object x/y: %1 / %2").arg(selected->scenePos().x()).arg(selected->scenePos().y()), 3000);
    }
}

void MainWindow::updateUi()
{
    if (!m_ignoreUpdateSignals) {
        updateTransformationUi();
        updateReflectionUi();
        updateColorUi();
        updateEditActions();
    }
    updateDefaultValues();
}

void MainWindow::updateDefaultValues()
{
    DefaultScreenieModel &defaultScreenieModel = m_screenieControl->getDefaultScreenieModel();
    defaultScreenieModel.setDistance(ui->distanceSlider->value());
    defaultScreenieModel.setRotation(ui->rotationSlider->value());
    defaultScreenieModel.setReflectionEnabled(ui->reflectionGroupBox->isChecked());
    defaultScreenieModel.setReflectionOffset(ui->reflectionOffsetSlider->value());
    defaultScreenieModel.setReflectionOpacity(ui->reflectionOpacitySlider->value());
}






