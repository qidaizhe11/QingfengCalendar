#ifndef RECURRENCEPROCESSOR_H
#define RECURRENCEPROCESSOR_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <libical/ical.h>

class DaySet;

class RecurrenceProcessor
{
public:
  RecurrenceProcessor();

  void expand(qint64 dtstart, const icalrecurrencetype& recur,
              qint64 range_start, qint64 range_end,
              QList<qint64>* expandedDates);



  static void unsafeNormalize(int year, int month, int day, QDate* normalized_date);

  static int yearLength(int year);

private:
//  void generateDaysList(const QDate& current_month,
//                        const icalrecurrencetype& recur,
//                        int byDayCount, int byMonthDayCount,
//                        QList<int>* dayslist);

  QDateTime m_iterator;
  QDateTime m_until;
  QDateTime m_generated;
  DaySet* m_days;

  icalrecurrencetype m_recur;
//  int byDayCount, byMonthDayCount;
};

class DaySet
{
public:
  DaySet(bool zulu);

  void setRecurrence(const icalrecurrencetype& recur);

  bool get(const QDateTime& iterator, int day);

private:
  int generateDaysList(const QDate& generated,
                       const icalrecurrencetype& recur);

  icalrecurrencetype m_r;
  int m_days;
  QDate m_date;
  int m_year;
  int m_month;
};

#endif // RECURRENCEPROCESSOR_H
