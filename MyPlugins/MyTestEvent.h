#ifndef MYTESTEVENT_H
#define MYTESTEVENT_H

#include <QDateTime>

class MyTestEvent
{
public:
  MyTestEvent();

  QDateTime startDateTime() const { return m_start_datetime; }
  void setStartDateTime(const QDateTime& date_time) { m_start_datetime = date_time; }

  QDateTime endDateTime() const { return m_end_datetime; }
  void setEndDateTime(const QDateTime& date_time) { m_end_datetime = date_time; }

private:
  QDateTime m_start_datetime;
  QDateTime m_end_datetime;
};

#endif // MYTESTEVENT_H
