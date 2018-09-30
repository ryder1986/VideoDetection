#-------------------------------------------------
#
# Project created by QtCreator 2018-09-18T17:48:24
#
#-------------------------------------------------
CONFIG+=c++11
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testprocessor
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
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

#DEFINES +=WITH_CUDA

SERVER_PATH=../../server/
CLIENT_PATH=../../client/
INCLUDEPATH+=$$SERVER_PATH/
INCLUDEPATH+=$$CLIENT_PATH/
HEADERS+=$$SERVER_PATH/tool.h
SOURCES+=$$SERVER_PATH/tool.cpp

HEADERS+=$$SERVER_PATH/videoprocessor.h
SOURCES+=$$SERVER_PATH/videoprocessor.cpp

HEADERS+=$$SERVER_PATH/c4common.h
SOURCES+=$$SERVER_PATH/c4common.cpp

HEADERS+=$$SERVER_PATH/camera.h
SOURCES+=$$SERVER_PATH/camera.cpp

HEADERS+=$$SERVER_PATH/videosource.h
SOURCES+=$$SERVER_PATH/videosource.cpp


HEADERS+=$$SERVER_PATH/detectregion.h
SOURCES+=$$SERVER_PATH/detectregion.cpp

HEADERS+=$$CLIENT_PATH/playerwidget.h
SOURCES+=$$CLIENT_PATH/playerwidget.cpp


INCLUDEPATH +=$$SERVER_PATH/cppjson/include
HEADERS +=$$SERVER_PATH/cppjson/include/json/reader.h $$SERVER_PATH/cppjson/include/json/writer.h $$SERVER_PATH/cppjson/include/json/value.h
SOURCES += $$SERVER_PATH/cppjson/json_reader.cpp $$SERVER_PATH/cppjson/json_writer.cpp  $$SERVER_PATH/cppjson/json_value.cpp


LIBS+=-lopencv_core -lopencv_highgui -lopencv_objdetect \
-lopencv_imgproc -lopencv_ml -lopencv_highgui\
-lopencv_video -lopencv_videostab  -lpthread -lopencv_videoio -lopencv_imgcodecs
LIBS+=-lavformat -lavcodec  -lavutil -lswresample
install_files1.files+=$$SERVER_PATH/res/
install_files1.path=$$OUT_PWD/
INSTALLS+=install_files1
#CONFIG+=WITH_CUDA
WITH_CUDA{
    DARKNET=/root/darknet-master-FVD1
    DEFINES+=WITH_CUDA
    LIBS+=-L/lib -ldarknet
    HEADERS+= $$SERVER_PATH/fvdprocessor.h  $$SERVER_PATH/pvdprocessor.h    $$SERVER_PATH/mvdprocessor.h
    SOURCES+= $$SERVER_PATH/fvdprocessor.cpp \
    $$SERVER_PATH/pvdprocessor.cpp
    INCLUDEPATH+=/usr/include/python2.7
    LIBS+=-lpython2.7
    LIBS+=-L$$DARKNET/ -ldarknet
}else{
    #message("no cuda")
   # LIBS+=-lopencv_imgcodecs
}
