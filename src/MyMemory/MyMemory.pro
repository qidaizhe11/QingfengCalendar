#-------------------------------------------------
#
# Project created by QtCreator 2015-06-26T08:48:56
#
#-------------------------------------------------

QT       += core gui organizer

TARGET = qtorganizer_mymemory
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/organizer

SOURCES += qorganizeritemmemorybackend.cpp

HEADERS += qorganizeritemmemorybackend.h
DISTFILES += \
    mymemory.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
