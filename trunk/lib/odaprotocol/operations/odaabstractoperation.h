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

#ifndef ODAABSTRACTOPERATION_H
#define ODAABSTRACTOPERATION_H

#include <QObject>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QtStateMachine>
#include <QtState>
#include "../odadefinitions.h"
#include "../odadata.h"
#include "../odaconnection.h"

/*!
  Abstract protocol operation
  It tunes up its link with connection object, state machine, etc.
*/
class OdaAbstractOperation : public QObject
{
    Q_OBJECT

protected:
    QSqlDatabase* db;                       ///< Database object
    OdaData sourcePackage;                  ///< Data package from client/server
    unsigned int uid;                       ///< User unique identifier
    unsigned int cid;                       ///< Comnpany unique identifier

    virtual qint16 getOperationCode() = 0;  ///< Returns operation code the object represents
public:
    OdaAbstractOperation();
    virtual void install(OdaConnection*, QtStateMachine*, QtState* routeState, QtState* waitState, QSqlDatabase*, bool serverSide = false);

public slots:
    void onOperation(qint16, OdaData package);
    void onAuthenticate(unsigned int, unsigned int);
    virtual void doClientSide() = 0;                    ///< Performs operation processing for client side
    virtual void doServerSide() = 0;                    ///< Performs operation processing for server side
signals:
    void commandExecute();                              ///< Signal indicating that current operation has to be executed
    void commandDone();                                 ///< Indicates command execution finish
    void route(qint16, unsigned int, OdaData);          ///< Performs data exchange throught "routing" via connection and upper level objects
    void packageReady(qint16, OdaData);                 ///< Sends package to connection object and then to another side via socket
    void packageReady(qint16, bool = false);            ///< Sends empty package to connection object and then to another side via socket
    void error(int, bool = false);                      ///< Sends error with code and fatal identificator
};

#endif // ODAABSTRACTOPERATION_H
