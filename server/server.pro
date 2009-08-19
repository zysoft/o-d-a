# -------------------------------------------------
# Project created by QtCreator 2009-08-04T19:04:13
# -------------------------------------------------
QT += network \
    sql
QT -= gui
TARGET = o-d-a-server
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    odaserver.cpp \
    ../lib/odaprotocol/odadata.cpp \
    ../lib/odaprotocol/odaconnection.cpp \
    ../lib/odaprotocol/operations/odaabstractoperation.cpp \
    ../lib/odaprotocol/operations/odaauthstep1.cpp \
    ../lib/odaprotocol/operations/odaauthstep2.cpp \
    ../lib/odaprotocol/operations/odagetcontactlist.cpp \
    ../lib/odaprotocol/operations/odastatusnotification.cpp \
    ../lib/odaprotocol/operations/odasendmessage.cpp \
    ../lib/odaprotocol/operations/odanewmessagenotification.cpp
HEADERS += odaserver.h \
    ../lib/odaprotocol/odadefinitions.h \
    ../lib/odaprotocol/odadata.h \
    ../lib/odaprotocol/odaconnection.h \
    ../lib/odaprotocol/operations/odaabstractoperation.h \
    ../lib/odaprotocol/operations/odaauthstep1.h \
    ../lib/odaprotocol/operations/odaauthstep2.h \
    ../lib/odaprotocol/operations/odagetcontactlist.h \
    ../lib/odaprotocol/operations/odastatusnotification.h \
    ../lib/odaprotocol/operations/odasendmessage.h \
    ../lib/odaprotocol/operations/odanewmessagenotification.h \
    ../lib/odaprotocol/operations/odaoperations.h
include( ../lib/qtstatemachine/src/qtstatemachine.pri)
