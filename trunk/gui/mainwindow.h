/*!
    Open Development Assistant Gui. Main window
    Copyright (C) 2009  Yuriy Zisin <zysoftik@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "settingswindow.h"

/*!
  Namespace for MainWindow
*/
namespace Ui {
    class MainWindow;
}

/*!
  Main window class
  This is the main application window
*/
class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    SettingsWindow settings; ///< Settings window instance

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *m_ui;

private slots:
    void on_actionSettings_activated();
};

#endif // MAINWINDOW_H
