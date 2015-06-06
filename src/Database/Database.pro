
QT       += core widgets sql

QT       -= gui

TARGET = Database
TEMPLATE = lib

DEFINES += Database_LIBRARY

INCLUDEPATH += ../../include

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    CalendarContract.h \
    Database.h \
    Event.h \
    DatabaseModule.h

SOURCES += \
    CalendarContract.cpp \
    Database.cpp \
    Event.cpp \
    DatabaseModule.cpp

