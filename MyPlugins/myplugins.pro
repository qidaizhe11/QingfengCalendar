TEMPLATE = lib
TARGET = MyPlugins
QT += qml quick core gui widgets organizer versit versitorganizer
CONFIG += qt plugin

DESTDIR = ../build
OBJECTS_DIR = $$DESTDIR
MOC_DIR = $$DESTDIR

TARGET = $$qtLibraryTarget($$TARGET)
uri = MyCalendar.Utils.Events

# Input
SOURCES += \
    myplugins_plugin.cpp \
    eventutils.cpp \
    MyEventList.cpp \
    MyEvent.cpp \
    ../QDeclarativeOrganizer/qdeclarativeorganizercollection.cpp \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritem.cpp \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritemdetail.cpp \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritemfetchhint.cpp \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritemfilter.cpp \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritemsortorder.cpp \
    ../QDeclarativeOrganizer/qdeclarativeorganizermodel.cpp \
    ../QDeclarativeOrganizer/qdeclarativeorganizerrecurrencerule.cpp

HEADERS += \
    myplugins_plugin.h \
    eventutils.h \
    MyEventList.h \
    MyEvent.h \
    ../QDeclarativeOrganizer/qdeclarativeorganizercollection_p.h \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritem_p.h \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritemdetail_p.h \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritemfetchhint_p.h \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritemfilter_p.h \
    ../QDeclarativeOrganizer/qdeclarativeorganizeritemsortorder_p.h \
    ../QDeclarativeOrganizer/qdeclarativeorganizermodel_p.h \
    ../QDeclarativeOrganizer/qdeclarativeorganizerrecurrencerule_p.h

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
