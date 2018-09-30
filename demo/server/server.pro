
CONFIG+=WITH_CUDA
WITH_CUDA{
    DEFINES+=WITH_CUDA
    LIBS+=-L/lib -ldarknet
    HEADERS+= fvdprocessor.h \
    pvdprocessor.h    mvdprocessor.h
    SOURCES+= fvdprocessor.cpp \
    pvdprocessor.cpp
    INCLUDEPATH+=/usr/include/python2.7
    LIBS+=-lpython2.7
}else{
    message("no cuda")
    # LIBS+=-lopencv_imgcodecs
}
CONFIG +=console
CONFIG -= app_bundle
CONFIG -= qt
TEMPLATE = app

install_files1.files+=res/
install_files1.path=$$OUT_PWD/
INSTALLS+=install_files1

INCLUDEPATH +=cppjson/include
INCLUDEPATH+=/usr/local/opencv-without-cuda/include
LIBS+=-L/usr/local/opencv-without-cuda/lib

HEADERS +=\
    cppjson/include/json/reader.h\
    cppjson/include/json/writer.h\
    cppjson/include/json/value.h \
    ffmpegvideocapture.h
SOURCES +=\
    cppjson/json_reader.cpp \
    cppjson/json_writer.cpp \
    cppjson/json_value.cpp

HEADERS +=configmanager.h  app.h \
   server.h \
    camera.h videosource.h detectregion.h \
    c4common.h c4processor.h videoprocessor.h rtsp.h jsonpacket.h app_data.h \
    camera_data.h \
    detectregion_data.h\
    tool.h socket.h
SOURCES += main.cpp  configmanager.cpp app.cpp   \
server.cpp camera.cpp videosource.cpp  detectregion.cpp \
c4common.cpp c4processor.cpp videoprocessor.cpp tool.cpp socket.cpp

#FFMPEG_PATH=/root/source/ffmpeg2814/__install
#CV_PATH=/root/source/opencv2491/__install
#LIBS+=-lavcodec  -lavutil -lswresample
#LIBS+= -L/root/source/opencv-3.2.0/build/__install/lib -lopencv_core -lopencv_highgui -lopencv_objdetect \
#-lopencv_imgproc -lopencv_ml -lopencv_highgui -lopencv_video -lopencv_videostab  -lopencv_videoio -lpthread  -lX11
#LIBS+=-lavformat
#INCLUDEPATH+=$$FFMPEG_PATH/include
#INCLUDEPATH+=$$CV_PATH/include
##LIBS+= -L/root/source/opencv-3.2.0/build/__install/lib
#LIBS += -L$$PWD/ -ldarknet
#PRE_TARGETDEPS += $$PWD/libdarknet.so
LIBS+=-lopencv_core -lopencv_highgui -lopencv_objdetect \
-lopencv_imgproc -lopencv_ml -lopencv_highgui\
-lopencv_video -lopencv_videostab  -lpthread -lopencv_videoio -lopencv_imgcodecs
LIBS+=-lavformat -lavcodec  -lavutil -lswresample
QMAKE_LFLAGS+=-rdynamic
QMAKE_CXXFLAGS +=-g -w
LIBS+=-lmysqlclient
include(common.pri)
