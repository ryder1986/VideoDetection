TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += main.cpp \
    test1.cpp

include(../common.pri)

HEADERS+=$$SERVER_PATH/server.h \
    test1.h
SOURCES+=$$SERVER_PATH/server.cpp

    LIBS+=-lpthread
