
TEMPLATE = lib
TARGET = Google
QT += qml quick organizer
CONFIG += qt plugin

DESTDIR = ../../imports/MyCalendar/Google
#OBJECTS_DIR = $$DESTDIR/.obj
#MOC_DIR = $$DESTDIR/.moc

TARGET = $$qtLibraryTarget($$TARGET)
uri = MyCalendar.Google

INCLUDEPATH += ../../include/
INCLUDEPATH += ../QDeclarativeOrganizer
INCLUDEPATH += ../Base

include(../QDeclarativeOrganizer/QDeclarativeOrganizer.pri)
include(../Base/Base.pri)

HEADERS += \
    GoogleManager.h \
    GooglePlugin.h \
    GoogleSettings.h

SOURCES += \
    GoogleManager.cpp \
    GooglePlugin.cpp \
    GoogleSettings.cpp


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

copyfile = $$PWD/qmldir
copydest = $$DESTDIR

# On Windows, use backslashes as directory separators
equals(QMAKE_HOST.os, Windows) {
    copyfile ~= s,/,\\,g
    copydest ~= s,/,\\,g
}

# Copy the qmldir file to the same folder as the plugin binary
QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$copyfile) $$quote($$copydest) $$escape_expand(\\n\\t)
