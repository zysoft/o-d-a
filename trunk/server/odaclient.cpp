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

#include <cstdlib>
#include <QtState>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include "../lib/odaprotocol/odadefinitions.h"
#include "odaclient.h"


/*!
  Constructor
  Fills in client settings

  \param clientId Current client unique identifier in server hash
  \param database Database connection
  \param sock Client socket
*/
OdaClient::OdaClient(QString clientId, QSqlDatabase* database, QTcpSocket* sock)
{
    socket = sock;
    uCid = clientId;
    db = database;
    
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    
    //Creating states
    QtState *init = new QtState();
    QtState *preAuth = new QtState();
    QtState *authentication = new QtState();
    QtState *commandWait = new QtState();
    QtState *commandRoute = new QtState();
    QtState *doingUserInfo = new QtState();
    
    //Creating state transitions
    init->addTransition(socket, SIGNAL(readyRead()), preAuth);
    preAuth->addTransition(socket, SIGNAL(readyRead()), authentication);
    authentication->addTransition(this, SIGNAL(authenticated()), commandWait);
    commandWait->addTransition(socket, SIGNAL(readyRead()), commandRoute);
    commandRoute->addTransition(this, SIGNAL(commandUnknown()), commandWait);
    commandRoute->addTransition(this, SIGNAL(getUserInfo()), doingUserInfo);
    doingUserInfo->addTransition(this, SIGNAL(commandDone()), commandWait);
    
    //Assingning event handlers
    preAuth->invokeMethodOnEntry(this, "onPreAuth");
    authentication->invokeMethodOnEntry(this, "onAuthenticate");
    commandRoute->invokeMethodOnEntry(this, "onCommand");
    doingUserInfo->invokeMethodOnEntry(this, "onGetUserInfo");
    
    //Filling up state machine
    stateMachine.addState(init);
    stateMachine.addState(preAuth);
    stateMachine.addState(authentication);
    stateMachine.addState(commandWait);
    stateMachine.addState(commandRoute);
    stateMachine.addState(doingUserInfo);
    

    //Running the state machine
    stateMachine.setInitialState(init);
    stateMachine.initialState();
    stateMachine.start();
}

/*!
  Returns client unique identifier

  \return Client unique identifier
*/
QString OdaClient::clientId() {
    return uCid;
}


/*!
  Sends data package

  \param operation Operation code
  \param package      Data package
*/
void OdaClient::sendPackage(qint16 operation, OdaData* package)
{
    socket->write((char*)&operation, sizeof(qint16));
    qint16 size = 0;
    if (package != NULL)
    {
        QByteArray data = package->serialize();
        size = data.size();
        socket->write((char*)&size, sizeof(qint16));
        socket->write(data.constData(), size);
    }
    else
    {
        socket->write((char*)&size, sizeof(qint16));
    }
}

/*!
  Reads operation code

  \return Operation code
*/
qint16 OdaClient::getOperation()
{
    qint16 operation;
    socket->read((char*)&operation, sizeof(qint16));
    return operation;
}


/*!
  Reads data package without preamble

  \return Data package
*/
OdaData* OdaClient::getPackage()
{
    qint16 size;
    socket->read((char*)&size, sizeof(qint16));
    if (size == 0)
    {
        return new OdaData();
    }

    char* datac = (char*)malloc(size);
    socket->read(datac, size);
    QByteArray data(datac);
    free(datac);
    return OdaData::unserialize(&data);
}

/*!
  Sends an error package to client
  
  \param error Error explanation string
*/
void OdaClient::sendError(int errorCode)
{
    OdaData* err = new OdaData();
    err->set("errorCode", errorCode);
    sendPackage(0xFFFF, err);
}

/*!
  Transport slot assigned to QTcpSocket::disconnected
  Emits signal for server object to catch
*/
void OdaClient::onDisconnect() {
    emit clientDisconnected();
}

/*!
  Slot. Gets executed on entering "Pre-Auth" state
  Checks operation code, generates and sends authentication token
*/
void OdaClient::onPreAuth()
{
    if (getOperation() != OP_AUTH_START)
    {
        sendError(ERR_PREAMBLE_INVALID);
        socket->close();
        return;
    }

    //Reading empty package
    getPackage();

    //Authentication token generation
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(QString().setNum(rand()+rand()+rand()).toAscii());
    authToken = hash.result().toHex();

    //Sending authentication token package with preamble
    OdaData* preAuth = new OdaData();
    preAuth->set("token", authToken);
    sendPackage(OP_AUTH_START, preAuth);
}

/*!
  Slot. Gets executed on entering "Authentication" state
  Checks operation code, searches user by login and check if password is
  hashed with previously generated authentication token
*/
void OdaClient::onAuthenticate()
{
    if (getOperation() != OP_AUTHENTICATE)
    {
        sendError(ERR_PREAMBLE_INVALID);
        socket->close();
        return;
    }
    
    //Reading credentials
    OdaData* credentials = getPackage();
    
    //Searching user by database
    QSqlQuery q(*db);
    q.prepare(QString("SELECT * FROM User INNER JOIN Company ON Company.cid = User.cid WHERE User.login=? LIMIT 1"));
    q.bindValue(0, credentials->getString("login"));
    q.exec();
    q.first();
    QSqlRecord rec = q.record();

    QString login;
    QString pass;
    if (rec.count())
    {
        login = rec.field("login").value().toString();
        pass = rec.field("pass").value().toString();
    }

    //Hashing password with previously generated token
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(QString(authToken+pass).toAscii());

    //Authentication validity check
    if (!(login.length()
        && pass.length()
        && credentials->getString("login") == login
        && credentials->getString("password") == hash.result().toHex()
        ))
        {
        sendError(ERR_USER_INVALID);
        socket->close();
        return;
    }

    //Setting user Unique Identifier
    uid = rec.field("id").value().toInt();

    //Creating minimum info package
    OdaData* info = new OdaData();
    info->set("uid", rec.field("id").value().toInt());
    info->set("cid", rec.field("cid").value().toInt());
    info->set("fullName", rec.field("fullName").value().toString());
    info->set("companyName", rec.field("companyName").value().toString());

    //Sending User Minimum Info package with preamble
    sendPackage(OP_AUTHENTICATE, info);

    //Telling that authentication is successful
    emit authenticated();
}

/*!
  Slot. Gets executed on entering "Command Route" state
  Checks operation code, generates and sends authentication token
*/
void OdaClient::onCommand()
{
    //Lookup if we can serve the requested command
    switch (getOperation())
    {
    case OP_GET_USER_INFO:
        emit getUserInfo();
        break;

    default:
        //Ignore command if we can't process it
        socket->readAll();
        emit commandUnknown();
        break;
    }
}

/*!
  Slot. Gets executed on OP_GET_USER_INFO command
  Collects user info and sends user info package
*/
void OdaClient::onGetUserInfo()
{
    emit commandDone();
}
