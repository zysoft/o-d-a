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

#ifndef ODACLIENTCONNECTION_H
#define ODACLIENTCONNECTION_H

#include <QObject>
#include "../lib/odaprotocol/odaconnection.h"
#include "../lib/odaprotocol/odadefinitions.h"
#include "../lib/odaprotocol/odadata.h"
#include "../lib/odaprotocol/operations/odaoperations.h"

/*!
  Client-Server data exchange class
*/
class OdaClientConnection : public OdaConnection
{
    Q_OBJECT

private:
    static QTcpSocket socket;
    static QString login;
    static QString pass;
    static QString hostName;
    static int portNum;

    OdaClientConnection();
    ~OdaClientConnection();
    static OdaClientConnection* self;         ///< Self instance

public:
    static OdaClientConnection* getInstance();
    static void setup(QString host, int port, QString loginName, QString passw);

    void establishConnection();
    void sendCommand(qint16 operationCode, OdaData package);
    void sendCommand(qint16 operationCode);
};

#endif // ODACLIENTCONNECTION_H
