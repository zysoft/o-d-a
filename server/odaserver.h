/*!
    Open Development Assistant Server
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

#ifndef ODASERVER_H
#define ODASERVER_H

#include <QObject>
#include <QTcpServer>
#include <QHash>
#include <QSqlDatabase>
#include "odaclient.h"

/*!
  Server class
*/
class OdaServer : public QObject
{
    Q_OBJECT
private:
    QTcpServer  socket;                 ///< Listening socket
    QSqlDatabase db;                    ///< Service database connection
    QHash<QString, OdaClient*> clients; ///< List of active clients

public:
    inline OdaServer() {}
    void start(int port, QString dbHost, QString dbUser, QString dbPass, QString dbName);
    void stop();

private slots:
    void clientConnected();
    void clientDisconnected();
    void onSignalRoute(qint16, unsigned int, OdaData);

signals:
    void signalRoute(qint16, unsigned int, OdaData);    ///< Signal for data routing between connected clients
};

#endif // ODASERVER_H
