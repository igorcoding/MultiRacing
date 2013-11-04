TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++1y

SOURCES += main.cpp \
    server.cpp \
    logic.cpp

INCLUDEPATH += C:/Projects/Qt/boost_1_54_0
LIBS += -LC:/Projects/Qt/boost_1_54_0/stage/lib/ \
    -lws2_32
LIBS += C:/Projects/Qt/boost_1_54_0/stage/lib/libboost_system-mgw48-mt-sd-1_54.a

OTHER_FILES += \
    protocol.txt

HEADERS += \
    server.h \
    logic.h
