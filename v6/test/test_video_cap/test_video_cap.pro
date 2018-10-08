TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
include(../common.pri)
LIBS+=-lopencv_core -lopencv_highgui -lopencv_objdetect \
-lopencv_imgproc -lopencv_ml -lopencv_highgui\
-lopencv_video -lopencv_videostab  -lpthread -lopencv_videoio -lopencv_imgcodecs
LIBS+=-lavformat -lavcodec  -lavutil -lswresample -ldhnetsdk
HEADERS+=dahuavideocapture.h
