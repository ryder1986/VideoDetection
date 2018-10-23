SRC_PATH =../server
CONFIG+=WITH_CUDA
CONFIG+=WITH_OPENGL
QT += widgets network axserver
TARGET = client
TEMPLATE = app
SOURCES += main.cpp\
    mainwindow.cpp \
    playerwidget.cpp

HEADERS  += mainwindow.h \
    playerwidget.h
FORMS    += mainwindow.ui

HEADERS  += $$SRC_PATH/tool.h
INCLUDEPATH +=$$SRC_PATH/cppjson/include
INCLUDEPATH +=$$SRC_PATH/

HEADERS +=$$SRC_PATH/cppjson/include/json/reader.h \
$$SRC_PATH/cppjson/include/json/writer.h \
$$SRC_PATH/cppjson/include/json/value.h
SOURCES += $$SRC_PATH/tool.cpp
SOURCES += $$SRC_PATH/cppjson/json_reader.cpp $$SRC_PATH/cppjson/json_writer.cpp  $$SRC_PATH/cppjson/json_value.cpp
SOURCES += $$SRC_PATH/videosource.cpp
unix{
CVPATH=/root/source/opencv-3.2.0/build/__install
AVPATH=/root/source/ffmpeg-3.4.4/__install
LIBS+=-L$$CVPATH/lib
LIBS+=-L$$AVPATH/lib
INCLUDEPATH+=$$CVPATH/include
INCLUDEPATH+=$$AVPATH/include
    QMAKE_LFLAGS+=-Wl,--rpath=$$CVPATH/lib
    QMAKE_LFLAGS+=-Wl,--rpath=$$AVPATH/lib
INCLUDEPATH+=/root/source/opencv-3.2.0/build/__install/include
LIBS+=-L/root/source/ffmpeg-3.4.4/__install/lib
LIBS+=-L/root/source/opencv-3.2.0/build3/__install/lib
#LIBS+=-L/root/source/opencv-3.2.0/build3/lib
QMAKE_LFLAGS+=-Wl,--rpath=/root/source/opencv-3.2.0/build/__install/lib
LIBS+=  -lopencv_core -lopencv_highgui -lopencv_objdetect \
-lopencv_imgproc -lopencv_ml -lopencv_highgui -lopencv_video -lopencv_videostab -lpthread  -lX11 -lopencv_imgcodecs  -lopencv_videoio #
LIBS+=-lavformat -lavcodec  -lavutil -lswresample
}else{
message(build win32)
INCLUDEPATH+=C:\opencv3.2\opencv\build\include
debug{
LIBS+=-LC:\opencv3.2\opencv\build\x64\vc14\lib -lopencv_world320d
}
release{
LIBS+=-LC:\opencv3.2\opencv\build\x64\vc14\lib -lopencv_world320
}

}
include($$SRC_PATH/common.pri)
