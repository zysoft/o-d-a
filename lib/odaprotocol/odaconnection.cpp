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

#include "odaconnection.h"
#include "operations/odaoperations.h"

/*!
  Instantinates connector, its state machine and operation objects

  \param clientId       Unique connection id (freeform)
  \param sock           Socket object
  \param isServerSide   Idicates work mode - client or server
  \param database       Database connection
*/
OdaConnection::OdaConnection(QString clientId, QTcpSocket* sock, bool isServerSide, QSqlDatabase* database)
{
    socket = sock;
    uCid = clientId;
    db = database;

    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));

    //Creating states
    QtState *preInit = new QtState();
    QtState *init = new QtState();
    QtState *preAuth = new QtState();
    QtState *authentication = new QtState();
    QtState *commandWait = new QtState();
    QtState *commandRoute = new QtState();

    //Creating state transitions
    preInit->addTransition(socket, SIGNAL(connected()), init);
    init->addTransition(socket, SIGNAL(readyRead()), preAuth);
    preAuth->addTransition(socket, SIGNAL(readyRead()), authentication);
    authentication->addTransition(this, SIGNAL(authenticated(uint,uint)), commandWait);
    commandWait->addTransition(socket, SIGNAL(readyRead()), commandRoute);
    commandWait->addTransition(this, SIGNAL(socketReady()), commandRoute);
    commandRoute->addTransition(this, SIGNAL(commandUnknown()), commandWait);

    //Assingning event handlers
    commandWait->invokeMethodOnEntry(this, "socketCheck");
    commandRoute->invokeMethodOnEntry(this, "onCommand");

    //Filling up state machine
    stateMachine.addState(preInit);
    stateMachine.addState(init);
    stateMachine.addState(preAuth);
    stateMachine.addState(authentication);
    stateMachine.addState(commandWait);
    stateMachine.addState(commandRoute);


    //Initializing operations that extend state machine
    OdaAuthStep1* authstep1 = new OdaAuthStep1();
    authstep1->install(this, init, preAuth, isServerSide);
    operations.append(authstep1);

    OdaAuthStep2* authstep2 = new OdaAuthStep2();
    authstep2->install(this, preAuth, authentication, db, isServerSide);
    authstep2->setAuthToken(authstep1->getToken());
    operations.append(authstep2);

    OdaGetContactList* getContactList = new OdaGetContactList();
    getContactList->install(this, &stateMachine, commandRoute, commandWait, db, isServerSide);
    operations.append(getContactList);

    OdaStatusNotification* statusNotification = new OdaStatusNotification();
    statusNotification->install(this, &stateMachine, commandRoute, commandWait, db, isServerSide);
    operations.append(statusNotification);

    OdaSendMessage* sendMessage = new OdaSendMessage();
    sendMessage->install(this, &stateMachine, commandRoute, commandWait, db, isServerSide);
    operations.append(sendMessage);

    OdaNewMessageNotification* newMessageNotification = new OdaNewMessageNotification();
    newMessageNotification->install(this, &stateMachine, commandRoute, commandWait, db, isServerSide);
    operations.append(newMessageNotification);

    //Running the state machine
    stateMachine.setInitialState((isServerSide) ? init : preInit);
    stateMachine.initialState();
    stateMachine.start();
}

/*!
  Return connection identifier
*/
QString OdaConnection::clientId()
{
    return uCid;
}

/*!
  Return user unique identifier
*/
unsigned int OdaConnection::userId()
{
    return uid;
}

/*!
  Return user company identifier
*/
unsigned int OdaConnection::companyId()
{
    return cid;
}

/*!
  Return socket object
*/
QTcpSocket* OdaConnection::getSocket()
{
    return socket;
}

/*!
  Checks if the socket still has any data
  This is needed because QTcpSocket::readyRead signal is emited once on data arrival
*/
void OdaConnection::socketCheck()
{
    if (socket->bytesAvailable())
    {
        emit socketReady();
    }
}

/*!
  Transport slot assigned to QTcpSocket::disconnected
  Emits signal for external object to catch
*/
void OdaConnection::onDisconnect()
{
    emit clientDisconnected();
}

/*!
  Reads operation code

  \return Operation code
*/
qint16 OdaConnection::getOperation()
{
    qint16 operation;
    socket->read((char*)&operation, sizeof(qint16));
    return operation;
}

/*!
  Reads data package without operation preamble (preamble is read by getOperation method)

  \see OdaConnection::getOperation()

  \return Data package
*/
OdaData OdaConnection::getPackage()
{
    qint16 size;
    socket->read((char*)&size, sizeof(qint16));
    if (size == 0)
    {
        return OdaData();
    }

    char* datac = (char*)malloc(size);
    socket->read(datac, size);
    QByteArray data(datac);
    free(datac);
    return OdaData::unserialize(&data);
}

/*!
  Sends data package

  \param operation    Operation code
  \param package      Data package
*/
void OdaConnection::sendPackage(qint16 operation, OdaData package)
{
    sendPackage(operation, true);

    QByteArray data = package.serialize();
    qint16 size = data.size()+1; //+1 is to always send \0
    socket->write((char*)&size, sizeof(qint16));
    socket->write(data.constData(), size);
}

/*!
  Sends data package

  \param operation      Operation code
  \param dataComesLater Idicates if the package is empty or data package comes later
*/
void OdaConnection::sendPackage(qint16 operation, bool dataComesLater)
{
    socket->write((char*)&operation, sizeof(qint16));
    if (!dataComesLater)
    {
        qint16 size = 0;
        socket->write((char*)&size, sizeof(qint16));
    }
}

/*!
  Sends an error package to client

  \param error      Error explanation string
  \param isFatal    Indicates if the error is fatal. Socket gets disconnected on fatal errors
*/
void OdaConnection::sendError(int errorCode, bool isFatal)
{
    OdaData err;
    err.set("errorCode", errorCode);
    sendPackage(0xFFFF, err);
    if (isFatal)
    {
        socket->close();
    }
}

/*!
  Slot. Gets executed on entering "Command Route" state
  Checks operation code, generates and sends authentication token
*/
void OdaConnection::onCommand()
{
    qint16 op = getOperation();
    OdaData package = getPackage();
    emit operation(op, package);
    emit commandUnknown();
}

/*!
  Special router.
  Takes an action when server performs signal routing between currently active clients for data exchange

  \param operation      Operation getting routed
  \param targetUserId   UID rout is addressed to (FFFF means to ALL)
  \param dataPack       Data package enclosed to operation
*/
void OdaConnection::onRoute(qint16 op, unsigned int targetUid, OdaData package)
{
    if (uid != targetUid && targetUid != 0xFFFF)
    {
        return;
    }

    emit operation(op, package);
    emit commandUnknown();
}

/*!
  Special proxy that provides routing featrue for operation objects

  \param operation      Operation getting routed
  \param targetUserId   UID rout is addressed to (FFFF means to ALL)
  \param dataPack       Data package enclosed to operation
*/
void OdaConnection::proxyRoute(qint16 op, unsigned int targetUid, OdaData package)
{
    emit route(op, targetUid, package);
}


/*!
  Special proxy that proxies authentication signal from authentication step2 operation to outside

  \param uid   Authenticated user unique identifier
  \param cid   Authenticated user company identifier
*/
void OdaConnection::proxyAuth(unsigned int uid, unsigned int cid)
{
    this->uid = uid;
    this->cid = cid;
    emit authenticated(uid, cid);
}
