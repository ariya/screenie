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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QWidget>
#include <QtGui/QMainWindow>

class QWidget;
class QCloseEvent;

class ScreenieModelInterface;
class ScreenieScene;
class ScreeniePixmapItem;
class ScreenieControl;
class Clipboard;
class ScreenieGraphicsScene;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    bool read(const QString &filePath);

protected:
    void closeEvent(QCloseEvent *event);
    void showFullScreen();
    void showNormal();

private:
    Ui::MainWindow *ui;
    ScreenieGraphicsScene *m_screenieGraphicsScene;
    ScreenieScene *m_screenieScene;
    ScreenieControl *m_screenieControl;
    bool m_ignoreUpdateSignals;
    Clipboard *m_clipboard;
    QString m_documentFilePath;

    void frenchConnection();

    void newScene(ScreenieScene &screenieScene);
    bool write(const QString &filePath);

    void updateTransformationUi();
    void updateReflectionUi();
    void updateColorUi();
    void updateEditActions();
    void updateTitle();
    void initializeUi();

    void createScene();
    void updateScene(ScreenieScene &screenieScene);

    bool proceedWithModifiedScene();
    void restoreWindowGeometry();

private slots:
    // File
    void on_newAction_triggered();
    void on_openAction_triggered();
    void on_saveAction_triggered();
    void on_saveAsAction_triggered();
    void on_exportAction_triggered();

    // Edit
    void on_cutAction_triggered();
    void on_copyAction_triggered();
    void on_pasteAction_triggered();
    void on_deleteAction_triggered();
    void on_selectAllAction_triggered();

    // Insert
    void on_addImageAction_triggered();
    void on_addTemplateAction_triggered();

    // View
    void on_toggleFullScreenAction_triggered();

    void on_rotationSlider_valueChanged(int value);
    void on_distanceSlider_valueChanged(int value);
    void on_reflectionGroupBox_toggled(bool enable);
    void on_reflectionOffsetSlider_valueChanged(int value);
    void on_reflectionOpacitySlider_valueChanged(int value);

    void on_backgroundColorGroupBox_toggled(bool enable);
    void on_redSlider_valueChanged(int value);
    void on_greenSlider_valueChanged(int value);
    void on_blueSlider_valueChanged(int value);
    void on_redSpinBox_valueChanged(int value);
    void on_greenSpinBox_valueChanged(int value);
    void on_blueSpinBox_valueChanged(int value);
    void on_htmlBGColorLineEdit_editingFinished();

    void on_aboutQtAction_triggered();

    void updateUi();
    void updateDefaultValues();

};

#endif // MAINWINDOW_H
