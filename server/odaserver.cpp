/*!
    Open Development Assistant Server
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

#include <iostream>
#include <QDateTime>
#include <QtCore/QCoreApplication>
#include "odaserver.h"

/*!
  Starts the server

  \param port   Port to listen at
  \param dbHost Service database host name
  \param dbUser Service database user name
  \param dbPass Service database user password
  \param dbName Service database name
*/
void OdaServer::start(int port, QString dbHost, QString dbUser, QString dbPass, QString dbName)
{
    srand((unsigned)time(0));
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(dbHost);
    db.setDatabaseName(dbName);
    db.setUserName(dbUser);
    db.setPassword(dbPass);
    if (!db.open()) {
        throw QString("Error connecting to database");
    }

    connect(&socket, SIGNAL(newConnection()), this, SLOT(clientConnected()));
    socket.listen(QHostAddress::Any, port);
}

/*!
  Stops the server
  Closes listening socket and database connection
*/
void OdaServer::stop()
{
    socket.close();
    db.close();
    db = QSqlDatabase();
}

/*!
  Slot serving newly connected clients
  Initializes client object, creates its unique identifier and puts it into the hash
*/
void OdaServer::clientConnected()
{
    QTcpSocket *clientConnection = socket.nextPendingConnection();
    QString clientId;
    clientId = clientConnection->peerAddress().toString() + QDateTime(QDateTime::currentDateTime()).toString("yyyyMMddhhmmsszzz");
    OdaClient* client = new OdaClient(clientId, &db , clientConnection);
    connect(client, SIGNAL(clientDisconnected()), this, SLOT(clientDisconnected()));
    clients[client->clientId()] = client;
}

/*!
  Slot serving disconnected clients
  Removes disconnected client from hash
*/
void OdaServer::clientDisconnected()
{
    OdaClient* client = static_cast<OdaClient*>(sender());
    clients.remove(client->clientId());
}