/*!
    Open Development Assistant Server. Operation "New Message Notification"
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

#include "odanewmessagenotification.h"

OdaNewMessageNotification::OdaNewMessageNotification()
{
}

/*!
  Performs client side processing
*/
void OdaNewMessageNotification::doClientSide()
{
    emit userMessage(sourcePackage);
    emit commandDone();
}

/*!
  Performs server side processing
*/
void OdaNewMessageNotification::doServerSide()
{
    OdaData message;
    message.set("uid", sourcePackage.get("from"));
    message.set("message", sourcePackage.get("message"));
    emit packageReady(NF_NEW_MESSAGE, message);
    emit commandDone();
}

/*!
  Returns operation code
*/
qint16 OdaNewMessageNotification::getOperationCode()
{
    return NF_NEW_MESSAGE;
}
