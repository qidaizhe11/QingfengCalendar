#include "private_plugin.h"

#include <qqml.h>

#include "qquickrangeddate_p.h"
#include "qquickcalendarmodel_p.h"

void PrivatePlugin::registerTypes(const char *uri)
{
  // @uri MyCalendar.Controls.Private
  qmlRegisterType<QQuickRangedDate>(uri, 1, 0, "RangedDate");
  qmlRegisterType<QQuickCalendarModel>(uri, 1, 0, "CalendarModel");

}


