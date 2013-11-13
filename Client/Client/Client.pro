TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    spriteinfo.cpp \
    resourcemanager.cpp \
    puck.cpp \
    player.cpp \
    paddle.cpp \
    gfxobject.cpp \
    game.cpp \
    client.cpp

HEADERS += \
    unix_platform.h \
    spriteinfo.h \
    resourcemanager.h \
    puck.h \
    player.h \
    paddle.h \
    main.h \
    gfxobject.h \
    game.h \
    client.h


QMAKE_CXXFLAGS += -std=c++11

unix {
    INCLUDEPATH += /usr/local/include/hge-unix/include \
                   /usr/include/SDL
    LIBS += /usr/local/include/hge-unix/src/core/libhge.so \
            /usr/local/include/hge-unix/src/core/libhge_helpers.so \
            -L/usr/local/lib \
            -lboost_system
}

win32 {

}
