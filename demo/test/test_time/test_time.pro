TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
SERVER_PATH=../../server/
INCLUDEPATH +=$$SERVER_PATH/

HEADERS+=$$SERVER_PATH/tool.h \
    database.h
SOURCES+=$$SERVER_PATH/tool.cpp
DEFINES+=IS_UNIX
