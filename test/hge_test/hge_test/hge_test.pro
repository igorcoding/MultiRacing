TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

QMAKE_CXXFLAGS += -std=c++11

unix {
    INCLUDEPATH += /usr/local/include/hge-unix/include \
                   /usr/include/SDL
    LIBS += /usr/local/include/hge-unix/src/core/libhge.so \
            /usr/local/include/hge-unix/src/core/libhge_helpers.so
}

win32 {

}
