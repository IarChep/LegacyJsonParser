#-------------------------------------------------
#
# Project created by QtCreator 2023-07-19T21:38:54
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = YTAPIJSON
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
OTHER_FILES += test.json

SOURCES += main.cpp \
    jsonparser.cpp

HEADERS += jsonparser.h
