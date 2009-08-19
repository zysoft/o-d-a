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

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include "odagetcontactlist.h"
#include "../../../server/odaserver.h"

OdaGetContactList::OdaGetContactList()
{
}

/*!
  Performs client side processing
*/
void OdaGetContactList::doClientSide()
{
    emit commandDone();
}

/*!
  Performs server side processing
*/
void OdaGetContactList::doServerSide()
{
    //Searching contacts by database
    QSqlQuery q(*db);
    QString query = "SELECT * FROM User WHERE uid <> " + QString().setNum(uid);

    if (sourcePackage.getInt("contactsType") == COMPANY_CONTACTS)
    {
        query += " AND cid = " + QString().setNum(cid);
    }

    q.exec(query);

    OdaData contacts;
    contacts.set("count", q.size());
    qint16 i = 0;

    QVector<int> online = OdaServer::getOnliners();
    while(q.next())
    {
        QSqlRecord rec = q.record();
        OdaData contact;
        contact.set("uid", rec.field("uid").value().toInt());
        contact.set("fullName", rec.field("fullName").value().toString());
        contact.set("status", (online.contains(rec.field("uid").value().toInt())) ? ST_ONLINE : ST_OFFLINE);
        contacts.setObject(QString("contact").append(QString().setNum(i++)), contact);
    }

    emit packageReady(OP_GET_CONTACTS, contacts);
    emit commandDone();
}

/*!
  Returns operation code
*/
qint16 OdaGetContactList::getOperationCode()
{
    return OP_GET_CONTACTS;
}
