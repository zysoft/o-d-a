/*!
    Open Development Assistant Server. Operation "Send Message"
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

#include "odasendmessage.h"

OdaSendMessage::OdaSendMessage()
{
}

/*!
  Performs client side processing
*/
void OdaSendMessage::doClientSide()
{
    emit commandDone();
}

/*!
  Performs server side processing
*/
void OdaSendMessage::doServerSide()
{
    int count = sourcePackage.getInt("count");
    for (int i=0; i<count; i++)
    {
        OdaData msg;
        msg.set("from", uid);
        msg.set("message", sourcePackage.getString("message"));
        int targetUid = sourcePackage.getInt("uid"+QString().setNum(i));
        emit route(NF_NEW_MESSAGE, targetUid, msg);
    }
    emit commandDone();
}

/*!
  Returns operation code
*/
qint16 OdaSendMessage::getOperationCode()
{
    return OP_SEND_MESSAGE;
}
