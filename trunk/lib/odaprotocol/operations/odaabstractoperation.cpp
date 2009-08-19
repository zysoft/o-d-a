/*!
    Open Development Assistant Server. Protocol abstract operation
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

#include "odaabstractoperation.h"

OdaAbstractOperation::OdaAbstractOperation()
{
}

/*!
  Installs the operation into connection object

  \param connection         Connection object
  \param stateMachine       State machine
  \param routeState         Routing state in state machine (predefined)
  \param waitState          Wait state in state machine (predefined)
  \param database           Database connection
  \param serverSide         Indicates operation mode - client (false) or server (true)
*/
void OdaAbstractOperation::install(OdaConnection* connection, QtStateMachine* stateMachine, QtState* routeState, QtState* waitState, QSqlDatabase* database, bool serverSide)
{
    db = database;
    QtState* doingOperation = new QtState();
    routeState->addTransition(this, SIGNAL(commandExecute()), doingOperation);
    //A hack for operations routhed throug the server
    waitState->addTransition(this, SIGNAL(commandExecute()), doingOperation);
    doingOperation->addTransition(this, SIGNAL(commandDone()), waitState);
    doingOperation->invokeMethodOnEntry(this, (serverSide) ? "doServerSide" : "doClientSide");
    stateMachine->addState(doingOperation);
    connect(connection, SIGNAL(authenticated(uint,uint)), this, SLOT(onAuthenticate(uint,uint)));
    connect(connection, SIGNAL(operation(qint16,OdaData)), this, SLOT(onOperation(qint16,OdaData)));
    connection->connect(this, SIGNAL(packageReady(qint16,OdaData)), connection, SLOT(sendPackage(qint16,OdaData)));
    connection->connect(this, SIGNAL(packageReady(qint16, bool)), connection, SLOT(sendPackage(qint16,bool)));
    connection->connect(this, SIGNAL(error(int, bool)), connection, SLOT(sendError(int, bool)));
    connection->connect(this, SIGNAL(route(qint16,uint,OdaData)), connection, SLOT(proxyRoute(qint16,uint,OdaData)));
}

/*!
  Gets executed on operation signal
  Checks if the requested operation corresponds one representing by the object

  \param operation  Operation code
  \param package    Data package
*/
void OdaAbstractOperation::onOperation(qint16 operation, OdaData package)
{
    
    if (operation != getOperationCode())
    {
        return;
    }

    sourcePackage = package;
    emit commandExecute();
}

/*!
  Gets executed when user successfully authenticates
  Allows all operations to know user identifiers

  \param uid    User unique identifier
  \param cid    User company identifier
*/
void OdaAbstractOperation::onAuthenticate(unsigned int uId, unsigned int cId)
{
    uid = uId;
    cid = cId;
}
