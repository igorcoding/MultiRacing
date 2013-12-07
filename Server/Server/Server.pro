TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    server.cpp \
    logic.cpp \
    player.cpp \
    puck.cpp \
    gameobject.cpp \
    vector2d.cpp

OTHER_FILES += \
    protocol.txt

HEADERS += \
    server.h \
    logic.h \
    player.h \
    puck.h \
    gameobject.h \
    vector2d.h

QMAKE_CXXFLAGS += -pthread -std=c++1y
QMAKE_LFLAGS += -pthread -std=c++1y

unix {
    LIBS += -L/usr/local/lib \
            -lboost_system

}

win32 {
    INCLUDEPATH += C:/Projects/Qt/boost_1_54_0
    LIBS += -LC:/Projects/Qt/boost_1_54_0/stage/lib/ \
        -lws2_32
    LIBS += C:/Projects/Qt/boost_1_54_0/stage/lib/libboost_system-mgw48-mt-sd-1_54.a
}

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}
