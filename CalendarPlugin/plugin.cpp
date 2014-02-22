#include "plugin.h"

#include "Private/qquickrangeddate_p.h"

void QtQuickControlsPlugin::registerTypes(const char *uri)
{
  Q_UNUSED(uri);

  const char* private_uri = "MyCalendar.Controls.Private";
  qmlRegisterType<QQuickRangedDate>(private_uri, 1, 0, "RangedDate");
}
