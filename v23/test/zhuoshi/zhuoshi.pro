TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    app.cpp

include(../common.pri)
LIBS+=-lpthread
HEADERS+=$$SERVER_PATH/server.h $$SERVER_PATH/configmanager.h\
  $$SERVER_PATH/jsonpacket.h   test1.h \
    app.h
SOURCES+=$$SERVER_PATH/server.cpp
   LIBS+=-lmysqlclient

INCLUDEPATH+= $$SERVER_PATH/cppjson/include
HEADERS +=\
    $$SERVER_PATH/cppjson/include/json/reader.h\
    $$SERVER_PATH/cppjson/include/json/writer.h\
    $$SERVER_PATH/cppjson/include/json/value.h
SOURCES +=\
    $$SERVER_PATH/cppjson/json_reader.cpp \
    $$SERVER_PATH/cppjson/json_writer.cpp \
    $$SERVER_PATH/cppjson/json_value.cpp
CONIG_FILE.path=$$OUT_PWD/
CONIG_FILE.files=res

INSTALLS+=$$CONIG_FILE

DISTFILES += \
    res/config.json
