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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QtStateMachine>
#include "../lib/odaprotocol/odadefinitions.h"
#include "../lib/odaprotocol/odadata.h"

/*!
  Client-Server data exchange class
*/
class OdaConnection : public QObject
{
    Q_OBJECT
private:
    static bool isInitialized;          ///< Indicate object reinitialization
    static QtStateMachine stateMachine; ///< Client state machine
    static QTcpSocket socket;           ///< Connection socket
    static QString login;               ///< User login
    static QString pass;                ///< User password

    void emitError();
    void sendPackage(qint16 operation, OdaData* data = NULL);
    qint16 getOperation();
    OdaData* getPackage();

public:
    OdaConnection();
    void initiate(QString host, int port, QString loginName, QString passw);
    QTcpSocket* sock();

    void requestUserInfo();


private slots:
    //Main slots
    void onSocketError(QAbstractSocket::SocketError);
    void onInit();
    void onAuthStep1();
    void onAuthStep2();
    void onData();

    //Operation processors
    void doGetUserInfo();

signals:
    //Main signals
    void error(QString);                    ///< Error signal with human readable explanation
    void authenticated();                   ///< Successful authentication
    void commandDone();                     ///< Operation processing finished
    void commandUnknown();                  ///< Operation cannot be processed because it is unknown

    //Operation related signals
    void op_getUserInfo();                  ///< Request to perform "Get user info" operation
    void userMiminumInfo(); ///< Transmits user info package

};

#endif // CONNECTION_H
