#include "QDeclarativeOrganizer_plugin.h"

#include "qdeclarativeorganizercollection_p.h"
#include "qdeclarativeorganizeritem_p.h"
#include "qdeclarativeorganizeritemdetail_p.h"
#include "qdeclarativeorganizeritemfetchhint_p.h"
#include "qdeclarativeorganizeritemfilter_p.h"
#include "qdeclarativeorganizeritemsortorder_p.h"
#include "qdeclarativeorganizermodel_p.h"
#include "qdeclarativeorganizerrecurrencerule_p.h"

#include <qqml.h>

void MyPluginsPlugin::registerTypes(const char *uri)
{
  // @uri MyCalendar.MyQtOrganizer
  int major = 1;
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

  qRegisterMetaType<QDeclarativeOrganizerModel*>();

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


