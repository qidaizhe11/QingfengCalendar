#-------------------------------------------------
#
# Project created by QtCreator 2015-07-15T14:11:51
#
#-------------------------------------------------

QT       -= gui
QT += network
CONFIG += c++11

TARGET = Network
TEMPLATE = lib

DEFINES += NETWORK_LIBRARY

#DEFINES += YAOLOG_EXPORTS

#INCLUDEPATH += ../3rdParty

win32:LIBS += -lws2_32

SOURCES += Network.cpp \
    Lock.cpp \
    util.cpp \
    BaseSocket.cpp \
#    ../3rdParty/yaolog.cpp \
    EventDispatch.cpp \
    UtilPdu.cpp \
    LoginModule.cpp

HEADERS += Network.h\
        network_global.h \
    ostype.h \
    Lock.h \
    util.h \
    GlobalConfig.h \
    ImCore.h \
    GlobalDefine.h \
    netlib.h \
    EventDispatch.h \
    BaseSocket.h \
#    ../3rdParty/yaolog.h \
    UtilPdu.h \
    LoginModule.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
