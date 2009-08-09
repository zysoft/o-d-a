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

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QtGui/QMainWindow>
#include <QHash>
#include "protocol/odaconnection.h"
#include "items/odacontactitem.h"
#include "../lib/odaprotocol/odadata.h"

/*!
  Namespace for LoginWindow
*/
namespace Ui {
    class ChatWindow;
}

/*!
  Chat window class
*/
class ChatWindow : public QMainWindow {
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = 0);
    ~ChatWindow();
    bool addUser(OdaContactItem* user);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ChatWindow *m_ui;
    QHash<int, OdaContactItem*> users;
    QHash<int, QWidget*> tabs;
    OdaConnection* client;

private slots:
    void on_send_clicked();
    void onError(QString);

public slots:
    void onMessage(OdaData);
};

#endif // CHATWINDOW_H
