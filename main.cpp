
#include <QApplication>
#include <QQmlEngine>
#include <QVariant>

#include "qtquick2controlsapplicationviewer.h"

//#include "MyPlugins/MyEventModel.h"
#include "MyPlugins/EventUtils.h"
//#include "MyPlugins/MyEvent.h"
//#include "MyPlugins/MyCollection.h"
//#include "MyPlugins/MyTestEvent.h"

#include "MyPlugins/MyWeekModel.h"

//#include "CalendarPlugin/Private/qquickabstractstyle_p.h"
//#include "CalendarPlugin/Private/qquickcalendarmodel_p.h"
//#include "CalendarPlugin/Private/qquickcontrolsettings_p.h"
//#include "CalendarPlugin/Private/qquickcontrolsprivate_p.h"
//#include "CalendarPlugin/Private/qquickrangeddate_p.h"

#include "Google/MainWindow.h"
#include "Google/GoogleSettings.h"
#include "Google/GoogleManager.h"

#include "QDeclarativeOrganizer/qdeclarativeorganizercollection_p.h"
#include "QDeclarativeOrganizer/qdeclarativeorganizeritem_p.h"
#include "QDeclarativeOrganizer/qdeclarativeorganizeritemdetail_p.h"
#include "QDeclarativeOrganizer/qdeclarativeorganizeritemfetchhint_p.h"
#include "QDeclarativeOrganizer/qdeclarativeorganizeritemfilter_p.h"
#include "QDeclarativeOrganizer/qdeclarativeorganizeritemsortorder_p.h"
#include "QDeclarativeOrganizer/qdeclarativeorganizermodel_p.h"
#include "QDeclarativeOrganizer/qdeclarativeorganizerrecurrencerule_p.h"

void registerQtOrganizerTypes(const char *uri)
{
  Q_ASSERT(uri == QStringLiteral("MyCalendar.QtOrganizer"));
  // @uri MyCalendar.QtOrganizer

  int major = 5;
  int minor = 0;
  qmlRegisterType<QDeclarativeOrganizerModel>(uri, major, minor, "OrganizerModel");
  qmlRegisterType<QDeclarativeOrganizerRecurrenceRule>(uri, major, minor, "RecurrenceRule");
  qmlRegisterType<QDeclarativeOrganizerItemFetchHint>(uri, major, minor, "FetchHint");
  qmlRegisterType<QDeclarativeOrganizerItemSortOrder>(uri, major, minor, "SortOrder");
  qmlRegisterType<QDeclarativeOrganizerCollection>(uri, major, minor, "Collection");

  //items
  qmlRegisterType<QDeclarativeOrganizerItem>(uri, major, minor, "OrganizerItem");
  qmlRegisterType<QDeclarativeOrganizerEvent>(uri, major, minor, "Event");
  qmlRegisterType<QDeclarativeOrganizerEventOccurrence>(uri, major, minor, "EventOccurrence");
  qmlRegisterType<QDeclarativeOrganizerJournal>(uri, major, minor, "Journal");
  qmlRegisterType<QDeclarativeOrganizerNote>(uri, major, minor, "Note");
  qmlRegisterType<QDeclarativeOrganizerTodo>(uri, major, minor, "Todo");
  qmlRegisterType<QDeclarativeOrganizerTodoOccurrence>(uri, major, minor, "TodoOccurrence");

  qRegisterMetaType< QList<QDeclarativeOrganizerEvent*> >();
  qRegisterMetaType< QList<QDeclarativeOrganizerCollection*> >();

  //details
  qmlRegisterType<QDeclarativeOrganizerItemDetail>(uri, major, minor, "Detail");
  qmlRegisterType<QDeclarativeOrganizerEventTime>(uri, major, minor, "EventTime");
  qmlRegisterType<QDeclarativeOrganizerItemComment>(uri, major, minor, "Comment");
  qmlRegisterType<QDeclarativeOrganizerItemDescription>(uri, major, minor, "Description");
  qmlRegisterType<QDeclarativeOrganizerItemDisplayLabel>(uri, major, minor, "DisplayLabel");
  qmlRegisterType<QDeclarativeOrganizerItemGuid>(uri, major, minor, "Guid");
  qmlRegisterType<QDeclarativeOrganizerItemLocation>(uri, major, minor, "Location");
  qmlRegisterType<QDeclarativeOrganizerItemParent>(uri, major, minor, "Parent");
  qmlRegisterType<QDeclarativeOrganizerItemPriority>(uri, major, minor, "Priority");
  qmlRegisterType<QDeclarativeOrganizerItemRecurrence>(uri, major, minor, "Recurrence");
  qmlRegisterType<QDeclarativeOrganizerItemTag>(uri, major, minor, "Tag");
  qmlRegisterType<QDeclarativeOrganizerItemTimestamp>(uri, major, minor, "Timestamp");
  qmlRegisterType<QDeclarativeOrganizerItemType>(uri, major, minor, "Type");
  qmlRegisterType<QDeclarativeOrganizerJournalTime>(uri, major, minor, "JournalTime");
  qmlRegisterType<QDeclarativeOrganizerTodoProgress>(uri, major, minor, "TodoProgress");
  qmlRegisterType<QDeclarativeOrganizerTodoTime>(uri, major, minor, "TodoTime");
  qmlRegisterType<QDeclarativeOrganizerItemExtendedDetail>(uri, major, minor, "ExtendedDetail");
  qmlRegisterType<QDeclarativeOrganizerEventAttendee>(uri, major, minor, "EventAttendee");
  qmlRegisterType<QDeclarativeOrganizerEventRsvp>(uri, major, minor, "EventRsvp");
  qmlRegisterType<QDeclarativeOrganizerItemClassification>(uri, major, minor, "Classification");
  qmlRegisterType<QDeclarativeOrganizerItemVersion>(uri, major, minor, "Version");

  qmlRegisterType<QDeclarativeOrganizerItemReminder>(uri, major, minor, "Reminder");
  qmlRegisterType<QDeclarativeOrganizerItemAudibleReminder>(uri, major, minor, "AudibleReminder");
  qmlRegisterType<QDeclarativeOrganizerItemEmailReminder>(uri, major, minor, "EmailReminder");
  qmlRegisterType<QDeclarativeOrganizerItemVisualReminder>(uri, major, minor, "VisualReminder");

  //filters
  qmlRegisterType<QDeclarativeOrganizerItemFilter>(uri, major, minor, "Filter");
  qmlRegisterType<QDeclarativeOrganizerItemCollectionFilter>(uri, major, minor, "CollectionFilter");
  qmlRegisterType<QDeclarativeOrganizerItemDetailFilter>(uri, major, minor, "DetailFilter");
  qmlRegisterType<QDeclarativeOrganizerItemDetailFieldFilter>(uri, major, minor, "DetailFieldFilter");
  qmlRegisterType<QDeclarativeOrganizerItemDetailRangeFilter>(uri, major, minor, "DetailRangeFilter");
  qmlRegisterType<QDeclarativeOrganizerItemIdFilter>(uri, major, minor, "IdFilter");
  qmlRegisterType<QDeclarativeOrganizerItemIntersectionFilter>(uri, major, minor, "IntersectionFilter");
  qmlRegisterType<QDeclarativeOrganizerItemUnionFilter>(uri, major, minor, "UnionFilter");
  qmlRegisterType<QDeclarativeOrganizerItemInvalidFilter>(uri, major, minor, "InvalidFilter");
  qmlRegisterType<QDeclarativeOrganizerItemCompoundFilter>();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setFont(QFont("Microsoft YaHei"));

//    const char* private_uri = "MyCalendar.Controls.Private";
//    // @uri MyCalendar.Controls.Private
//    qmlRegisterType<QQuickAbstractStyle>(private_uri, 1, 0, "AbstractStyle");
//    qmlRegisterType<QQuickRangedDate>(private_uri, 1, 0, "RangedDate");
//    qmlRegisterType<QQuickCalendarModel>(private_uri, 1, 0, "CalendarModel");

    const char* uri = "MyCalendar2.Events";
    // @uri MyCalendar2.Events
    qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
//    qmlRegisterType<MyEventModel>(uri, 1, 0, "MyEventModel");
//    qmlRegisterType<MyEvent>(uri, 1, 0, "MyEvent");
//    qmlRegisterType<MyCollection>(uri, 1, 0, "MyCollection");
    qRegisterMetaType< QList<QObject*> >();
//    qRegisterMetaType< QList<MyEvent*> >();
//    qRegisterMetaType< QList<MyCollection*> >();

    uri = "MyCalendar.Weeks";
    // @uri MyCalendar.Weeks
    qmlRegisterType<MyWeekModel>(uri, 1, 0, "MyWeekModel");

    uri = "MyCalendar.Sync.Google";
    // @uri MyCalendar.Sync.Google
    qmlRegisterType<GoogleSettings>(uri, 1, 0, "GoogleSettings");

    registerQtOrganizerTypes("MyCalendar.QtOrganizer");

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QingfengCalendar/main.qml"));

    viewer.show();

    return app.exec();
}

