# Add more folders to ship with the application, here
folder_01.source = qml
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += core gui widgets organizer versit versitorganizer webkitwidgets

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp


# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

#include(QDeclarativeOrganizer/QDeclarativeOrganizer.pri)

#include(versitorganizer/versitorganizer.pri)

#include(CalendarPlugin/Private/QtQuickControls.pri)

RESOURCES += \
    resources/Calendar.qrc
