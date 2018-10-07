unix{
    DEFINES+=IS_UNIX
    LIBS+=-lpthread
}else{
    DEFINES+=IS_WIN
}
INCLUDEPATH+=cppjson/include


TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    app.cpp configmanager.cpp camera.cpp tool.cpp server.cpp\
 socket.cpp videosource.cpp detectregion.cpp videoprocessor.cpp \
    mvdprocessor.cpp \
    dummyprocessor.cpp jsonpacket.cpp

INCLUDEPATH+=/root/sources/opencv-3.2.0/build/__install/include
LIBS+=-L/root/sources/opencv-3.2.0/build/__install/lib

HEADERS +=\
    cppjson/include/json/reader.h\
    cppjson/include/json/writer.h\
    cppjson/include/json/value.h \
    app.h configmanager.h camera.h tool.h server.h socket.h \
videosource.h detectregion.h videoprocessor.h \
    mvdprocessor.h \
    dummyprocessor.h
SOURCES +=\
    cppjson/json_reader.cpp \
    cppjson/json_writer.cpp \
    cppjson/json_value.cpp
LIBS+=-lopencv_core
LIBS+=-lopencv_video
LIBS+=-lopencv_imgcodecs
LIBS+=-lopencv_highgui
LIBS+=-lopencv_ml
LIBS+=-lopencv_videoio
LIBS+=-lopencv_imgproc

install_files.files+=res/
install_files.path=$$OUT_PWD/
INSTALLS+=install_files
