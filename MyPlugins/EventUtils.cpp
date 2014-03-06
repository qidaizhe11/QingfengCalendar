#include "EventUtils.h"
#include <QDateTime>
#include <QLocale>

namespace {
    static const int daysInAWeek = 7;

    /*
        Not the number of weeks per month, but the number of weeks that are
        shown on a typical calendar.
    */
    static const int weeksOnACalendarMonth = 6;

    /*
        The amount of days to populate the calendar with.
    */
    static const int daysOnACalendarMonth = daysInAWeek * weeksOnACalendarMonth;
}

EventUtils::EventUtils(QQuickItem *parent):
  QQuickItem(parent)
{
  // setFlag(ItemHasContents, true);
}

EventUtils::~EventUtils()
{
}

//int EventUtils::gridIndex(const QDate &date, const QDate& visible_date)
//{
//  QDate first_day_of_month_date(visible_date.year(), visible_date.month(), 1);
//  int difference = ((first_day_of_month_date.dayOfWeek() -
//                     QLocale::system().firstDayOfWeek()) + 7) % 7;
//  if (difference == 0) {
//    difference += daysInAWeek;
//  }

//  QDate first_day_to_display = first_day_of_month_date.addDays(-difference);

//  int index = first_day_to_display.daysTo(date);
//  return index;
//}

int EventUtils::daysTo(const QDateTime &start, const QDateTime &end)
{
  return start.daysTo(end);
}

int EventUtils::lastDays(const QDateTime &start_date, const QDateTime &end_date)
{
//  qDebug() << "Calculate last days: " + start_date.toString() + ", " + end_date.toString();
  int days = start_date.daysTo(end_date);

  if ( !(end_date.time() == QTime(0, 0, 0) &&
         start_date.time() == QTime(0, 0, 0)) ) {
    days += 1;
  }

//  qDebug() << "Days: " + QString::number(days);
  return days;

//  return start_date.daysTo(end_date) + 1;
}
