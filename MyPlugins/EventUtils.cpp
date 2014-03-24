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
}

EventUtils::~EventUtils()
{
}

int EventUtils::daysTo(const QDateTime &start, const QDateTime &end)
{
  return start.daysTo(end);
}

int EventUtils::lastDays(const QDateTime &start_date, const QDateTime &end_date)
{
  // when the time of end_date is 00:00, QDateTime judge it as a new day, but
  // here it shouldn't.
  QDateTime end = end_date.addSecs(-60);

  return start_date.daysTo(end) + 1;
}
