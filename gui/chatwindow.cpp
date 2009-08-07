/*!
    Open Development Assistant. Chat window
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

#include <QVector>
#include <QMessageBox>
#include "chatwindow.h"
#include "ui_chatwindow.h"

/*!
  Constructs chat window object

  \param parent Parent object
*/
ChatWindow::ChatWindow(QWidget *parent) :
        QMainWindow(parent),
        m_ui(new Ui::ChatWindow)
{
    m_ui->setupUi(this);

    client = OdaConnection::getInstance();
    connect(client, SIGNAL(userMessage(OdaData)), this, SLOT(onMessage(OdaData)));
    connect(client, SIGNAL(error(QString)), this, SLOT(onError(QString)));
}

ChatWindow::~ChatWindow()
{
    delete m_ui;
}

/*!
  Performs GUI translation on language change

  \param e Event
*/
void ChatWindow::changeEvent(QEvent *e)
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
  Resets chat window and prepares it to setup
*/
void ChatWindow::reset()
{
    setWindowTitle(tr("Chat with:"));
    users.empty();
}

/*!
  Adds user to chat

  \param user User contact item
*/
void ChatWindow::addUser(OdaContactItem* user)
{
    users.insert(user->uid(), user);
    setWindowTitle(windowTitle()+user->text()+", ");
}

/*!
  Slot displaying errors

  \param err Error string
*/
void ChatWindow::onError(QString err)
{
    QMessageBox::critical(this, tr("Error"), err);
}

/*!
  Takes an action when new message arrives

  \param msg Message data
*/
void ChatWindow::onMessage(OdaData msg)
{

    int uid = msg.getInt("uid");
    if (users.find(uid) == users.end())
    {
        return;
    }

    m_ui->messages->append("<b>"+users[uid]->text()+":</b><br/>"+msg.getString("message")+"<hr/>");
}

/*!
  Sends user message
*/
void ChatWindow::on_send_clicked()
{
    if (!m_ui->myMessage->toPlainText().trimmed().length())
    {
        return;
    }

    m_ui->messages->append("<b>Me:</b><br/>"+m_ui->myMessage->toPlainText()+"<hr/>");

    QVector<int> uids = QVector<int>::fromList(users.keys());
    client->sendChatMessage(uids, m_ui->myMessage->toPlainText());
    m_ui->myMessage->clear();
}
