TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += puck.h \
    paddle.h \
    gfxobject.h \
    player.h \
    main.h \
    resourcemanager.h \
    unix_platform.h \
    textureinfo.h \
    game.h

SOURCES += main.cpp \
           puck.cpp \
    paddle.cpp \
    gfxobject.cpp \
    player.cpp \
    resourcemanager.cpp \
    textureinfo.cpp \
    game.cpp

QMAKE_CXXFLAGS += -std=c++11

unix {
    INCLUDEPATH += /usr/local/include/hge-unix/include \
                   /usr/include/SDL
    LIBS += /usr/local/include/hge-unix/src/core/libhge.so \
            /usr/local/include/hge-unix/src/core/libhge_helpers.so
}

win32 {

}

