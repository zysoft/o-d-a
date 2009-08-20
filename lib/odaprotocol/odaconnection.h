/*!
    Open Development Assistant Server. Protocol network base connector
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

#ifndef ODACONNECTION_H
#define ODACONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QtStateMachine>
#include <QHash>
#include "odadata.h"

/*!
  Abstract operation prototype
*/
class OdaAbstractOperation;

/*!
  Network protocol connection class
  Used for both client and server side communication
*/
class OdaConnection : public QObject
{
    Q_OBJECT

protected:
    QString uCid;                                   ///< Client unique id
    unsigned int uid;                               ///< Authenticated user unique id
    unsigned int cid;                               ///< User company unique id
    QTcpSocket* socket;                             ///< TCP Socket
    QtStateMachine stateMachine;                    ///< State machine
    QSqlDatabase* db;                               ///< Database object
    QHash<int, OdaAbstractOperation*> operations;   ///< List of the installed operations

    void addOperation(int operationCode, OdaAbstractOperation*, QtState* routeState, QtState* waitState, bool isServerSide);

    qint16 getOperation();
    OdaData getPackage();

public:
    OdaConnection(QString clientId, QTcpSocket*, bool isServerSide = false, QSqlDatabase* database = NULL);
    QString clientId();
    unsigned int userId();
    unsigned int companyId();
    QTcpSocket* getSocket();
    OdaAbstractOperation* getOperation(int operationCode);

private slots:
    void socketCheck();                          ///< Slot that checks if socket has any data
    void onDisconnect();                         ///< Slot that takes an action on client disconnect
    void onRoute(qint16, unsigned int, OdaData); ///< Slot that takse an action on server request routing

public slots:
    void sendPackage(qint16 operation, OdaData package);
    void sendPackage(qint16 operation, bool dataComesLater = false);
    void sendError(int errCode, bool isFatal = false);
    void onCommand();

    void proxyRoute(qint16, unsigned int, OdaData);
    void proxyAuth(unsigned int, unsigned int);

signals:
    void init();                                    ///< Initializes state machine
    void errorCode(int);                            ///< Transports error code
    void operation(qint16, OdaData);                ///< Operation signal
    void route(qint16, unsigned int, OdaData);      ///< Routing signal
    void socketReady();                             ///< Indicates that socket still has some data
    void authenticated(unsigned int, unsigned int); ///< Signal that indicates authentication success
    void clientDisconnected();                      ///< Signal that indicates client socket disconnect
    void commandUnknown();                          ///< Signal that indicates unknown command

};

#endif // ODACONNECTION_H
