/*!
    Open Development Assistant Server main file
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

#include <iostream>
#include <QtCore/QCoreApplication>
#include "odaserver.h"

/*!
  Main function
*/
int main(int argc, char *argv[])
{
    enum Arguments {SELF, PORT, DBHOST, DBUSER, DBPASS, DBNAME};

    //Checking arguments
    if (argc < 6)
    {

        std::cout << "Open Development Assistant Server Copyright (C) 2009  Yuriy Zisin (zysoftik@gmail.com)" << std::endl;
        std::cout << "This program comes with ABSOLUTELY NO WARRANTY; see COPYING for details." << std::endl;
        std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
        std::cout << "under certain conditions; see COPYING for details." << std::endl << std::endl;
        std::cout << "Call: " << argv[SELF] << " port dbHost dbUser dbPass dbName" << std::endl;
        std::cout << "Parameters are:" << std::endl;
        std::cout << " - port    - Port to listen at" << std::endl;
        std::cout << " - dbHost  - System database host" << std::endl;
        std::cout << " - dbUser  - System database username" << std::endl;
        std::cout << " - dbPass  - System database user password" << std::endl;
        std::cout << " - dbName  - System database name" << std::endl;
        return 1;
    }

    //Creating application
    QCoreApplication a(argc, argv);
    a.setApplicationName("OdaServer");
    a.setOrganizationName("Open Development Assistant");

    //Starting server
    OdaServer* server = new OdaServer();
    try
    {
        server->start(QString(argv[PORT]).toInt(), QString(argv[DBHOST]), QString(argv[DBUSER]), QString(argv[DBPASS]), QString(argv[DBNAME]));
    }
    catch (QString err)
    {
        std::cout << err.toUtf8().data() << std::endl;
        server->stop();
        return 1;
    }

    return a.exec();
}

