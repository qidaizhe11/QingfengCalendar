
QT       += core sql

QT       -= gui

TARGET = Database
TEMPLATE = lib

DEFINES += Database_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    CalendarContract.h \
    CalendarInstancesHelper.h \
    Database.h \
    Event.h \
    EventRecurrence.h \
    RecurrenceProcessor.h

SOURCES += \
    CalendarContract.cpp \
    CalendarInstancesHelper.cpp \
    Database.cpp \
    Event.cpp \
    EventRecurrence.cpp \
    RecurrenceProcessor.cpp

