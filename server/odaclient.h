/*!
    Open Development Assistant Server. Client representation class
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

#ifndef ODATCLIENT_H
#define ODACLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtStateMachine>
#include <QSqlDatabase>
#include "../lib/odaprotocol/odadata.h"

/*!
  Client representation class
  Implements data exchange protocol from server side
*/
class OdaClient : public QObject
{
    Q_OBJECT

private:
    QString uCid;               ///< Unique client id (object id in server hash)
    unsigned int uid;           ///< User unique id
    QTcpSocket* socket;         ///< Client socket
    QtStateMachine stateMachine;///< Client state machine
    QString authToken;          ///< Authentication token (generated for each connection)
    QSqlDatabase *db;           ///< Database connection (provided by server object)

    void sendError(int errorCode);
    void sendPackage(qint16 operation, OdaData package);
    void sendPackage(qint16 operation, bool dataComesLater = false);
    qint16 getOperation();
    OdaData getPackage();

public:
    OdaClient(QString clientId, QSqlDatabase* database, QTcpSocket*);
    QString clientId();

private slots:
    void onDisconnect();        ///< Slot that takes an action on client disconnect

    // Methods processing the states
    void onPreAuth();           ///< Slot that process "Pre-auth" state
    void onAuthenticate();      ///< Slot that process "Authentication" state
    void onCommand();           ///< Slot that process "Command route" state
    void onGetUserInfo();       ///< Slot that process "GetUserInfo" command

signals:
    void authenticated();       ///< Signal that indicates authentication success
    void commandUnknown();      ///< Signal that indicates unknow command
    void commandDone();         ///< Signal that indicates command processing end
    void clientDisconnected();  ///< Signal that indicates client socket disconnect

    void getUserInfo();         ///< Signal that indicates getUserInfo operation

};

#endif // ODACLIENT_H
