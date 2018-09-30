TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    database.cpp
LIBS+=-lmysqlclient
SERVER_PATH=../../server/
INCLUDEPATH +=$$SERVER_PATH/

HEADERS+=$$SERVER_PATH/tool.h \
    database.h
SOURCES+=$$SERVER_PATH/tool.cpp
