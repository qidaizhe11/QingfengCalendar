#include "myplugins_plugin.h"
#include "EventUtils.h"
#include "MyQQuickCalendarModel.h"

#include <qqml.h>

void MyPluginsPlugin::registerTypes(const char *uri)
{
  // @uri MyCalendar.Utils
  qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
  qmlRegisterType<MyQQuickCalendarModel>(uri, 1, 0, "MyCalendarModel");
}


