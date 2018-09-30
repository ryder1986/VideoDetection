HEADERS += \
    player.h \
    ../server/videosource.h \
    ../server/tool.h

SOURCES += \
    player.cpp \
    test-vaapi.cpp \
    ../server/videosource.cpp \
    ../server/tool.cpp
SERVR_PATH=../server
QMAKE_CXXFLAGS+=-w
INCLUDEPATH +=$$SERVR_PATH
CONFIG+=c++11
LIBS+=-lopencv_core -lopencv_highgui -lopencv_objdetect \
-lopencv_imgproc -lopencv_ml -lopencv_highgui -lopencv_video -lopencv_videostab  -lpthread# -lopencv_videoio
LIBS+=-lavformat -lavcodec  -lavutil -lswresample
INCLUDEPATH+=sdk/include



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/sdk/lib/release/ -lNet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/sdk/lib/debug/ -lNet
else:unix: LIBS += -L$$PWD/sdk/lib/ -lNet

INCLUDEPATH += $$PWD/sdk/include
DEPENDPATH += $$PWD/sdk/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/sdk/lib/release/libNet.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/sdk/lib/debug/libNet.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/sdk/lib/release/Net.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/sdk/lib/debug/Net.lib
else:unix: PRE_TARGETDEPS += $$PWD/sdk/lib/libNet.a
