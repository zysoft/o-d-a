/*!
    Open Development Assistant Data Exchange Protocol constants and packages definition
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

#ifndef ODAPACKAGES_H
#define ODAPACKAGES_H

#include <QString>
#include <QHash>

/*!
  Operations and notifications enum
*/
enum OdaOperations
{
    //Operation constants
    OP_NOP,
    OP_AUTH_START,
    OP_AUTHENTICATE,
    OP_GET_USER_INFO,
    OP_GET_CONTACTS,
    OP_SEND_MESSAGE,
    OP_TASK_REPORT,
    OP_GET_TASK_PARAMETERS,
    OP_CREATE_TASK,

    //Notification constants
    NF_STATUS,
    NF_TASK_UPDATE,
    NF_NEW_MESSAGE
};


/*!
  Error codes enum
*/
enum OdaErrors
{
    ERR_UNKNOWN,
    ERR_PREAMBLE_INVALID,
    ERR_USER_INVALID
};


/*!
  Server error package
*/
struct SError
{
    int errorCode;
};

/*!
  Client Auth package
*/
struct CAuthPackage
{
    char login[50];
    char password[255];
};

/*!
  Server auth package
*/
struct SAuthPackage
{
    char token[255];
};

/*!
  Server user minimum info package
*/
struct SUserMinimumInfo
{
    unsigned int uid;
    unsigned int cid;
    char fullName[255];
    char companyName[255];
};


#endif // ODAPACKAGES_H
