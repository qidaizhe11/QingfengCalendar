# Add more folders to ship with the application, here
#folder_01.source = qml
#folder_01.target = ../..
#DEPLOYMENTFOLDERS = folder_01

TEMPLATE = app

#folder_01.source = qml
#folder_01.target = ../..
#DEPLOYMENTFOLDERS = folder_01

QT += qml quick widgets organizer versit versitorganizer webkitwidgets

DESTDIR = ../../bin
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.rcc

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
#include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
#qtcAddDeployment()

include(deployment.pri)

#include(QDeclarativeOrganizer/QDeclarativeOrganizer.pri)

#include(versitorganizer/versitorganizer.pri)

#include(CalendarPlugin/Private/QtQuickControls.pri)

RESOURCES += \
    resources/Calendar.qrc \
    qml/qml.qrc
