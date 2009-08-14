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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "items/odacontactitem.h"

/*!
  Constructs main window object

  \param parent Parent object
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_ui->toolBox->setCurrentIndex(0); //Enabling first tab

    statusBar()->addWidget(&status);

    client = OdaConnection::getInstance();
    connect(client, SIGNAL(authenticated()), this, SLOT(onAuthenticated()));
    connect(client, SIGNAL(userContactList(OdaData)), this, SLOT(onContactList(OdaData)));
    connect(client, SIGNAL(userMessage(OdaData)), this, SLOT(onChatMessage(OdaData)));
    connect(client, SIGNAL(userStatus(OdaData)), this, SLOT(onStatusUpdate(OdaData)));
    connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

/*!
  Performs GUI translation on language change

  \param e Event
*/
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
  Shows setting window
*/
void MainWindow::on_actionSettings_activated()
{
    settings.display();
}

/*!
  Gets called when user is successfully authenticated and requests intial info
*/
void MainWindow::onAuthenticated()
{
    status.setText("Connected");
    client->requestContactList();
}

/*!
  Takes an action when client is disconnected
*/
void MainWindow::onDisconnected()
{
    status.setText("Disconnected");
}

/*!
  Opens chat window when new message arrives
*/
void MainWindow::onChatMessage(OdaData msg)
{
    int muid = msg.getInt("uid");

    for (int i=0; i<m_ui->contactList->count(); i++)
    {
        OdaContactItem* item = static_cast<OdaContactItem*>(m_ui->contactList->item(i));
        if (item->uid() == muid)
        {
            if (chat.addUser(item))
            {
                chat.onMessage(msg);
            }
            chat.show();
            return;
         }
    }
}

/*!
  Handles users status update
*/
void MainWindow::onStatusUpdate(OdaData pack)
{
    int muid = pack.getInt("uid");

    for (int i=0; i<m_ui->contactList->count(); i++)
    {
        OdaContactItem* item = static_cast<OdaContactItem*>(m_ui->contactList->item(i));
        if (item->uid() == muid)
        {
            item->updateStatus(pack.getInt("status"));
        }
    }
}

/*!
  Processes contacts list
*/
void MainWindow::onContactList(OdaData contacts)
{

    m_ui->contactList->clear();
    qint16 count = contacts.getInt("count");
    for (int i=0; i<count; i++)
    {
        OdaData contact = contacts.getObject(QString("contact").append(QString().setNum(i)));
        OdaContactItem* item = new OdaContactItem();
        item->setUid(contact.getInt("uid"));
        item->setText(contact.getString("fullName"));
        item->updateStatus(contact.getInt("status"));
        m_ui->contactList->addItem(item);
    }
}

/*!
  Opens a chat with selected user

  \param item Selected user
*/
void MainWindow::on_contactList_itemDoubleClicked(QListWidgetItem* item)
{
    OdaContactItem* contact = static_cast<OdaContactItem*>(item);
    chat.addUser(contact);
    chat.show();
}
