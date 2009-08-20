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

#ifndef ODAAUTHSTEP2_H
#define ODAAUTHSTEP2_H

#include "odaabstractoperation.h"

/*!
  Implements step 2 of authentication process from both client and server sides
*/
class OdaAuthStep2 : public OdaAbstractOperation
{
    Q_OBJECT

public:
    OdaAuthStep2();
    virtual void install(OdaConnection*, QtState* authenticate, QSqlDatabase* database, bool serverSide = false);
    void setAuthToken(QString);
protected:
    QString authToken;
    virtual qint16 getOperationCode();                   ///< Returns operation code the object represents
public slots:
    virtual void doClientSide();                         ///< Performs operation processing for client side
    virtual void doServerSide();                         ///< Performs operation processing for server side

signals:
    void authenticated(unsigned int, unsigned int);     ///< Signal indicating that user is successfully authenticated
    void userMinimumInfo(OdaData);                      ///< Tranports User Minimum Info package
};

#endif // ODAAUTHSTEP1_H
