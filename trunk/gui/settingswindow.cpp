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

#include <QSettings>
#include "settingswindow.h"
#include "ui_settingswindow.h"

/*!
  Constructs settings window object

  \param parent Parent object
*/
SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsWindow)
{
    m_ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
    delete m_ui;
}

/*!
  Performs GUI translation on language change

  \param e Event
*/
void SettingsWindow::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
  Reads the settings and displays the window
  SHOULD BE CALLED INSTEAD OF show()
*/
void SettingsWindow::display()
{
    QSettings config;
    m_ui->serverHost->setText(config.value("server/host").toString());
    m_ui->serverPort->setValue(config.value("server/port", 55000).toInt());
    m_ui->userName->setText(config.value("user/login").toString());
    show();
}

/*!
  Takes an action when user saves the settings
  Perform settings save
*/
void SettingsWindow::on_buttonBox_accepted()
{
    QSettings config;
    config.setValue("server/host", m_ui->serverHost->text());
    config.setValue("server/port", m_ui->serverPort->value());
    config.setValue("user/login", m_ui->userName->text());
}
