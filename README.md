***! WARNINIG !***

The work on the project is on very beginning stage.
Note, almost everything can be changed later, so
don't rely on what you see right now.


## Prerequisites

1. Download Qt State Machine Framework from http://www.qtsoftware.com/products/appdev/add-on-products/catalog/4/Utilities/qt-state-machine-framework
2. Unpack it into lib/qtstatemachine
3. Create MySQL user and database (use UTF-8 database charset)
4. Import database dump from misc/database.sql

## Making

### Make server

```bash
    cd server
    qmake
    make
```

### Make client

```bash
    cd gui
    qmake
    make
```

## Running

### Run server 

```
	cd server
	./o-d-a-server port dbHost dbUser dbPass dbName 2>&1>/dev/null &   
```   

### Run client  

```
	cd gui
	./o-d-a
```
Login as user "admin" and password "admin"   
Enjoy!

Check [Wiki](https://github.com/zysoft/o-d-a/wiki) for details.
