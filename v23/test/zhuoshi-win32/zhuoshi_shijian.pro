#-------------------------------------------------
#
# Project created by QtCreator 2018-10-25T15:36:10
#
#-------------------------------------------------

QT       += core gui network widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zhuoshi_shijian
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    tcpclient.cpp

HEADERS += \
        mainwindow.h \
    tcpclient.h

FORMS += \
        mainwindow.ui
include(../common.pri)
INCLUDEPATH+= $$SERVER_PATH/cppjson/include
HEADERS +=\
    $$SERVER_PATH/cppjson/include/json/reader.h\
    $$SERVER_PATH/cppjson/include/json/writer.h\
    $$SERVER_PATH/cppjson/include/json/value.h
SOURCES +=\
    $$SERVER_PATH/cppjson/json_reader.cpp \
    $$SERVER_PATH/cppjson/json_writer.cpp \
    $$SERVER_PATH/cppjson/json_value.cpp
#INSTALLS+=$$CONIG_FILE

DISTFILES += \
    res/config.json
HEADERS+=  $$SERVER_PATH/configmanager.h\
  $$SERVER_PATH/jsonpacket.h


install_files.files+=res/ res/libmysql.dll
install_files.path=$$OUT_PWD/debug/
INSTALLS+=install_files


DISTFILES += \
    res/config.json
#LIBS+=-lmysqlclient
#LIBS+=-lmysql

#win32:CONFIG(release, debug|release): LIBS += -LC:/mysql/lib64/vs14/ -lmysqlcppconn-static
#else:win32:CONFIG(debug, debug|release): LIBS += -LC:/mysql/lib64/vs14/ -lmysqlcppconn-static
#else:unix: LIBS += -LC:/mysql/lib64/vs14/ -lmysqlcppconn-static

#INCLUDEPATH += C:/mysql/lib64/vs14
#DEPENDPATH += C:/mysql/lib64/vs14


win32:CONFIG(release, debug|release): LIBS += -LC:/Qt/Qt5.9.6/5.9.6/msvc2017_64/lib/ -llibmysql
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/Qt/Qt5.9.6/5.9.6/msvc2017_64/lib/ -llibmysql

INCLUDEPATH += C:/Qt/Qt5.9.6/5.9.6/msvc2017_64/include
DEPENDPATH += C:/Qt/Qt5.9.6/5.9.6/msvc2017_64/include
