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

#ifndef ODADATA_H
#define ODADATA_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QByteArray>

/*!
  Universal Data Package class
  Used to transmit various protocol data.
  Supports one Universal Data Package to another inclusion
*/
class OdaData : public QObject
{
private:
    QHash<QString, QVariant> scalars;          ///< Scalar properties
    QHash<QString, OdaData > objects;          ///< Object properties
public:
    OdaData();
    OdaData(const OdaData&);

    void set(QString, QVariant);
    void setObject(QString, OdaData);

    QVariant get(QString);
    int getInt(QString);
    QString getString(QString);
    OdaData getObject(QString);

    QByteArray serialize();
    static OdaData unserialize(QByteArray*);

    OdaData& operator=(const OdaData& foreign);
};

#endif // ODADATA_H
