
#include "RecurrenceProcessor.h"
//#include <QDate>
#include "EventRecurrence.h"

#define DEBUG

//-------------------------------------------------------------------------
// RecurrenceProcessor

RecurrenceProcessor::RecurrenceProcessor()
{
//  icalrecurrencetype_clear(&m_recur);
  m_iterator = QDateTime();
  m_until = QDateTime();
  m_generated = QDateTime();
  m_days = new DaySet(false);
}

void RecurrenceProcessor::expand(qint64 dtstart,
                                 const icalrecurrencetype &recur,
                                 qint64 range_start, qint64 range_end,
                                 QList<qint64> *expandedDates)
{
  int count = 0;

  if (dtstart >= range_start && dtstart < range_end) {
    expandedDates->append(dtstart);
    ++count;
  }

  QDateTime iterator = m_iterator;
//  QDateTime until = m_until;
  QDateTime generated = m_generated;
  DaySet days = m_days;

#ifdef DEBUG
  QDateTime dt_start_date = QDateTime::fromMSecsSinceEpoch(dtstart);
  QDateTime range_start_date = QDateTime::fromMSecsSinceEpoch(range_start);
  QDateTime range_end_time = QDateTime::fromMSecsSinceEpoch(range_end);
#endif

  days.setRecurrence(recur);

//  m_recur = recur;

  int freqAmount = recur.interval;
  int freq = recur.freq;
  if (freq == ICAL_WEEKLY_RECURRENCE) {
    freqAmount = 7 * recur.interval;
    if (freqAmount <= 0) {
      freqAmount = 7;
    }
  }

  if (freq == ICAL_NO_RECURRENCE) {
    // TODO: 这里应该是要处理下的
  }

  int byMonthCount = EventRecurrence::calculateByXXCount(recur.by_month,
                                                         ICAL_BY_MONTH_SIZE);
  bool usebymonth = freq > ICAL_MONTHLY_RECURRENCE && byMonthCount > 0;
  int byMonthDayCount = EventRecurrence::calculateByXXCount(recur.by_month_day,
                                                        ICAL_BY_MONTHDAY_SIZE);
  int byDayCount = EventRecurrence::calculateByXXCount(recur.by_day, ICAL_BY_DAY_SIZE);
  bool useDays = freq > ICAL_DAILY_RECURRENCE &&
      (byDayCount>0 || byMonthDayCount > 0);

  // TODO: 完善usebyhour, usebyminute, usebysecond

  iterator = QDateTime::fromMSecsSinceEpoch(dtstart);
  if (freq == ICAL_MONTHLY_RECURRENCE) {
    if (useDays) {
      // if ..., set the iterator day field to 1 because sometimes
      // we'll skip months if it's greater than 28.
      iterator.setDate(
            QDate(iterator.date().year(), iterator.date().month(), 1));
    }
  }

  qint64 untilDateValue;
  if (!icaltime_is_null_time(recur.until)) {
    untilDateValue = static_cast<qint64>( icaltime_as_timet(recur.until) );
  } else {
    untilDateValue = std::numeric_limits<qint64>::max();
  }

  bool eventEnded = false;
//  int failsafe = 0; // Avoid infinite loops

  while (!eventEnded) {
    int monthIndex = 0;

    int iteratorYear = iterator.date().year();
    int iteratorMonth = iterator.date().month();
    int iteratorDay = iterator.date().day();

    generated = iterator;

    do { // month
      int month = usebymonth ? recur.by_month[monthIndex] : iteratorMonth;

      int dayIndex = 1;
      int lastDayToExamine = 0;
//      QList<int> my_by_day;

      if (useDays) {
        if (freq == ICAL_WEEKLY_RECURRENCE) {
          // 这里%7是为了跟Android的weekDay保持一致
          int weekStartAdj = (iterator.date().dayOfWeek()%7 -
                              EventRecurrence::icalWeekday2TimeDay(recur.week_start) + 7) % 7;
          dayIndex = iterator.date().day() - weekStartAdj;
          lastDayToExamine = dayIndex + 6;
        } else {
          lastDayToExamine = generated.date().daysInMonth();
        }
//        generateDaysList(iterator.date(), &my_by_day);
//        my_by_day_count = my_by_day.count();
      }

      do { // day
//        int day = useDays ? my_by_day.at(dayIndex) : iteratorDay;

        int day;
        if (useDays) {
          if (!days.get(iterator, dayIndex)) {
            dayIndex++;
            continue;
          } else {
            day = dayIndex;
          }
        } else {
          day = iteratorDay;
        }

        QDate generated_date;
        unsafeNormalize(iteratorYear, month, day, &generated_date);
        generated = QDateTime(generated_date, generated.time());

        qint64 genDateValue = generated.toMSecsSinceEpoch();

        if (genDateValue >= dtstart) {

          // TODO: 原来interval > 0的跳跃性事件的判断是在这里，
          // 又重新循环了一遍，有种被骗了的感觉...
//          int filter = filter(recur, generated);

          // TODO: 这count究竟什么意思？？？
          if (!(dtstart == genDateValue &&
              dtstart >= range_start && dtstart < range_end)) {
            ++count;
          }

          if (genDateValue > untilDateValue) {
            eventEnded = true;
            break;
          }

          if (genDateValue >= range_end) {
            eventEnded = true;
            break;
          }

          if (genDateValue >= range_start) {
            expandedDates->append(genDateValue);
          }

          if (recur.count > 0 && recur.count == count) {
            eventEnded = true;
            break;
          }
        }
        dayIndex++;
      } while (!eventEnded && useDays && dayIndex <= lastDayToExamine);
      monthIndex++;
    } while (!eventEnded && usebymonth && monthIndex < byMonthCount);

    int oldDay = iterator.date().day();
    generated = iterator;
    int n = 1;
    while (true) {
      int value = freqAmount * n;
      switch (freq) {
      case ICAL_DAILY_RECURRENCE:
      case ICAL_WEEKLY_RECURRENCE:
        iterator = iterator.addDays(value);
        break;
      case ICAL_MONTHLY_RECURRENCE:
        iterator = iterator.addMonths(value);
        break;
      case ICAL_YEARLY_RECURRENCE:
        iterator = iterator.addYears(value);
        break;
      default:
        break;
      }

      if (freq != ICAL_YEARLY_RECURRENCE && freq != ICAL_MONTHLY_RECURRENCE) {
        break;
      }
      if (iterator.date().day() == oldDay) {
        break;
      }
      n++;
      iterator = generated;
    }
  }
}

void RecurrenceProcessor::unsafeNormalize(int year, int month, int day,
                                          QDate* normalized_date)
{
  //Andorid的month范围为0-11，QDate的是1-12，又要处理兼容了
  month--;

  while (day <= 0) {
    int days = month > 1 ? yearLength(year) : yearLength(year - 1);
    day += days;
    year -= 1;
  }

  if (month < 0) {
    int years = (month + 1) / 12 - 1;
    year += years;
    month -= 12 * years;
  } else if (month >= 12) {
    int years = month / 12;
    year += years;
    month -= 12 * years;
  }

  while (true) {
    if (month == 0) {
      int year_length = yearLength(year);
      if (day > year_length) {
        year++;
        day -= year_length;
      }
    }
    int month_length = QDate(year, month, 1).daysInMonth();
    if (day > month_length) {
      day -= month_length;
      month++;
      if (month > 12) {
        month -= 12;
        year++;
      }
    } else {
      break;
    }
  }

  // 跟开头的month--对应，粗暴解决兼容问题
  month++;
  *normalized_date = QDate(year, month, day);
}

int RecurrenceProcessor::yearLength(int year)
{
  return QDate::isLeapYear(year) ? 366 : 365;
}



//-------------------------------------------------------------------------
// DaySet

DaySet::DaySet(bool /*zulu*/)
{
  m_date = QDate();
}

void DaySet::setRecurrence(const icalrecurrencetype &recur)
{
  m_year = 0;
  m_month = -1;
  m_r = recur;
}

bool DaySet::get(const QDateTime &iterator, int day)
{
  int real_year = iterator.date().year();
  int real_month = iterator.date().month();

  QDate date = QDate();

  if (day < 1 || day > 28) {
    date = m_date;
    RecurrenceProcessor::unsafeNormalize(
          real_year, real_month, day, &date);
    real_year = date.year();
    real_month = date.month();
    day = date.day();
  }

  if (real_year != m_year || real_month != m_month) {
    if (date.isNull()) {
      date = m_date;
      RecurrenceProcessor::unsafeNormalize(
            real_year, real_month, day, &date);
    }
    m_year = real_year;
    m_month = real_month;
    m_days = generateDaysList(date, m_r);
  }
  return (m_days & (1<<day)) != 0;
}

int DaySet::generateDaysList(const QDate &generated,
                             const icalrecurrencetype &recur)
{
  int days = 0;

  int count, by_day_num;
  int day, days_in_this_month;
  int firstday_of_week;

  days_in_this_month = generated.daysInMonth();

  // BYDAY
  count = EventRecurrence::calculateByXXCount(recur.by_day,
                                                  ICAL_BY_DAY_SIZE);
  if (count > 0) {
    day = generated.day();
    while (day >= 8) {
      day -= 7;
    }
    // %7 以与Andriod的weekDay保持一致
    firstday_of_week = generated.dayOfWeek() % 7;
    if (firstday_of_week >= day) {
      firstday_of_week = firstday_of_week - day + 1;
    } else {
      firstday_of_week = firstday_of_week - day + 8;
    }

//    by_day = recur.by_day;
    for (int i = 0; i < count; ++i) {
      by_day_num = recur.by_day[i] / 8;
//      if (by_day_num == 0) {
//        by_day_num = recur.by_day[i] > 0 ? 1 : -1;
//      }
      day = EventRecurrence::icalByDay2WeekDay(recur.by_day[i]) -
          firstday_of_week + 1;
      if (day <= 0) {
        day += 7;
      }

      if (by_day_num == 0) {
        // by_day_num is 0, each day in the month/week
        for (; day <= days_in_this_month; day += 7) {
          days |= 1 << day;
        }
      } else if (by_day_num > 0) {
        // by_day_num is positive, count from the begining of the month
        day += 7 * (by_day_num - 1);
        if (day <= days_in_this_month) {
          days |= 1 << day;
        }
      } else { // by_day_num < 0
        // by_day_num is negative, count from the end of the month
        for (; day <= days_in_this_month; day += 7) { // find the last one
        }
        day += 7 * by_day_num;
        if (day >= 1) {
          days |= 1 << day;
        }
      }
    }
  }

  // BYMONTHDAY
  if (recur.freq > ICAL_WEEKLY_RECURRENCE) {
    count = EventRecurrence::calculateByXXCount(recur.by_month_day,
                                                    ICAL_BY_MONTHDAY_SIZE);
    if (count != 0) {
//      by_month_day = recur.by_month_day;
      int by_day_count = EventRecurrence::calculateByXXCount(
            recur.by_day, ICAL_BY_DAY_SIZE);
      if (by_day_count == 0) {
        for (int i = 0; i < count; ++i) {
          day = recur.by_month_day[i];
          if (day >= 0) {
            days |= 1 << day;
          } else { // day < 0
            day = days_in_this_month + day + 1;
            if (day >= 1 && day <= days_in_this_month) {
              day |= 1 << day;
            }
          }
        }
      } else { // by_day_count != 0
        for (int day = 0; day <= days_in_this_month; ++day) {
          bool end = false;
          while (!end) {
            if ((days & (1 << day)) != 0) {
              for (int i = 0; i < count && !end; ++i) {
                if (recur.by_month_day[i] == day) {
                  end = true;
                  break;
                }
              }
              days &= ~(1 << day);
            }
          }
        } // end for
      } // end by_day_count
    } // end count
  } // end BYMONTHDAT
  return days;
}


//void RecurrenceProcessor::generateDaysList(const QDate &current_month,
//                                           QList<int> *dayslist)
//{
////  int dayIndex = 0;
//  int days_in_current_month = current_month.daysInMonth();
//  QDate first_day = QDate(current_month.year(), current_month.month(), 1);

//  // TODO: 这里统统没考虑规则为负的情形
//  // (计算日期从每月的最后倒着往前数）
//  //类似BYMONTHDAY=-3,BYDAY=-2MO,... 当前版本，遇此情形可能会崩溃！

//  // TODO: interval > 1的处理依然不正确，
//  // 对于类似BYMONTHDAY=31,29的情形，last_occur的追踪是错误的！

//  // BYMONTHDAY
//  if (byMonthDayCount > 0 && m_recur.freq > ICAL_WEEKLY_RECURRENCE) {
//    int dayIndex = 0;
//    int interval = m_recur.interval;

//    while (dayIndex < byMonthDayCount) {
//      int day = m_recur.by_month_day[dayIndex];
//      bool first_occured = m_bymonthday_last_occurs.at(dayIndex).isNull();

//      if (day <= days_in_current_month) {
//        if (interval == 1 || first_occured) {
//          dayslist->append(day);
//          if (interval > 1) {
//            m_bymonthday_last_occurs.append(
//                  QDate(current_month.year(), current_month.month(), day));
//          }
//        } else { // interval > 1 && first_occured = false
//          QDate last_occured_date = m_bymonthday_last_occurs.at(dayIndex);
//          last_occured_date = last_occured_date.addMonths(interval);
//          if (last_occured_date.month() == current_month.month()) {
//            dayslist->append(day);
//            m_bymonthday_last_occurs[dayIndex] = last_occured_date;
//          }
//        } // end else
//      }
//      dayIndex++;
//    } // end while


//  } // end BYMONTHDAY

//  // BYDAY
//  if (byDayCount > 0) {
//    // TODO: 这里暂时忽略了WKST的影响，有待以后完善，额，2.0版以后再说吧

//    int dayIndex = 0;
//    int firstday_of_week = first_day.dayOfWeek();
//    int interval = m_recur.interval;

//    while (dayIndex < byDayCount) {
//      int ical_by_day_day = m_recur.by_day[dayIndex];
//      int day_number = ical_by_day_day / 8;
//      int day_of_week = icalByDay2WeekDay(ical_by_day_day);

//      int day = 0;
//      if (day_of_week >= firstday_of_week) {
//        day = day_of_week - firstday_of_week + 1 + 7*day_number;
//      } else {
//        day = day_of_week + 8 - firstday_of_week + 7*day_number;
//      }
//      bool is_first_occured = m_byday_last_occurs.at(dayIndex).isNull();

//      if (day <= days_in_current_month) {
//        if (interval == 1 || is_first_occured) {
//          // weekly event
//          if (m_recur.freq == ICAL_WEEKLY_RECURRENCE) {
//            while (day <= days_in_current_month) {
//              dayslist->append(day);
//              if (interval > 1) {
//                QDate t_date = m_byday_last_occurs.at(dayIndex);
//                m_byday_last_occurs[dayIndex] =
//                    QDate(t_date.year(), t_date.month(), day);
//              }
//              day += 7;
//            }
//          } else { // monthly event
//            if (day <= days_in_current_month) {
//              dayslist->append(day);
//              if (interval > 1) {
//                QDate t_date = m_byday_last_occurs.at(dayIndex);
//                m_byday_last_occurs[dayIndex] =
//                    QDate(t_date.year(), t_date.month(), day);
//              }
//            }
//          }
//        } else { // interval > 1 && is_first_occured = false
//          // weekly event
//          if (m_recur.freq == ICAL_WEEKLY_RECURRENCE) {
//            QDate last_occured_date = m_byday_last_occurs.at(dayIndex);
//            last_occured_date = last_occured_date.addDays(interval * 7);
//            if (last_occured_date.month() == current_month.month()) {
//              while (day <= days_in_current_month) {
//                dayslist->append(day);
//                QDate t_date = m_byday_last_occurs.at(dayIndex);
//                m_byday_last_occurs[dayIndex] =
//                    QDate(t_date.year(), t_date.month(), day);
//                day += interval*7;
//              }
//            }
//          } else { // monthly event
//            QDate last_occured_date = m_byday_last_occurs.at(dayIndex);
//            last_occured_date = last_occured_date.addMonths(interval);
//            if (last_occured_date.month() == current_month.month()) {
//              if (day <= days_in_current_month) {
//                dayslist->append(day);
//                QDate t_date = m_byday_last_occurs.at(dayIndex);
//                m_byday_last_occurs[dayIndex] =
//                    QDate(t_date.year(), t_date.month(), day);
//              }
//            }
//          }
//        } // end else
//      } // end if

//      dayIndex++;
//    } // end while
//  } // end BYDAY
//}
