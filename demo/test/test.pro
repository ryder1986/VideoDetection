
SRC_PATH=../..
INCLUDEPATH +=$$SRC_PATH

CONFIG +=c++11
DEFINES+=IS_UNIX



SRC_PATH =../server
HEADERS  += $$SRC_PATH/tool.h
IHEADERS  += $$SRC_PATH/jsonpacket.h
INCLUDEPATH +=$$SRC_PATH/cppjson/include
INCLUDEPATH +=$$SRC_PATH/


HEADERS +=$$SRC_PATH/cppjson/include/json/reader.h cppjson/include/json/writer.h cppjson/include/json/value.h

SOURCES += $$SRC_PATH/tool.cpp \
    main.cpp
SOURCES += $$SRC_PATH/cppjson/json_reader.cpp $$SRC_PATH/cppjson/json_writer.cpp  $$SRC_PATH/cppjson/json_value.cpp

#LIBS+=-L/root/build/2/build-libjson-Unnamed-Debug -ljson
#LIBS+=-L/root/build/2/build-libsocket-Unnamed-Debug -lsocket
#LIBS+=-L/root/build/2/build-libtool-Unnamed-Debug -ltool


SOURCES += $$SRC_PATH/videosource.cpp

LIBS+= -lopencv_core -lopencv_highgui -lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_highgui -lopencv_video   -lpthread



INCLUDEPATH+=/usr/include/python2.7
LIBS+=  -lpython2.7
QMAKE_CXXFLAGS+=-w
