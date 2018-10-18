TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS+=jsonpacket.h




include(../common.pri)
INCLUDEPATH+=$$SERVER_PATH/cppjson/include
HEADERS +=\
    $$SERVER_PATH/cppjson/include/json/reader.h\
    $$SERVER_PATH/cppjson/include/json/writer.h\
    $$SERVER_PATH/cppjson/include/json/value.h
SOURCES +=\
    $$SERVER_PATH/cppjson/json_reader.cpp \
    $$SERVER_PATH/cppjson/json_writer.cpp \
    $$SERVER_PATH/cppjson/json_value.cpp
