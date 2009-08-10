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

#include <QMessageBox>
#include <QSettings>
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "protocol/odaconnection.h"

/*!
  Constructs login window object

  \param parent Parent object
*/
LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    client = OdaConnection::getInstance();
    connect(client, SIGNAL(authenticated()), this, SLOT(onAuthSuccess()));

    QSettings config;
    ui->login->setText(config.value("user/login").toString());
    if (ui->login->text().trimmed().length())
    {
        ui->password->setFocus();
    }
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


/*!
  Initiates server session
*/
void LoginWindow::on_doLogin_clicked()
{
    if (!(ui->login->text().trimmed().length() && ui->password->text().trimmed().length()))
    {
        QMessageBox::warning(this, tr("Error"), tr("You have to enter username and password to login"));
        return;
    }

    QSettings config;
    client->initiate(config.value("server/host").toString(), config.value("server/port").toInt(), ui->login->text(), ui->password->text());
}


/*!
  Takes an action when session is successfully intiated (user authenticated)
*/
void LoginWindow::onAuthSuccess()
{
    accept();
}

/*!
  Shows setup dialog
*/
void LoginWindow::on_setup_clicked()
{
    setup.display();
}
