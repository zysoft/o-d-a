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
    odaclient.cpp
HEADERS += odaclient.h \
    odaserver.h \
    ../lib/odapackages.h
include( ../lib/qtstatemachine/src/qtstatemachine.pri)
