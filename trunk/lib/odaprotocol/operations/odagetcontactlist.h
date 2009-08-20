/*!
    Open Development Assistant Server. Operation "Get contact list"
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

#ifndef ODAGETCONTACTLIST_H
#define ODAGETCONTACTLIST_H

#include <QObject>
#include "odaabstractoperation.h"

/*!
  Implements Get Contact List operation from both client and server sides
*/
class OdaGetContactList : public OdaAbstractOperation
{
    Q_OBJECT

public:
    OdaGetContactList();

protected:
    virtual qint16 getOperationCode();

public slots:
    virtual void doClientSide();
    virtual void doServerSide();

signals:
    void userContactLits(OdaData);
};

#endif // ODAGETCONTACTLIST_H
