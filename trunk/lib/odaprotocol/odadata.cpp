/*!
    Open Development Assistant Universal Data Package
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

#include "odadata.h"

#define SPLIT_MARKER 0xFF   ///< Marker that indicates field name/value start
#define OBJECT_MARKER 0x0A  ///< Marker that indicates base64 serialized object propery


OdaData::OdaData()
{
}

/*!
  Adds/sets scalar property

  \param name Property name
  \param value Property value
*/
void OdaData::set(QString name, QVariant value)
{
    data[name] = value;
}

/*!
  Adds/sets object property

  \param name Property name
  \param value Object
*/
void OdaData::setObject(QString name, OdaData* value)
{
    packages.insert(name, value);
}

/*!
  Returns scalar property value by name

  \param name Property name
*/
QVariant OdaData::get(QString name)
{
    if (data.find(name) != data.end())
    {
        return data.value(name);
    }
    return QVariant();
}

/*!
  Returns scalar int property value by name

  \param name Property name
*/
int OdaData::getInt(QString name)
{
    return get(name).toInt();
}

/*!
  Returns scalar string property value by name

  \param name Property name
*/
QString OdaData::getString(QString name)
{
    return get(name).toString();
}


/*!
  Returns object property value by name

  \param name Property name
*/
OdaData* OdaData::getObject(QString name)
{
    return packages.value(name);
}

/*!
  Serializes object with all properties

  \return QByteArray with serialized object
*/
QByteArray OdaData::serialize()
{
    QByteArray serial;

    //Going through the scalar properties
    QHashIterator<QString, QVariant> i(data);
    i.toFront();
    while (i.hasNext())
    {
        i.next();
        serial.append(SPLIT_MARKER);
        serial.append(i.key());
        serial.append(SPLIT_MARKER);
        serial.append(i.value().toByteArray().toBase64());
    }

    //Going through the object properties
    QHashIterator<QString, OdaData*> j(packages);
    j.toFront();
    while (j.hasNext())
    {
        j.next();
        serial.append(SPLIT_MARKER);
        serial.append(j.key());
        serial.append(SPLIT_MARKER);
        serial.append(OBJECT_MARKER);
        serial.append(packages[j.key()]->serialize().toBase64());
    }

    return serial;
}

/*!
  Unserializes object with all properties

  \param  serial QByteArray with serialized object
  \return Initialized Universal package object
*/
OdaData* OdaData::unserialize(QByteArray* serial)
{

    OdaData* package = new OdaData();

    int pos = serial->indexOf(SPLIT_MARKER)+1;
    int nextPos;
    while (pos)
    {
        //We seek between split markers
        nextPos = serial->indexOf(SPLIT_MARKER, pos) + 1;
        if (!nextPos)
        {
            break;
        }

        //Retrieving property key
        QString key(serial->mid(pos, nextPos - pos - 1));
        pos = nextPos;
        nextPos = serial->indexOf(SPLIT_MARKER, nextPos) + 1;

        //Checking if value is an object property value
        bool isObject = false;
        if (serial->at(pos) == OBJECT_MARKER)
        {
            isObject = true;
            //Omiting object marker
            pos++;
        }

        //Retrieving value
        QString value = QString().fromUtf8(QByteArray().fromBase64(serial->mid(pos, nextPos - pos - 1)));

        if (isObject)
        {
            //Initializing object property if the value is object property
            QByteArray serialized = QByteArray().fromBase64(value.toAscii());
            package->setObject(key, OdaData::unserialize(&serialized));
        }
        else
        {
            //Setting retrieved protperty into the package
            package->set(key, value);
        }

        pos = nextPos;
    }
    return package;
}
