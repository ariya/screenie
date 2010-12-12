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
#include <QtGui/QWidget>
#include <QtGui/QMainWindow>

class QWidget;
class QString;

class ScreenieModelInterface;
class ScreenieScene;
class ScreeniePixmapItem;
class ScreenieControl;
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

private:
    Ui::MainWindow *ui;
    ScreenieGraphicsScene *m_screenieGraphicsScene;
    ScreenieScene *m_screenieScene;
    ScreenieControl *m_screenieControl;
    bool m_ignoreUpdateSignals;

    void frenchConnection();

    void updateTransformationUi();
    void updateReflectionUi();
    void updateColorUi();
    void initializeUi();

private slots:
    void on_addImageAction_triggered();
    void on_exportAction_triggered();
    void on_exportPDFAction_triggered();

    void on_rotationSlider_valueChanged(int value);
    void on_distanceSlider_valueChanged(int value);
    void on_reflectionGroupBox_toggled(bool enable);
    void on_reflectionOffsetSlider_valueChanged(int value);
    void on_reflectionOpacitySlider_valueChanged(int value);
    void on_backgroundColorGroupBox_toggled(bool enable);
    void on_redSlider_valueChanged(int value);
    void on_greenSlider_valueChanged(int value);
    void on_blueSlider_valueChanged(int value);

    void on_aboutQtAction_triggered();

    void handleGraphicsSceneChanged();
    void updateUi();
    void updateDefaultValues();
};

#endif // MAINWINDOW_H
