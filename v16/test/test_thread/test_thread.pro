TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    testclass1.cpp
include(../common.pri)

HEADERS += \
    testclass1.h
