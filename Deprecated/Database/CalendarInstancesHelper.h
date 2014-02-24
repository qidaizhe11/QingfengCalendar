#ifndef CALENDARINSTANCESHELPER_H
#define CALENDARINSTANCESHELPER_H

#include <QString>
#include <QDateTime>
#include <QSqlRecord>
#include <QList>
//#include <libical/ical.h>

class Instance;

class CalendarInstancesHelper
{
public:
  CalendarInstancesHelper();
  ~CalendarInstancesHelper() {}

  void performInstanceExpansion(qint64 begin, qint64 end,
                                const QSqlRecord& record);

  void instancesReplace(const QList<Instance*>& instances_list);

private:

};

class Instance
{
public:
  Instance() {}
  ~Instance() {}

  int m_event_id;
  qint64 m_begin;
  qint64 m_end;
};

#endif // CALENDARINSTANCESHELPER_H
