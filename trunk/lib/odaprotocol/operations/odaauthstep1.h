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

#ifndef ODAAUTHSTEP1_H
#define ODAAUTHSTEP1_H

#include "odaabstractoperation.h"

/*!
  Implements step 1 of authentication process from both client and server sides
*/
class OdaAuthStep1 : public OdaAbstractOperation
{
    Q_OBJECT

public:
    OdaAuthStep1();
    virtual void install(OdaConnection*, QtState* init, QtState* preAuth, bool serverSide = false);
    QString getToken();
    void setCredentials(QString uLogin, QString uPass);

protected:
    QString login;                                       ///< User login for client side
    QString pass;                                        ///< User password for client side
    QString authToken;
    virtual qint16 getOperationCode();                   ///< Returns operation code the object represents

public slots:
    virtual void doClientSide();                         ///< Performs operation processing for client side
    virtual void doServerSide();                         ///< Performs operation processing for server side

};

#endif // ODAAUTHSTEP1_H
