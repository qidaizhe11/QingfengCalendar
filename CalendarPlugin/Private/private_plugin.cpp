#include "private_plugin.h"

#include <qqml.h>

#include "qquickrangeddate_p.h"
#include "qquickcalendarmodel_p.h"
#include "qquickabstractstyle_p.h"
#include "qquickcontrolsettings_p.h"
#include "qquickcontrolsprivate_p.h"

void PrivatePlugin::registerTypes(const char *uri)
{
  // @uri MyCalendar.Controls.Private
  qmlRegisterType<QQuickAbstractStyle>(uri, 1, 0, "AbstractStyle");
  qmlRegisterType<QQuickRangedDate>(uri, 1, 0, "RangedDate");
  qmlRegisterType<QQuickCalendarModel>(uri, 1, 0, "CalendarModel");
//  qmlRegisterType<QQuickControlSettings>(uri, 1, 0, "Settings")
  qmlRegisterSingletonType<QQuickControlSettings>(uri, 1, 0, "Settings",
                      QQuickControlsPrivate::registerSettingsModule);
}


