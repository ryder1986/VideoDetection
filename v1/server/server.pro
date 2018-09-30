TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp




HEADERS +=\
    cppjson/include/json/reader.h\
    cppjson/include/json/writer.h\
    cppjson/include/json/value.h
SOURCES +=\
    cppjson/json_reader.cpp \
    cppjson/json_writer.cpp \
    cppjson/json_value.cpp
