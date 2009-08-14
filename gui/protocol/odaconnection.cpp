/*!
    Open Development Assistant Gui. Client-Server data exchange class
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
#include <QtCore/QCoreApplication>
#include "odaconnection.h"

/*!
  Constructs an object
  Initializes client state machine and socket
*/
OdaConnection::OdaConnection()
{
    if (isInitialized)
    {
        return;
    }

    isInitialized = true;

    QtState *preInit = new QtState();
    QtState *init = new QtState();
    QtState *authStep1 = new QtState();
    QtState *authStep2 = new QtState();
    QtState *dataWait = new QtState();
    QtState *dataRoute = new QtState();
    QtState *getUserInfo = new QtState();
    QtState *getContactList = new QtState();
    QtState *getMessage = new QtState();

    preInit->addTransition(&socket, SIGNAL(connected()), init);
    init->addTransition(&socket, SIGNAL(readyRead()), authStep1);
    authStep1->addTransition(&socket, SIGNAL(readyRead()), authStep2);
    authStep1->addTransition(this, SIGNAL(error(QString)), preInit);
    authStep2->addTransition(this, SIGNAL(authenticated()), dataWait);
    authStep2->addTransition(this, SIGNAL(error(QString)), preInit);
    dataWait->addTransition(&socket, SIGNAL(readyRead()), dataRoute);
    dataWait->addTransition(this, SIGNAL(socketReady()), dataRoute);
    dataRoute->addTransition(this, SIGNAL(op_getUserInfo()), getUserInfo);
    dataRoute->addTransition(this, SIGNAL(op_getContactList()), getContactList);
    dataRoute->addTransition(this, SIGNAL(op_getMessage()), getMessage);
    dataRoute->addTransition(this, SIGNAL(commandDone()), dataWait);
    getUserInfo->addTransition(this, SIGNAL(commandDone()), dataWait);
    getContactList->addTransition(this, SIGNAL(commandDone()), dataWait);
    getMessage->addTransition(this, SIGNAL(commandDone()), dataWait);


    init->invokeMethodOnEntry(this, "onInit");
    authStep1->invokeMethodOnEntry(this, "onAuthStep1");
    authStep2->invokeMethodOnEntry(this, "onAuthStep2");
    dataWait->invokeMethodOnEntry(this, "socketCheck");
    dataRoute->invokeMethodOnEntry(this, "onData");
    getUserInfo->invokeMethodOnEntry(this, "doGetUserInfo");
    getContactList->invokeMethodOnEntry(this, "doGetContactList");
    getMessage->invokeMethodOnEntry(this, "doGetMessage");

    stateMachine.addState(preInit);
    stateMachine.addState(init);
    stateMachine.addState(authStep1);
    stateMachine.addState(authStep2);
    stateMachine.addState(dataWait);
    stateMachine.addState(dataRoute);
    stateMachine.addState(getUserInfo);
    stateMachine.addState(getContactList);
    stateMachine.addState(getMessage);

    stateMachine.setInitialState(preInit);
    stateMachine.initialState();
    stateMachine.start();

    connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}


/*!
  Removes self instance
*/
OdaConnection::~OdaConnection()
{
    if (self != 0)
    {
        delete self;
    }
}

/*!
  Returns self instance
*/
OdaConnection* OdaConnection::getInstance()
{
    if (self == 0)
    {
        self = new OdaConnection();
    }

    return self;
}


/*!
  Initiates server session

  \param host       Server host to connect to
  \param port       Server port
  \param loginName  User login
  \param passw      User password
*/
void OdaConnection::initiate(QString host, int port, QString loginName, QString passw)
{
    login = loginName;
    pass = passw;


    if (socket.isOpen())
    {
        socket.close();
    }
    socket.connectToHost(host, port);

}

/*!
  Retrieves error package and sends error signal with error explanation by error code
*/
void OdaConnection::emitError()
{
    OdaData err = getPackage();

    switch (err.getInt("errorCode"))
    {
    case ERR_PREAMBLE_INVALID :
        emit error(tr("Protocol violation"));
        break;
    case ERR_USER_INVALID:
        emit error(tr("Incorrect login/password"));
        break;
    default:
        emit error(tr("Unknown error, try again"));
        break;
    }

}

/*!
  Sends data package

  \param operation Operation code
  \param package      Data package
*/
void OdaConnection::sendPackage(qint16 operation, OdaData package)
{
    sendPackage(operation, true);

    QByteArray data = package.serialize();
    qint16 size = data.size()+1; //+1 is to always send \0
    socket.write((char*)&size, sizeof(qint16));
    socket.write(data.constData(), size);
}

/*!
  Sends data package

  \param operation      Operation code
  \param dataComesLater Idicates if the package is empty or data package comes later
*/
void OdaConnection::sendPackage(qint16 operation, bool dataComesLater)
{
    socket.write((char*)&operation, sizeof(qint16));
    if (!dataComesLater)
    {
        qint16 size = 0;
        socket.write((char*)&size, sizeof(qint16));
    }
}


/*!
  Reads operation code

  \return Operation code
*/
qint16 OdaConnection::getOperation()
{
    qint16 operation;
    socket.read((char*)&operation, sizeof(qint16));
    return operation;
}


/*!
  Reads data package without preamble

  \return Data package
*/
OdaData OdaConnection::getPackage()
{
    qint16 size;
    socket.read((char*)&size, sizeof(qint16));
    if (size == 0)
    {
        return OdaData();
    }

    char* datac = (char*)malloc(size);
    socket.read(datac, size);
    QByteArray data(datac);
    free(datac);
    return OdaData::unserialize(&data);
}



/*!
  Socket errors forwarder
  Receives socket error and forwards it by emiting own error sinal

  \param errorCode Socket error code
*/
void OdaConnection::onSocketError(QAbstractSocket::SocketError errorCode)
{
    switch (errorCode)
    {
    case QAbstractSocket::RemoteHostClosedError:
    case QAbstractSocket::SocketTimeoutError:
    case QAbstractSocket::NetworkError:
        emit disconnected();
        break;
    default:
        emit error(socket.errorString());
    }
}

/*!
  Checks if the socket still has any data
  This is needed because QTcpSocket::readyRead signal is emited once on data arrival
*/
void OdaConnection::socketCheck()
{
    if (socket.bytesAvailable())
    {
        emit socketReady();
    }
}

/*!
  Slot which takes an action on "Init" state
*/
void OdaConnection::onInit()
{
    sendPackage(OP_AUTH_START);
}

/*!
  Slot which takes an action on "Authentication step 1" state
*/
void OdaConnection::onAuthStep1()
{

    if (getOperation() != OP_AUTH_START)
    {
        emitError();
        return;
    }

    OdaData preAuth = getPackage();

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(pass.toUtf8());
    QString psw = hash.result().toHex();
    hash.reset();
    hash.addData(QString(preAuth.getString("token")+psw).toAscii());

    OdaData auth;
    auth.set("login" , login);
    auth.set("password" , hash.result().toHex());

    sendPackage(OP_AUTHENTICATE, auth);
}


/*!
  Slot which takes an action on "Authentication step 2" state
*/
void OdaConnection::onAuthStep2()
{

    if (getOperation() != OP_AUTHENTICATE)
    {
        emitError();
        return;
    }

    emit authenticated();

    OdaData info = getPackage();
    emit userMiminumInfo(info);
}

/*!
  Slot which takes an action on "Data route" state
*/
void OdaConnection::onData()
{
    switch (getOperation())
    {
    case OP_GET_USER_INFO:
        emit op_getUserInfo();
        break;
    case OP_GET_CONTACTS:
        emit op_getContactList();
        break;
    case NF_NEW_MESSAGE:
        emit op_getMessage();
        break;
    case NF_STATUS:
        emit userStatus(getPackage());
        emit commandDone();
        break;

    default:
        socket.readAll();
        emit commandUnknown();
        break;
    }

}


/*!
  Performs user info package read
*/
void OdaConnection::doGetUserInfo()
{
    OdaData info = getPackage();
    emit commandDone();

}


/*!
  Performs user contacts list package read
*/
void OdaConnection::doGetContactList()
{
    OdaData contacts = getPackage();

    emit userContactList(contacts);
    emit commandDone();

}


/*!
  Performs user message package read
*/
void OdaConnection::doGetMessage()
{
    OdaData message = getPackage();

    emit userMessage(message);
    emit commandDone();

}



/*!
  Requests user info from server

  \todo Needs to be implemented
*/
void OdaConnection::requestUserInfo()
{

}


/*!
  Requests contacts list from server

  \param contactType Identifies contact type required (compant, office, team)
  \param pid         Project id to get contast for (in case contactType = TEAM_CONTACTS
*/
void OdaConnection::requestContactList(int contactType, int pid)
{
    OdaData request;
    request.set("contactsType", contactType);
    request.set("pid", pid);
    sendPackage(OP_GET_CONTACTS, request);
}

/*!
  Sends a message to users

  \param uids       User ids to send message to
  \param message    Message text
*/
void OdaConnection::sendChatMessage(QVector<int> uids, QString message)
{
    OdaData msg;

    int count = uids.count();
    msg.set("message", message);
    msg.set("count", count);
    for (int i=0; i<count; i++)
    {
        msg.set("uid"+QString().setNum(i), uids.at(i));
    }

    sendPackage(OP_SEND_MESSAGE, msg);
}


//Static properties declaration
bool OdaConnection::isInitialized;
QtStateMachine OdaConnection::stateMachine;
QTcpSocket OdaConnection::socket;
QString OdaConnection::login;
QString OdaConnection::pass;

OdaConnection* OdaConnection::self = 0;
