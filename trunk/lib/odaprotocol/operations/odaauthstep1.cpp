/*!
    Open Development Assistant Server. Operation "Authentication step 1"
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

#include <QCryptographicHash>
#include "odaauthstep1.h"

OdaAuthStep1::OdaAuthStep1()
{
}

/*!
  Installs operation

  \see OdaAbstractOperation::install
*/
void OdaAuthStep1::install(OdaConnection* connection, QtState* init, QtState* preAuth, bool serverSide)
{
    if (!serverSide)
    {
        init->invokeMethodOnEntry(this, "doClientSide");
    }
    preAuth->invokeMethodOnEntry(connection, "onCommand");
    connect(this, SIGNAL(commandExecute()), this, (serverSide) ? SLOT(doServerSide()) : SLOT(doClientSide()));
    connect(connection, SIGNAL(operation(qint16,OdaData)), this, SLOT(onOperation(qint16,OdaData)));
    connection->connect(this, SIGNAL(packageReady(qint16,OdaData)), connection, SLOT(sendPackage(qint16,OdaData)));
    connection->connect(this, SIGNAL(packageReady(qint16, bool)), connection, SLOT(sendPackage(qint16,bool)));
    connection->connect(this, SIGNAL(error(int, bool)), connection, SLOT(sendError(int, bool)));
}

/*!
  Generates, saves and returns authentication token
*/
QString OdaAuthStep1::getToken()
{
    //Authentication token generation
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(QString().setNum(rand()+rand()+rand()).toAscii());
    authToken = hash.result().toHex();
    return authToken;
}


/*!
  Sets user credentials
*/
void OdaAuthStep1::setCredentials(QString uLogin, QString uPass)
{
    login = uLogin;
    pass = uPass;
}

/*!
  Performs client side processing
*/
void OdaAuthStep1::doClientSide()
{
    if (sourcePackage.getString("token").length())
    {
        authToken = sourcePackage.getString("token");

        QCryptographicHash hash(QCryptographicHash::Sha1);
        hash.addData(QString(pass).toAscii());
        QString passHash = hash.result().toHex();
        hash.reset();
        hash.addData(QString(authToken+passHash).toAscii());
        OdaData credentials;
        credentials.set("login", login);
        credentials.set("password", hash.result().toHex());
        emit packageReady(OP_AUTHENTICATE, credentials);

        sourcePackage = OdaData();

        return;
    }
    emit packageReady(OP_AUTH_START);
}

/*!
  Performs server side processing
*/
void OdaAuthStep1::doServerSide()
{
    //Sending authentication token package with preamble
    OdaData preAuth;
    preAuth.set("token", authToken);
    emit packageReady(OP_AUTH_START, preAuth);
}

/*!
  Returns operation code
*/
qint16 OdaAuthStep1::getOperationCode()
{
    return OP_AUTH_START;
}
