
#include <QApplication>
#include <QQmlEngine>
#include "qtquick2controlsapplicationviewer.h"
#include <QQuickWindow>
#include <QQuickView>
#include <QVariant>

#include "MyPlugins/MyEventModel.h"
#include "MyPlugins/EventUtils.h"
#include "MyPlugins/MyEvent.h"
#include "MyPlugins/MyTestEvent.h"

#include "CalendarPlugin/Private/qquickabstractstyle_p.h"
#include "CalendarPlugin/Private/qquickcalendarmodel_p.h"
#include "CalendarPlugin/Private/qquickcontrolsettings_p.h"
#include "CalendarPlugin/Private/qquickcontrolsprivate_p.h"
//#include "CalendarPlugin/Private/qquickpadding_p.h"
#include "CalendarPlugin/Private/qquickrangeddate_p.h"


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

//    MyTestEvent event;
//    event.setStartDateTime(QDateTime::currentDateTime());
//    QDateTime time = event.startDateTime();

    MyEvent my_event;
    my_event.setDisplayLabel("hello");
    my_event.setStartDateTime(QDateTime::currentDateTime());

    QDateTime temp = my_event.startDateTime();
//    qDebug() << temp;

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QingfengCalendar/mainCalendarExample.qml"));
//    viewer.setMainQmlFile(QStringLiteral("qml/QingfengCalendar/EventEditWindow.qml"));

//    QQuickView* viewer = new QQuickView;
//    viewer->setResizeMode(QQuickView::SizeRootObjectToView);
//    viewer->setMinimumSize(QSize(800, 560));
//    viewer->setSource(QUrl::fromLocalFile("qml/QingfengCalendar/mainCalendarExample.qml"));

//    QObject* style_object = viewer.rootObject()->findChild<QObject*>("calendar_style");
//    QMetaObject::invokeMethod(viewer.rootObject(), "refreshData",
//                              Q_ARG(QVariant, event_list->events()));
//    QMetaObject::invokeMethod(style_object, "refreshData",
//                              Q_ARG(QVariant, event_list->events()));
//    QMetaObject::invokeMethod(viewer.rootObject(), "refreshData",
//                              Q_ARG(QVariant, events));


    viewer.show();

    return app.exec();
}

