/*!
    Open Development Assistant Gui. Login window
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

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QtGui/QDialog>
#include "settingswindow.h"
#include "protocol/odaconnection.h"

/*!
  Namespace for LoginWindow
*/
namespace Ui
{
    class LoginWindow;
}

/*!
  Login window class
*/
class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private:
    Ui::LoginWindow *ui;
    OdaConnection* client;
    SettingsWindow setup;

private slots:
    void on_setup_clicked();
    void on_doLogin_clicked();
    void onAuthSuccess();
};

#endif // LOGINWINDOW_H
