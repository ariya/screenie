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
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QSignalMapper>
#include <QtCore/QEvent>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QMenu>
#include <QtGui/QWidget>
#include <QtGui/QColor>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QFileDialog>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QShortcut>
#include <QtGui/QKeySequence>
#include <QtGui/QCloseEvent>
#include <QtGui/QAbstractButton>
#include <QtGui/QPushButton>
//#include <QtOpenGL/QGLWidget>
//#include <QtOpenGL/QGLFormat>

#include "../../Utils/src/Settings.h"
#include "../../Utils/src/Version.h"
#include "../../Utils/src/FileUtils.h"
#include "../../Model/src/ScreenieScene.h"
#include "../../Model/src/SceneLimits.h"
#include "../../Model/src/ScreenieModelInterface.h"
#include "../../Model/src/ScreenieTemplateModel.h"
#include "../../Model/src/Dao/ScreenieSceneDao.h"
#include "../../Model/src/Dao/Xml/XmlScreenieSceneDao.h"
#include "../../Kernel/src/ExportImage.h"
#include "../../Kernel/src/Clipboard/Clipboard.h"
#include "../../Kernel/src/ScreenieControl.h"
#include "../../Kernel/src/ScreenieGraphicsScene.h"
#include "../../Kernel/src/ScreeniePixmapItem.h"
#include "../../Kernel/src/PropertyDialogFactory.h"
#include "../../Kernel/src/DocumentManager.h"
#include "../../Kernel/src/DocumentInfo.h"
#include "../../Kernel/src/PropertyDialogFactory.h"
#include "PlatformManager/PlatformManagerFactory.h"
#include "PlatformManager/PlatformManager.h"
#include "RecentFiles.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

// public

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_ignoreUpdateSignals(false)
{
    ui->setupUi(this);

    m_screenieGraphicsScene = new ScreenieGraphicsScene(this);
    ui->graphicsView->setScene(m_screenieGraphicsScene);
    ui->graphicsView->setAcceptDrops(true);

    // Gesture support
    ui->graphicsView->viewport()->grabGesture(Qt::PinchGesture);

    // later: OpenGL support (configurable)
    // ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

    createScene();
    updateDocumentManager();
    initializeUi();
    m_platformManager = PlatformManagerFactory::getInstance().create();
    m_platformManager->initialize(*this, *ui);
    updateUi();
    restoreWindowGeometry();
    // call unified toolbar AFTER restoring window geometry!
    setUnifiedTitleAndToolBarOnMac(true);
    m_screenieControl->setRenderQuality(ScreenieControl::MaximumQuality);
    frenchConnection();
}

MainWindow::~MainWindow()
{
    delete m_screenieScene;
    delete m_screenieControl;
    delete m_platformManager;
    delete ui;
}

bool MainWindow::read(const QString &filePath)
{
    bool result;
    QFile file(filePath);
    ScreenieSceneDao *screenieSceneDao = new XmlScreenieSceneDao(file);
    ScreenieScene *screenieScene = screenieSceneDao->read();
    if (screenieScene != 0) {
        m_documentFilePath = filePath;
        newScene(*screenieScene);
        result = true;
    } else {
        result = false;
    }
    return result;
}

// public slots

void MainWindow::showFullScreen()
{
    /*!\todo Settings which control what becomes invisible in fullscreen mode */
    ui->toolBar->setVisible(false);
    ui->sidePanel->setVisible(false);
    // Note: Qt crashes when we don't disable the unified toolbar before going
    // fullscreen (when we switch back to normal view, that is)!
    // But since for now we hide it anyway that does not make any visible difference.
    // The Qt documentation recommends anyway to do so.
    // Also refer to: http://bugreports.qt.nokia.com/browse/QTBUG-16274
    setUnifiedTitleAndToolBarOnMac(false);
    QMainWindow::showFullScreen();
}

void MainWindow::showNormal()
{
    QMainWindow::showNormal();
    ui->toolBar->setVisible(true);
    ui->sidePanel->setVisible(true);
    setUnifiedTitleAndToolBarOnMac(true);
}

// protected

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_screenieScene->isModified()) {
        DocumentInfo::SaveStrategy saveStrategy = DocumentManager::getInstance().getSaveStrategy(*this);
        switch (saveStrategy) {
        case DocumentInfo::Save:
            event->ignore();
            saveBeforeClose();
        case DocumentInfo::Ask:
            event->ignore();
            askBeforeClose();
            break;
        case DocumentInfo::Discard:
            event->accept();
            break;
        default:
#ifdef DEBUG
            qCritical("MainWindow::closeEvent: UNHANDLED SaveStrategy: %d", saveStrategy);
#endif
            event->ignore();
        }
    } else {
        Settings::WindowGeometry windowGeometry;
        windowGeometry.fullScreen = isFullScreen();
        windowGeometry.position = pos();
        windowGeometry.size = size();
        Settings::getInstance().setWindowGeometry(windowGeometry);
        event->accept();
    }
}

// private

void MainWindow::frenchConnection()
{
    connect(m_screenieGraphicsScene, SIGNAL(selectionChanged()),
            this, SLOT(updateUi()));
    connect(m_screenieScene, SIGNAL(changed()),
            this, SLOT(updateUi()));
    connect(m_clipboard, SIGNAL(dataChanged()),
            this, SLOT(updateUi()));
    connect(&DocumentManager::getInstance(), SIGNAL(changed()),
            this, SLOT(updateWindowMenu()));

    // recent files
    connect(&m_recentFiles, SIGNAL(openRecentFile(const QString &)),
            this, SLOT(handleRecentFile(const QString &)));

    // Window menu
    connect(m_minimizeWindowsAction, SIGNAL(triggered()),
            this, SLOT(showMinimized()));
    connect(m_maximizeWindowsAction, SIGNAL(triggered()),
            this, SLOT(showMaximized()));
}

void MainWindow::newScene(ScreenieScene &screenieScene)
{
    updateScene(screenieScene);
    updateTitle();
}

bool MainWindow::writeScene(const QString &filePath)
{
    bool result;
    QFile file(filePath);
    ScreenieSceneDao *screenieSceneDao = new XmlScreenieSceneDao(file);
    result = screenieSceneDao->write(*m_screenieScene);
    if (result) {
        m_screenieScene->setModified(false);
        setWindowModified(false);
        m_documentFilePath = filePath;
        updateTitle();
    }
    return result;
}

bool MainWindow::writeTemplate(const QString &filePath)
{
    m_screenieControl->convertItemsToTemplate(*m_screenieScene);
    return writeScene(filePath);
}

void MainWindow::initializeUi()
{
    m_minimizeWindowsAction = new QAction(tr("Minimize", "Window menu"), this);
    m_minimizeWindowsAction->setShortcut(QKeySequence(Qt::Key_M + Qt::CTRL));
    m_maximizeWindowsAction = new QAction(tr("Maximize", "Window menu"), this);

    setWindowIcon(QIcon(":/img/application-icon.png"));
    updateTitle();
    updateWindowMenu();

    // recent files menu
    foreach (QAction *recentFileAction, m_recentFiles.getRecentFilesActionGroup().actions()) {
        ui->recentFilesMenu->addAction(recentFileAction);
    }

    ui->distanceSlider->setMinimum(SceneLimits::MinDistance);
    ui->distanceSlider->setMaximum(SceneLimits::MaxDistance);

    DefaultScreenieModel &defaultScreenieModel = m_screenieControl->getDefaultScreenieModel();
    ui->distanceSlider->setValue(defaultScreenieModel.getDistance());
    ui->rotationSlider->setValue(defaultScreenieModel.getRotation());
    ui->reflectionGroupBox->setChecked(defaultScreenieModel.isReflectionEnabled());
    ui->reflectionOffsetSlider->setValue(defaultScreenieModel.getReflectionOffset());
    ui->reflectionOpacitySlider->setValue(defaultScreenieModel.getReflectionOpacity());
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
    // sliders
    ui->redSlider->blockSignals(true);
    ui->redSlider->setValue(backgroundColor.red());
    ui->redSlider->blockSignals(false);
    ui->greenSlider->blockSignals(true);
    ui->greenSlider->setValue(backgroundColor.green());
    ui->greenSlider->blockSignals(false);
    ui->blueSlider->blockSignals(true);
    ui->blueSlider->setValue(backgroundColor.blue());
    ui->blueSlider->blockSignals(false);
    // spinboxes
    ui->redSpinBox->blockSignals(true);
    ui->redSpinBox->setValue(backgroundColor.red());
    ui->redSpinBox->blockSignals(false);
    ui->greenSpinBox->blockSignals(true);
    ui->greenSpinBox->setValue(backgroundColor.green());
    ui->greenSpinBox->blockSignals(false);
    ui->blueSpinBox->blockSignals(true);
    ui->blueSpinBox->setValue(backgroundColor.blue());
    ui->blueSpinBox->blockSignals(false);
    // html colour (without # prepended)
    ui->htmlBGColorLineEdit->setText(backgroundColor.name().right(6));
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

void MainWindow::updateTitle()
{
    QString title;
    DocumentManager &documentManager = DocumentManager::getInstance();
    if (!m_documentFilePath.isNull()) {
        title = QFileInfo(m_documentFilePath).fileName();
        documentManager.setWindowTitle(title, *this);
    } else {
        title = documentManager.getDocumentName(*this);
    }
    title.append("[*] - ");
    title.append(Version::getApplicationName());
    setWindowTitle(title);
}

void MainWindow::createScene()
{
    m_screenieScene = new ScreenieScene();
    m_screenieControl = new ScreenieControl(*m_screenieScene, *m_screenieGraphicsScene);
    m_clipboard = new Clipboard(*m_screenieControl, this);
}

void MainWindow::updateScene(ScreenieScene &screenieScene)
{
    // delete previous instances
    delete m_screenieScene;
    delete m_screenieControl;
    delete m_clipboard;
    m_screenieScene = &screenieScene;

    m_screenieControl = new ScreenieControl(*m_screenieScene, *m_screenieGraphicsScene);
    m_clipboard = new Clipboard(*m_screenieControl, this);
    m_screenieControl->updateScene();
    updateUi();
    updateDocumentManager();
    connect(m_screenieScene, SIGNAL(changed()),
            this, SLOT(updateUi()));
}

void MainWindow::handleMultipleModifiedBeforeQuit()
{
    QMessageBox *messageBox = new QMessageBox(QMessageBox::Warning,
                                              Version::getApplicationName(),
                                              tr("You have %1 documents with unsaved changes. Do you want to review these changes before quitting?")
                                              .arg(DocumentManager::getInstance().getModifiedCount()) +
                                              QString("<br /><br /><font size=\"-1\" style=\"font-weight:normal;\">") +
                                              tr("If you don't review your documents, all your changes will be lost.") + QString("</font>"),
                                              QMessageBox::NoButton,
                                              this);
    QAbstractButton *verifyButton = messageBox->addButton(tr("Verify changes..."), QMessageBox::AcceptRole);
    QAbstractButton *discardButton = messageBox->addButton(tr("Discard changes"), QMessageBox::DestructiveRole);
    messageBox->addButton(QMessageBox::Cancel);
    messageBox->setSizeGripEnabled(false);
    messageBox->exec();
    QAbstractButton *clickedButton = messageBox->clickedButton();
    if (verifyButton == clickedButton) {
        DocumentManager::getInstance().setSaveStrategyForAll(DocumentInfo::Ask);
        QApplication::closeAllWindows();
    } else if (discardButton == clickedButton) {
        DocumentManager::getInstance().setSaveStrategyForAll(DocumentInfo::Discard);
        QApplication::closeAllWindows();
    }
    delete messageBox;
}

void MainWindow::askBeforeClose()
{
    QMessageBox *messageBox = new QMessageBox(QMessageBox::Warning,
                                              Version::getApplicationName(),
                                              tr("Do you want to save the changes you made in the document \"%1\"?")
                                              .arg(DocumentManager::getInstance().getDocumentName(*this)) +
                                              QString("<br /><br /><font size=\"-1\" style=\"font-weight:normal;\">") +
                                              tr("Your changes will be lost if you don't save them.") + QString("</font>"),
                                              QMessageBox::Save,
                                              this);
    messageBox->addButton(QMessageBox::Discard);
    messageBox->addButton(QMessageBox::Cancel);
    messageBox->setAttribute(Qt::WA_DeleteOnClose);
    messageBox->open(this, SLOT(handleAskBeforeClose(int)));
}

void MainWindow::saveBeforeClose()
{
    if (!isFilePathRequired()) {
        bool ok = writeScene(m_documentFilePath);
        if (ok) {
            close();
        } else {
            showWriteError(DocumentManager::getInstance().getDocumentName(*this),
                           QDir::toNativeSeparators(m_documentFilePath));
        }
#ifdef DEBUG
        qDebug("MainWindow::saveBeforeClose: ok: %d", ok);
#endif
    } else {
        saveAsBeforeClose();
    }
}

void MainWindow::saveAsBeforeClose()
{
    QString lastDocumentDirectoryPath = Settings::getInstance().getLastDocumentDirectoryPath();
    QString sceneFilter = tr("Screenie Scene (*.%1)", "Save As dialog filter")
                          .arg(FileUtils::SceneExtension);

    QFileDialog *fileDialog = new QFileDialog(this, Qt::Sheet);
    fileDialog->setNameFilter(sceneFilter);
    fileDialog->setDefaultSuffix(FileUtils::SceneExtension);
    fileDialog->setWindowTitle(tr("Save As"));
    fileDialog->setDirectory(lastDocumentDirectoryPath);
    fileDialog->setWindowModality(Qt::WindowModal);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->open(this, SLOT(handleFileSaveAsBeforeCloseSelected(const QString &)));
}

void MainWindow::restoreWindowGeometry()
{
    Settings::WindowGeometry windowGeometry = Settings::getInstance().getWindowGeometry();
    if (windowGeometry.fullScreen) {
        showFullScreen();
    } else {
        resize(windowGeometry.size);
        if (!windowGeometry.position.isNull()) {
            move(windowGeometry.position);
        }
    }
}

void MainWindow::updateDocumentManager()
{
    bool created;
    DocumentManager &documentManager = DocumentManager::getInstance();
    DocumentInfo *documentInfo = documentManager.getDocumentInfo(*this);
    if (documentInfo == 0) {
        documentInfo = new DocumentInfo();
        created = true;
    } else {
        created = false;
    }
    documentInfo->mainWindow = this;
    documentInfo->screenieScene = m_screenieScene;
    documentInfo->saveStrategy = DocumentInfo::Ask;
    if (created) {
        documentManager.add(documentInfo);
    }
}

MainWindow *MainWindow::createMainWindow()
{
    MainWindow *result = new MainWindow();
    QPoint position = pos();
    /*!\todo Bug (Mac): With unified toolbar calling move calculates wrong window positions!
             Possible workaround: pass along desired position in c'tor */
    position += QPoint(28, 28);
    result->move(position);
    result->setAttribute(Qt::WA_DeleteOnClose, true);
    return result;
}

bool MainWindow::isFilePathRequired() const
{
    return m_documentFilePath.isNull() || (m_screenieScene->isTemplate() && !m_screenieScene->hasTemplatesExclusively());
}

void MainWindow::showReadError(const QString &filePath)
{
    showError(tr("Could not open document from file \"%1\"!")
              .arg(QDir::toNativeSeparators(filePath)));
}

void MainWindow::showWriteError(const QString &documentName, const QString &filePath)
{
    showError(tr("Could not save document \"%1\" to file \"%2\"!")
              .arg(documentName)
              .arg(filePath));
}

void MainWindow::showError(const QString &message)
{
    QMessageBox *messageBox = new QMessageBox(QMessageBox::Warning,
                                              Version::getApplicationName(),
                                              message,
                                              QMessageBox::Ok,
                                              this);
    messageBox->setAttribute(Qt::WA_DeleteOnClose);
    messageBox->open();
}

// private slots

void MainWindow::on_newAction_triggered()
{
    MainWindow *mainWindow = createMainWindow();
    mainWindow->show();
}

void MainWindow::on_openAction_triggered()
{
    Settings &settings = Settings::getInstance();
    QString lastDocumentDirectoryPath = settings.getLastDocumentDirectoryPath();
    QString allFilter = tr("Screenie Scenes (*.%1 *.%2)", "Open dialog filter")
                        .arg(FileUtils::SceneExtension)
                        .arg(FileUtils::TemplateExtension);
    QString sceneFilter = tr("Screenie Scene (*.%1)", "Open dialog filter")
                          .arg(FileUtils::SceneExtension);
    QString templateFilter = tr("Screenie Template (*.%1)", "Open dialog filter")
                             .arg(FileUtils::TemplateExtension);
    QString filter = allFilter + ";;" + sceneFilter + ";;" + templateFilter;

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open", "Open file dialog"), lastDocumentDirectoryPath, filter);

    if (!filePath.isNull()) {
        bool ok;
        if (m_screenieScene->isDefault()) {
            ok = read(filePath);
        } else {
            MainWindow *mainWindow = createMainWindow();
            ok = mainWindow->read(filePath);
            if (ok) {
                mainWindow->show();
            } else {
                delete mainWindow;
            }
        }
        if (ok) {
            QString lastDocumentFilePath = QFileInfo(filePath).absolutePath();
            Settings::getInstance().setLastDocumentDirectoryPath(lastDocumentFilePath);
        } else {
            showReadError(QDir::toNativeSeparators(filePath));
        }
    }
}

void MainWindow::on_saveAction_triggered()
{
    // save with given 'm_documentFilePath', if scene is not a template or if so, has only template items
    if (!isFilePathRequired()) {
        bool ok = writeScene(m_documentFilePath);
#ifdef DEBUG
        qDebug("MainWindow::on_saveAction_triggered: ok: %d", ok);
#endif
        if (!ok) {
            showWriteError(DocumentManager::getInstance().getDocumentName(*this),
                           QDir::toNativeSeparators(m_documentFilePath));

        }
    } else {
        on_saveAsAction_triggered();
    }
}

void MainWindow::on_saveAsAction_triggered()
{
    QString lastDocumentDirectoryPath = Settings::getInstance().getLastDocumentDirectoryPath();
    QString sceneFilter = tr("Screenie Scene (*.%1)", "Save As dialog filter")
                             .arg(FileUtils::SceneExtension);

    QFileDialog *fileDialog = new QFileDialog(this, Qt::Sheet);
    fileDialog->setNameFilter(sceneFilter);
    fileDialog->setDefaultSuffix(FileUtils::SceneExtension);
    fileDialog->setWindowTitle(tr("Save As"));
    fileDialog->setDirectory(lastDocumentDirectoryPath);
    fileDialog->setWindowModality(Qt::WindowModal);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->open(this, SLOT(handleFileSaveAsSelected(const QString &)));
}

void MainWindow::on_saveAsTemplateAction_triggered()
{
    QString lastDocumentDirectoryPath = Settings::getInstance().getLastDocumentDirectoryPath();
    QString templateFilter = tr("Screenie Template (*.%1)", "Save As Template dialog filter")
                             .arg(FileUtils::TemplateExtension);
    QFileDialog *fileDialog = new QFileDialog(this, Qt::Sheet);
    fileDialog->setNameFilter(templateFilter);
    fileDialog->setDefaultSuffix(FileUtils::TemplateExtension);
    fileDialog->setWindowTitle(tr("Save As Template"));
    fileDialog->setDirectory(lastDocumentDirectoryPath);
    fileDialog->setWindowModality(Qt::WindowModal);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->open(this, SLOT(handleFileSaveAsTemplateSelected(const QString &)));
}

void MainWindow::on_exportAction_triggered()
{
    Settings &settings = Settings::getInstance();
    QString lastExportDirectoryPath = settings.getLastExportDirectoryPath();
    QString filter = FileUtils::getSaveImageFileFilter();
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export Image"), lastExportDirectoryPath, filter);
    if (!filePath.isNull()) {
        ExportImage exportImage(*m_screenieScene, *m_screenieGraphicsScene);
        bool ok = exportImage.exportImage(filePath);
        if (ok) {
            lastExportDirectoryPath = QFileInfo(filePath).absolutePath();
            settings.setLastExportDirectoryPath(lastExportDirectoryPath);
        } else {
            showError(tr("Could not export iamge to file %1!")
                      .arg(filePath));
        }
    }
}

void MainWindow::on_closeAction_triggered()
{
    DocumentManager::setCloseRequest(DocumentManager::CloseCurrent);
    close();
}

void MainWindow::on_quitAction_triggered()
{
    DocumentManager &documentManager = DocumentManager::getInstance();
    DocumentManager::setCloseRequest(DocumentManager::Quit);
    int count = documentManager.count();
    if (count > 1) {
        int nofModified = documentManager.getModifiedCount();
        if (nofModified > 1) {
            handleMultipleModifiedBeforeQuit();
        } else {
            QApplication::closeAllWindows();
        }
    } else {
        QApplication::closeAllWindows();
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
    QString filter = FileUtils::getOpenImageFileFilter();
    QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Add Image"), lastImageDirectoryPath, filter);
    if (filePaths.count() > 0) {
        foreach(QString filePath, filePaths) {
            m_screenieControl->addImage(filePath, QPointF(0.0, 0.0));
        }
        lastImageDirectoryPath = QFileInfo(filePaths.last()).absolutePath();
        settings.setLastImageDirectoryPath(lastImageDirectoryPath);
    }
}

void MainWindow::on_addTemplateAction_triggered()
{
    m_screenieControl->addTemplate(QPointF(0.0, 0.0));
}

void MainWindow::on_toggleFullScreenAction_triggered()
{
    if (!isFullScreen()) {
        showFullScreen();
    } else {
        showNormal();
    }
}

void MainWindow::on_aboutAction_triggered()
{
    Version version;
    QString description = tr("A fancy screenshot composer");
    QString authors1 = tr("Developed by Till Oliver Knoll");
    QString authors2 = tr("Idea by Ariya Hidayat");
    QString versionString = tr("Version %1").arg(Version::getUserVersion());
    QString aboutText = QString("<b>") + Version::getApplicationName() + "</b><br />" +
                        description + "<br /><br />" +
                        "<font style=\"font-weight:normal;\">" + authors1 + "<br />" +
                        authors2 + "<br /><br />" +
                        Version::getApplicationName() + "<br />" +
                        versionString + "<br />" +
                        "\"" + Version::getCodeName() + "\"</font><br />" +
                        "<font style=\"font-weight:normal;color:#aaa;\">" + version.toString() + "</font>";
    QMessageBox::about(this, tr("About %1").arg(Version::getApplicationName()), aboutText);
}

void MainWindow::on_aboutQtAction_triggered()
{
    QMessageBox::aboutQt(this);
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
    updateColorUi();
}

void MainWindow::on_greenSlider_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setGreenBackgroundComponent(value);
    m_ignoreUpdateSignals = false;
    updateColorUi();
}

void MainWindow::on_blueSlider_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setBlueBackgroundComponent(value);
    m_ignoreUpdateSignals = false;
    updateColorUi();
}


void MainWindow::on_redSpinBox_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setRedBackgroundComponent(value);
    m_ignoreUpdateSignals = false;
    updateColorUi();
}

void MainWindow::on_greenSpinBox_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setGreenBackgroundComponent(value);
    m_ignoreUpdateSignals = false;
    updateColorUi();
}

void MainWindow::on_blueSpinBox_valueChanged(int value)
{
    m_ignoreUpdateSignals = true;
    m_screenieControl->setBlueBackgroundComponent(value);
    m_ignoreUpdateSignals = false;
    updateColorUi();
}

void MainWindow::on_htmlBGColorLineEdit_editingFinished()
{
    QString htmlNotation = QString("#") + ui->htmlBGColorLineEdit->text();
    QColor color(htmlNotation);
    qDebug("Foreground role: %d", ui->htmlBGColorLineEdit->foregroundRole());
    QPalette palette;
    if (color.isValid()) {
        m_screenieControl->setBackgroundColor(color);
        palette.setColor(ui->htmlBGColorLineEdit->foregroundRole(), Qt::black);

    } else {
        palette.setColor(ui->htmlBGColorLineEdit->foregroundRole(), Qt::red);
    }
    ui->htmlBGColorLineEdit->setPalette(palette);
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
    setWindowModified(m_screenieScene->isModified());
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

void MainWindow::handleRecentFile(const QString &filePath)
{
    // xxx
    bool ok;
    if (m_screenieScene->isDefault()) {
        ok = read(filePath);
    } else {
        MainWindow *mainWindow = createMainWindow();
        ok = mainWindow->read(filePath);
        if (ok) {
            mainWindow->show();
        } else {
            delete mainWindow;
        }
    }
    if (!ok) {
        showReadError(filePath);
    }
}

void MainWindow::updateWindowMenu()
{
    QMenu *windowMenu = ui->windowMenu;
    windowMenu->clear();
    windowMenu->addAction(m_minimizeWindowsAction);
    windowMenu->addAction(m_maximizeWindowsAction);
    windowMenu->addSeparator();
    QActionGroup &actionGroup = DocumentManager::getInstance().getActionGroup();
    foreach (QAction *action, actionGroup.actions()) {
        windowMenu->addAction(action);
    }
}

void MainWindow::handleFileSaveAsSelected(const QString &filePath)
{
    bool ok = false;
    if (!filePath.isNull()) {
        m_screenieScene->setTemplate(false);
        ok = writeScene(filePath);
        if (ok) {
            QString lastDocumentDirectoryPath = QFileInfo(filePath).absolutePath();
            Settings &settings = Settings::getInstance();
            settings.setLastDocumentDirectoryPath(lastDocumentDirectoryPath);
            settings.addRecentFile(filePath);
        } else {
            showWriteError(DocumentManager::getInstance().getDocumentName(*this),
                           QDir::toNativeSeparators(filePath));
        }
    }
}

void MainWindow::handleFileSaveAsTemplateSelected(const QString &filePath)
{
    bool ok = false;
    if (!filePath.isNull()) {
        m_screenieScene->setTemplate(true);
        ok = writeTemplate(filePath);
        if (ok) {
            QString lastDocumentDirectoryPath = QFileInfo(filePath).absolutePath();
            Settings &settings = Settings::getInstance();
            settings.setLastDocumentDirectoryPath(lastDocumentDirectoryPath);
            settings.addRecentFile(filePath);
        } else {
            showWriteError(DocumentManager::getInstance().getDocumentName(*this),
                           filePath);
        }
    }
}

void MainWindow::handleFileSaveAsBeforeCloseSelected(const QString &filePath)
{
    bool ok = false;
    if (!filePath.isNull()) {
        m_screenieScene->setTemplate(false);
        ok = writeScene(filePath);
        if (ok) {
            QString lastDocumentDirectoryPath = QFileInfo(filePath).absolutePath();
            Settings &settings = Settings::getInstance();
            settings.setLastDocumentDirectoryPath(lastDocumentDirectoryPath);
            settings.addRecentFile(filePath);
            if (DocumentManager::getCloseRequest() == DocumentManager::CloseCurrent) {
                close();
            } else {
                QApplication::closeAllWindows();
            }
        } else {
            showWriteError(DocumentManager::getInstance().getDocumentName(*this),
                           QDir::toNativeSeparators(filePath));
        }
    }
}

void MainWindow::handleAskBeforeClose(int answer)
{
    switch (answer) {
    case QMessageBox::Save:
        saveBeforeClose();
        break;
    case QMessageBox::Discard:
        DocumentManager::getInstance().setSaveStrategy(*this, DocumentInfo::Discard);
        if (DocumentManager::getCloseRequest() == DocumentManager::CloseCurrent) {
            close();
        } else {
            QApplication::closeAllWindows();
        }
        break;
    case QMessageBox::Cancel:
        break;
    default:
#ifdef DEBUG
        qCritical("MainWindow::handleAskBeforeClose: UNHANDLED QMessageBox answer: %d", answer);
#endif
        break;
    }
}

