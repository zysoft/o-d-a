/*!
    Open Development Assistant. Contact representation item
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

#ifndef ODACONTACTITEM_H
#define ODACONTACTITEM_H

#include <QListWidgetItem>
#include "../../lib/odaprotocol/odadefinitions.h"

/*!
  Contact representation item
  Represents user in contact list for QListWidget component
*/
class OdaContactItem : public QListWidgetItem
{
private:
    int UID;    ///< User id
public:
    OdaContactItem();
    void setUid(int userId);
    int uid();
    void updateStatus(int status = ST_OFFLINE);
};

#endif // ODACONTACTITEM_H
