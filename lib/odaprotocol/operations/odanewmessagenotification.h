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

#ifndef ODANEWMESSAGENOTIFICATION_H
#define ODANEWMESSAGENOTIFICATION_H

#include <QObject>
#include "odaabstractoperation.h"

/*!
  Implements New Message Notification from both client and server sides
*/
class OdaNewMessageNotification : public OdaAbstractOperation
{
    Q_OBJECT

public:
    OdaNewMessageNotification();

protected:
    virtual qint16 getOperationCode();

public slots:
    virtual void doClientSide();
    virtual void doServerSide();
};

#endif // ODANEWMESSAGENOTIFICATION_H
