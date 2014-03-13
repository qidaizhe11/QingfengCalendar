
#include <QApplication>
#include <QQmlEngine>
//#include <QQuickWindow>
//#include <QQuickView>
#include <QVariant>

#include "qtquick2controlsapplicationviewer.h"

#include "MyPlugins/MyEventModel.h"
#include "MyPlugins/EventUtils.h"
#include "MyPlugins/MyEvent.h"
#include "MyPlugins/MyTestEvent.h"

#include "CalendarPlugin/Private/qquickabstractstyle_p.h"
#include "CalendarPlugin/Private/qquickcalendarmodel_p.h"
#include "CalendarPlugin/Private/qquickcontrolsettings_p.h"
#include "CalendarPlugin/Private/qquickcontrolsprivate_p.h"
#include "CalendarPlugin/Private/qquickrangeddate_p.h"

#include "Google/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const char* private_uri = "MyCalendar2.Controls.Private";
    // @uri MyCalendar.Controls.Private
    qmlRegisterType<QQuickAbstractStyle>(private_uri, 1, 0, "AbstractStyle");
    qmlRegisterType<QQuickRangedDate>(private_uri, 1, 0, "RangedDate");
    qmlRegisterType<QQuickCalendarModel>(private_uri, 1, 0, "CalendarModel");

    const char* uri = "MyCalendar2.Utils.Events";
    // @uri MyCalendar2.Utils.Events
    qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
    qmlRegisterType<MyEventModel>(uri, 1, 0, "MyEventModel");
    qmlRegisterType<MyEvent>(uri, 1, 0, "MyEvent");
    qRegisterMetaType< QList<QObject*> >();
    qRegisterMetaType< QList<MyEvent*> >();

//    QtQuick2ControlsApplicationViewer viewer;
//    viewer.setMainQmlFile(QStringLiteral("qml/QingfengCalendar/mainCalendarExample.qml"));

//    viewer.show();

    MainWindow window;
    window.show();

    return app.exec();
}

