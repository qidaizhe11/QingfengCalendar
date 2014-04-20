# Add more folders to ship with the application, here
folder_01.source = qml/QingfengCalendar
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += core gui widgets organizer versit webkitwidgets

#LIBS        += /usr/lib/libical.so.1

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
#    Database/CalendarContract.cpp \
#    Database/CalendarInstancesHelper.cpp \
#    Database/Database.cpp \
#    Database/Event.cpp \
#    Database/EventRecurrence.cpp \
#    Database/RecurrenceProcessor.cpp \
    MyPlugins/MyEventModel.cpp \
    MyPlugins/MyTestEvent.cpp \
    MyPlugins/EventUtils.cpp \
    Google/OAuth2.cpp \
    Google/LoginDialog.cpp \
    Google/MainWindow.cpp \
    Google/GoogleSettings.cpp \
    MyPlugins/MyCollection.cpp \
    Google/GoogleManager.cpp \
    MyPlugins/MyWeekModel.cpp \
    MyPlugins/MyRecurrenceRule.cpp \
    MyPlugins/MyItem.cpp \
    MyPlugins/MyItemDetail.cpp \


# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
#    Database/CalendarContract.h \
#    Database/CalendarInstancesHelper.h \
#    Database/Database.h \
#    Database/Event.h \
#    Database/EventRecurrence.h \
#    Database/RecurrenceProcessor.h \
    MyPlugins/MyEventModel.h \
    MyPlugins/MyTestEvent.h \
    MyPlugins/EventUtils.h \
    Google/OAuth2.h \
    Google/LoginDialog.h \
    Google/MainWindow.h \
    Google/GoogleSettings.h \
    MyPlugins/MyCollection.h \
    Google/GoogleManager.h \
    MyPlugins/MyWeekModel.h \
    MyPlugins/MyRecurrenceRule.h \
    MyPlugins/MyItem.h \
    MyPlugins/MyItemDetail.h \

include(QDeclarativeOrganizer/QDeclarativeOrganizer.pri)

include(versitorganizer/versitorganizer.pri)

include(CalendarPlugin/Private/QtQuickControls.pri)

RESOURCES += \
    resources/Calendar.qrc

OTHER_FILES += \
    README.md \
