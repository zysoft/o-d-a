/*!
    Open Development Assistant Server. Operation "Status Notification"
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

#include "odastatusnotification.h"

OdaStatusNotification::OdaStatusNotification()
{
}

/*!
  Performs client side processing
*/
void OdaStatusNotification::doClientSide()
{
    emit userStatus(sourcePackage);
    emit commandDone();
}

/*!
  Performs server side processing
*/
void OdaStatusNotification::doServerSide()
{
    emit packageReady(NF_STATUS, sourcePackage);
    emit commandDone();
}

/*!
  Returns operation code
*/
qint16 OdaStatusNotification::getOperationCode()
{
    return NF_STATUS;
}
