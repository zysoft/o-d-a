/*!
    Open Development Assistant Server. Operation "Authentication step 2"
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

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QCryptographicHash>
#include "odaauthstep2.h"


OdaAuthStep2::OdaAuthStep2()
{
}

/*!
  Sets auth token for authentication process
*/
void OdaAuthStep2::setAuthToken(QString token)
{
    authToken = token;
}


/*!
  Installs operation

  \see OdaAbstractOperation::install
*/
void OdaAuthStep2::install(OdaConnection* connection, QtState* authenticate, QSqlDatabase* database, bool serverSide)
{
    db = database;
    authenticate->invokeMethodOnEntry(connection, "onCommand");
    connect(this, SIGNAL(commandExecute()), this, (serverSide) ? SLOT(doServerSide()) : SLOT(doClientSide()));
    connect(connection, SIGNAL(operation(qint16,OdaData)), this, SLOT(onOperation(qint16,OdaData)));
    connection->connect(this, SIGNAL(packageReady(qint16,OdaData)), connection, SLOT(sendPackage(qint16,OdaData)));
    connection->connect(this, SIGNAL(packageReady(qint16, bool)), connection, SLOT(sendPackage(qint16,bool)));
    connection->connect(this, SIGNAL(error(int, bool)), connection, SLOT(sendError(int, bool)));
    connection->connect(this, SIGNAL(authenticated(uint,uint)), connection, SLOT(proxyAuth(uint,uint)));
}

/*!
  Performs client side processing
*/
void OdaAuthStep2::doClientSide()
{
    if (sourcePackage.getInt("uid"))
    {
        uid = sourcePackage.getInt("uid");
        cid = sourcePackage.getInt("cid");
        emit authenticated(uid, cid);
        emit userMinimumInfo(sourcePackage);
    }
}

/*!
  Performs server side processing
*/
void OdaAuthStep2::doServerSide()
{
    //Searching user by database
    QSqlQuery q(*db);
    q.prepare(QString("SELECT * FROM User INNER JOIN Company ON Company.cid = User.cid WHERE User.login=? LIMIT 1"));
    q.bindValue(0, sourcePackage.getString("login"));
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
        && sourcePackage.getString("login") == login
        && sourcePackage.getString("password") == hash.result().toHex()
        ))
    {
        emit error(ERR_USER_INVALID, true);
        return;
    }

    //Setting user Unique Identifier
    uid = rec.field("uid").value().toInt();
    cid = rec.field("cid").value().toInt();

    //Creating minimum info package
    OdaData info;
    info.set("uid", rec.field("uid").value().toInt());
    info.set("cid", rec.field("cid").value().toInt());
    info.set("fullName", rec.field("fullName").value().toString());
    info.set("companyName", rec.field("companyName").value().toString());

    //Sending User Minimum Info package with preamble
    emit packageReady(OP_AUTHENTICATE, info);

    //Telling that authentication is successful
    emit authenticated(uid, cid);
}

/*!
  Returns operation code
*/
qint16 OdaAuthStep2::getOperationCode()
{
    return OP_AUTHENTICATE;
}
