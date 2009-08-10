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
#include <QLabel>
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
    m_ui->tabs->clear();
    delete m_ui->tab;

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
  Adds user to chat

  \param user User contact item
  \return     Result state (true - user added, false - already exists)
*/
bool ChatWindow::addUser(OdaContactItem* user)
{
    if (tabs.find(user->uid()) != tabs.end())
    {
        m_ui->tabs->setCurrentIndex(m_ui->tabs->indexOf(tabs[user->uid()]));
        return false;
    }

    QWidget* tab = new QWidget();
    QHBoxLayout* lay = new QHBoxLayout(tab);
    QTextEdit* t = new QTextEdit(tab);
    t->setReadOnly(true);
    lay->addWidget(t);
    tab->setLayout(lay);
    m_ui->tabs->addTab(tab, user->text());
    tabs.insert(user->uid(), tab);
    users.insert(user->uid(), user);
    m_ui->tabs->setCurrentIndex(m_ui->tabs->indexOf(tab));
    return true;
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

    tabs[uid]->findChild<QTextEdit*>()->append("<b>"+users[uid]->text()+":</b><br/>"+msg.getString("message")+"<hr/>");
    m_ui->tabs->setCurrentIndex(m_ui->tabs->indexOf(tabs[uid]));
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

    m_ui->tabs->currentWidget()->findChild<QTextEdit*>()->append("<b>Me:</b><br/>"+m_ui->myMessage->toPlainText()+"<hr/>");

    QVector<int> uids;
    uids << tabs.key(m_ui->tabs->currentWidget());
    client->sendChatMessage(uids, m_ui->myMessage->toPlainText());
    m_ui->myMessage->clear();
}

/*!
  Closes tab
  Removes widget object and requested tab
*/
void ChatWindow::on_tabs_tabCloseRequested(int index)
{
    m_ui->tabs->setCurrentIndex(index);
    int uid = tabs.key(m_ui->tabs->currentWidget());
    m_ui->tabs->removeTab(index);
    delete tabs[uid];
    tabs.remove(uid);
}
