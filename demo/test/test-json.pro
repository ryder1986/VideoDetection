HEADERS += \
    test.h

SOURCES += \
    test.cpp
SRC_PATH=../server
INCLUDEPATH +=$$SRC_PATH
INCLUDEPATH +=$$SRC_PATH/cppjson/include
CONFIG +=c++11
HEADERS +=$$SRC_PATH/cppjson/include/json/reader.h cppjson/include/json/writer.h cppjson/include/json/value.h

SOURCES += $$SRC_PATH/tool.cpp

SOURCES += $$SRC_PATH/cppjson/json_reader.cpp $$SRC_PATH/cppjson/json_writer.cpp  $$SRC_PATH/cppjson/json_value.cpp



HEADERS +=$$SRC_PATH/jsonpacket.h $$SRC_PATH/configmanager.h
SOURCES +=$$SRC_PATH/configmanager.cpp
RS.files+=$$PWD/../server/res/
RS.path=$$OUT_PWD/
INSTALLS +=RS
