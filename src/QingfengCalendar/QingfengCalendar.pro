
TEMPLATE = app

QT += qml quick widgets organizer versit versitorganizer webkitwidgets

DESTDIR = ../../bin
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.rcc

SOURCES += main.cpp

# Installation path
# target.path =

include(deployment.pri)

RESOURCES += \
    resources/Calendar.qrc \
    qml/qml.qrc
