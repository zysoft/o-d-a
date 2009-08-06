# -------------------------------------------------
# Project created by QtCreator 2009-08-05T12:31:16
# -------------------------------------------------
QT += network
TARGET = o-d-a
TEMPLATE = app
SOURCES += main.cpp \
    loginwindow.cpp \
    protocol/odaconnection.cpp \
    mainwindow.cpp \
    settingswindow.cpp \
    ../lib/odaprotocol/odadata.cpp
HEADERS += loginwindow.h \
    protocol/odaconnection.h \
    mainwindow.h \
    settingswindow.h \
    ../lib/odaprotocol/odadefinitions.h \
    ../lib/odaprotocol/odadata.h
FORMS += loginwindow.ui \
    mainwindow.ui \
    settingswindow.ui
include(../lib/qtstatemachine/src/qtstatemachine.pri)
