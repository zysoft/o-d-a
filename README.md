***! WARNINIG !***

The work on the project is on very beginning stage.
Note, almost everything can be changed later, so
don't rely on what you see right now.


I. Prerequisites:

1. Download Qt State Machine Framework from http://www.qtsoftware.com/products/appdev/add-on-products/catalog/4/Utilities/qt-state-machine-framework
2. Unpack it into lib/qtstatemachine
3. Create MySQL user and database (use UTF-8 database charset)
4. Import database dump from misc/database.sql

II. Making:

1. Make server:
    cd server
    qmake
    make
2. Make client:
    cd gui
    qmake
    make

III. Running
    1. Run server:
```
	cd server
	./o-d-a-server port dbHost dbUser dbPass dbName 2>&1>/dev/null &
```
    2. Run client:
```
	cd gui
	./o-d-a
```
    3. Login as user "admin" and password "admin"
    4. Enjoy!

Check [Wiki](https://github.com/zysoft/o-d-a/wiki) for details.