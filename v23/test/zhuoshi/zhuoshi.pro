TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    app.cpp

include(../common.pri)
LIBS+=-lpthread
HEADERS+=$$SERVER_PATH/server.h \
    test1.h \
    app.h
SOURCES+=$$SERVER_PATH/server.cpp
