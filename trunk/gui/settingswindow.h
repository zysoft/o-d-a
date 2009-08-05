/*!
    Open Development Assistant Gui. Settings window
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

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QtGui/QDialog>

/*!
  Namespace for SettingsWindow
*/
namespace Ui {
    class SettingsWindow;
}


/*!
  Settings window class
  Handles user settings like server host/port, etc.
*/
class SettingsWindow : public QDialog {
    Q_OBJECT
public:
    SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsWindow *m_ui;

private slots:
    void on_buttonBox_accepted();

public slots:
    void display();

};

#endif // SETTINGSWINDOW_H
