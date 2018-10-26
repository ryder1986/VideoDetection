TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QT+=sql
SOURCES += \
        main.cpp
include(../common.pri)
HEADERS +=\
    $$SERVER_PATH/cppjson/include/json/reader.h\
    $$SERVER_PATH/cppjson/include/json/writer.h\
    $$SERVER_PATH/cppjson/include/json/value.h
SOURCES +=\
    $$SERVER_PATH/cppjson/json_reader.cpp \
    $$SERVER_PATH/cppjson/json_writer.cpp \
    $$SERVER_PATH/cppjson/json_value.cpp
#LIBS+=-lmysqlclient

INCLUDEPATH+= $$SERVER_PATH/cppjson/include

#win32:CONFIG(release, debug|release): LIBS += -LC:/mysql/lib64/vs14/ -lmysqlcppconn
#else:win32:CONFIG(debug, debug|release): LIBS += -LC:/mysql/lib64/vs14/ -lmysqlcppconnd
#else:unix: LIBS += -LC:/mysql/lib64/vs14/ -lmysqlcppconn

INCLUDEPATH += C:/mysql/include/jdbc/cppconn
DEPENDPATH += C:/mysql/lib64/vs14
