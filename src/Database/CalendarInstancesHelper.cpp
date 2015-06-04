
#include "CalendarInstancesHelper.h"
#include <QtSql>
#include <libical/ical.h>
#include "CalendarContract.h"
#include "RecurrenceProcessor.h"

#define DEBUG

CalendarInstancesHelper::CalendarInstancesHelper()
{

//  struct icalrecurrencetype recur = icalrecurrencetype_from_string(
//        "FREQ=WEEKLY;COUNT=35;INTERVAL=2;BYDAY=TU,WE,TH");
//  struct icalrecurrencetype recur = icalrecurrencetype_from_string(
//        "FREQ=WEEKLY;COUNT=8;BYDAY=MO,TU,WE,TH,FR");

//  char* str = "FREQ=MONTHLY;BYDAY=2SU";
//  char* str = "FREQ=WEEKLY;COUNT=8;BYDAY=SU";
//  icalrecurrencetype recur = icalrecurrencetype_from_string(str);

//  int count = recur.count;
//  QString str = "PT32400S";
//  QByteArray byte_array = str.toLatin1();
//  char* c_str = byte_array.data();
//  icaldurationtype duration = icaldurationtype_from_string(c_str);

//  int i = duration.days;
}

//
// public
//

void CalendarInstancesHelper::performInstanceExpansion(qint64 begin, qint64 end,
                                                       const QSqlRecord &record)
{
  RecurrenceProcessor* rp = new RecurrenceProcessor();

  QList<Instance*> instances_list;

  qint64 dtstart_millis = record.value(Events::DT_START).toLongLong();
  int event_id = record.value(Events::_ID).toInt();
//  int calendar_id = record.value(Events::CALENDAR_ID).toInt();
  QString duration_str = record.value(Events::DURATION).toString();

  QString rrule_str = record.value(Events::RRULE).toString();
//  QString rdate_str = record.value(Events::RDATE).toString();

  icaldurationtype duration;

  if (!duration_str.isEmpty()) {
    QByteArray byte_array = duration_str.toLatin1();
    char* c_dur = byte_array.data();
    duration = icaldurationtype_from_string(c_dur);
  }

  if (!rrule_str.isEmpty()) {
    QByteArray byte_array = rrule_str.toLatin1();
    const char* c_rrule = byte_array.data();
    icalrecurrencetype recurrence = icalrecurrencetype_from_string(c_rrule);

    if (duration_str.isEmpty()) {
      // TODO: 把它搞成 全天 事件
    }

    QList<qint64> dates;

    rp->expand(dtstart_millis, recurrence, begin, end, &dates);

#ifdef DEBUG
    QList<QDateTime> dates_qdate;
    foreach (qint64 date, dates) {
      QDateTime date_time = QDateTime::fromMSecsSinceEpoch(date);
      dates_qdate.append(date_time);
    }
#endif

    qint64 duration_millis = icaldurationtype_as_int(duration);

    foreach (qint64 date, dates) {
      qint64 dtend_millis = date + duration_millis;
      Instance* instance_value = new Instance();
      instance_value->m_event_id = event_id;
      instance_value->m_begin = date;
      instance_value->m_end = dtend_millis;
      instances_list.append(instance_value);
    }

    instancesReplace(instances_list);
  }
}

void CalendarInstancesHelper::instancesReplace(const QList<Instance *>& instances_list)
{
  Instance* instance_value = NULL;
  QString database_error;

  QString SQL_REPLACE_INSTANCES = QString(
        "insert or replace into " + Tables::INSTANCES + " ("
        + Instances::EVENT_ID + ", "
        + Instances::BEGIN + ", "
        + Instances::END + ") "
        + "values ("
        + ":event_id, :begin, :end)");

  QSqlDatabase db = QSqlDatabase::database();
  if (db.isValid()) {
    if (db.isOpen()) {
      QSqlQuery query(db);

      foreach (instance_value, instances_list) {
        query.prepare(SQL_REPLACE_INSTANCES);

        query.bindValue(":event_id", instance_value->m_event_id);
        query.bindValue(":begin", instance_value->m_begin);
        query.bindValue(":end", instance_value->m_end);
        query.exec();
      }
      database_error = query.lastError().text();
    }
  }
}

//
// private
//


