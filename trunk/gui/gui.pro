# -------------------------------------------------
# Project created by QtCreator 2009-08-05T12:31:16
# -------------------------------------------------
QT += network \
    sql
TARGET = o-d-a
TEMPLATE = app
SOURCES += main.cpp \
    loginwindow.cpp \
    protocol/odaclientconnection.cpp \
    mainwindow.cpp \
    settingswindow.cpp \
    ../lib/odaprotocol/odadata.cpp \
    items/odacontactitem.cpp \
    chatwindow.cpp \
    ../lib/odaprotocol/odaconnection.cpp \
    ../lib/odaprotocol/operations/odastatusnotification.cpp \
    ../lib/odaprotocol/operations/odasendmessage.cpp \
    ../lib/odaprotocol/operations/odanewmessagenotification.cpp \
    ../lib/odaprotocol/operations/odagetcontactlist.cpp \
    ../lib/odaprotocol/operations/odaauthstep2.cpp \
    ../lib/odaprotocol/operations/odaauthstep1.cpp \
    ../lib/odaprotocol/operations/odaabstractoperation.cpp \
    ../server/odaserver.cpp
HEADERS += loginwindow.h \
    protocol/odaclientconnection.h \
    mainwindow.h \
    settingswindow.h \
    ../lib/odaprotocol/odadefinitions.h \
    ../lib/odaprotocol/odadata.h \
    items/odacontactitem.h \
    chatwindow.h \
    ../lib/odaprotocol/odaconnection.h \
    ../lib/odaprotocol/operations/odasendmessage.h \
    ../lib/odaprotocol/operations/odaoperations.h \
    ../lib/odaprotocol/operations/odanewmessagenotification.h \
    ../lib/odaprotocol/operations/odagetcontactlist.h \
    ../lib/odaprotocol/operations/odaauthstep2.h \
    ../lib/odaprotocol/operations/odaauthstep1.h \
    ../lib/odaprotocol/operations/odaabstractoperation.h \
    ../lib/odaprotocol/operations/odastatusnotification.h \
    ../server/odaserver.h
FORMS += loginwindow.ui \
    mainwindow.ui \
    settingswindow.ui \
    chatwindow.ui
include(../lib/qtstatemachine/src/qtstatemachine.pri)
