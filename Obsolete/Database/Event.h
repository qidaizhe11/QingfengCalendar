#ifndef EVENT_H
#define EVENT_H

#include <QSqlRecord>
#include <QDateTime>
#include <QString>
#include <QList>

class Event
{
public:
  enum DurationType {
    Duration_none = 0,
    Duration_30_min,
    Duration_1_hour,
    Duration_90_min,
    Duration_2_hour,
    Duration_all_day,
    Duration_custom
  };

//  enum RepeatType {
//    Repeat_none = 0,
//    Repeat_day,
//    Repeat_workday,
//    Repeat_week,
//    Repeat_month
//  };

  Event();

  static void loadEvents(const QDate& start_day, int days, QList<Event> *events);

private:
//  void load(const QSqlRecord& record);

  int m_event_id;
  QString m_title;
  QString m_description;
  QString m_location;
  QDateTime m_dtstart;
  QDateTime m_dtend;
  DurationType m_duration;
  bool m_allday;
  bool m_is_repeating;
  int m_access_level;
  int m_availability;

};

#endif // EVENT_H
