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

#include "odaclientconnection.h"

/*!
  Constructs an object
  Initializes client state machine and socket
*/
OdaClientConnection::OdaClientConnection() : OdaConnection("CLIENT", &socket, false)
{

}


/*!
  Removes self instance
*/
OdaClientConnection::~OdaClientConnection()
{
    if (self != 0)
    {
        delete self;
    }
}

/*!
  Returns self instance
*/
OdaClientConnection* OdaClientConnection::getInstance()
{
    if (self == 0)
    {
        self = new OdaClientConnection();
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
void OdaClientConnection::setup(QString host, int port, QString loginName, QString passw)
{
    login = loginName;
    pass = passw;
    hostName = host;
    portNum = port;
}

/*!
  Establishes connection to server
*/
void OdaClientConnection::establishConnection()
{
    OdaAuthStep1* auth1 = static_cast<OdaAuthStep1*>(getOperation(OP_AUTH_START));
    auth1->setCredentials(login, pass);

    if (socket.isOpen())
    {
        socket.close();
    }

    emit init();

    socket.connectToHost(hostName, portNum);
}


/*!
  Sends server a command
*/
void OdaClientConnection::sendCommand(qint16 operationCode, OdaData package)
{
    sendPackage(operationCode, package);
}

/*!
  Sends server a command
*/
void OdaClientConnection::sendCommand(qint16 operationCode)
{
    sendPackage(operationCode);
}


//Initializing statics
QString OdaClientConnection::login;
QString OdaClientConnection::pass;
QString OdaClientConnection::hostName;
int OdaClientConnection::portNum;
QTcpSocket OdaClientConnection::socket;
OdaClientConnection* OdaClientConnection::self = 0;
