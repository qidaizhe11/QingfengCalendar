
#include <QApplication>
#include <QQmlEngine>
//#include <QQuickWindow>
//#include <QQuickView>
#include <QVariant>

#include "qtquick2controlsapplicationviewer.h"

#include "MyPlugins/MyEventModel.h"
#include "MyPlugins/EventUtils.h"
#include "MyPlugins/MyEvent.h"
#include "MyPlugins/MyCollection.h"
#include "MyPlugins/MyTestEvent.h"

#include "CalendarPlugin/Private/qquickabstractstyle_p.h"
#include "CalendarPlugin/Private/qquickcalendarmodel_p.h"
#include "CalendarPlugin/Private/qquickcontrolsettings_p.h"
#include "CalendarPlugin/Private/qquickcontrolsprivate_p.h"
#include "CalendarPlugin/Private/qquickrangeddate_p.h"

#include "Google/MainWindow.h"
#include "Google/GoogleSettings.h"
#include "Google/GoogleManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const char* private_uri = "MyCalendar.Controls.Private";
    // @uri MyCalendar.Controls.Private
    qmlRegisterType<QQuickAbstractStyle>(private_uri, 1, 0, "AbstractStyle");
    qmlRegisterType<QQuickRangedDate>(private_uri, 1, 0, "RangedDate");
    qmlRegisterType<QQuickCalendarModel>(private_uri, 1, 0, "CalendarModel");

    const char* uri = "MyCalendar2.Events";
    // @uri MyCalendar2.Events
    qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
    qmlRegisterType<MyEventModel>(uri, 1, 0, "MyEventModel");
    qmlRegisterType<MyEvent>(uri, 1, 0, "MyEvent");
    qmlRegisterType<MyCollection>(uri, 1, 0, "MyCollection");
    qRegisterMetaType< QList<QObject*> >();
    qRegisterMetaType< QList<MyEvent*> >();
    qRegisterMetaType< QList<MyCollection*> >();

    uri = "MyCalendar.Sync.Google";
    // @uri MyCalendar.Sync.Google
    qmlRegisterType<GoogleSettings>(uri, 1, 0, "GoogleSettings");

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QingfengCalendar/main.qml"));
//    viewer.setMainQmlFile(QStringLiteral("qml/QingfengCalendar/GoogleOAuth.qml"));

    viewer.show();

//    GoogleManager google_manager;
//    google_manager.getCalendars();
//    google_manager.getEventsForCalendar("");

//    MainWindow window;
//    window.show();

    return app.exec();
}

