#include "eventutils.h"
#include <QDate>
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
  // By default, QQuickItem does not draw anything. If you subclass
  // QQuickItem to create a visual item, you will need to uncomment the
  // following line and re-implement updatePaintNode()

  // setFlag(ItemHasContents, true);
}

EventUtils::~EventUtils()
{
}

int EventUtils::gridIndex(const QDate &date, const QDate& visible_date)
{
  QDate first_day_of_month_date(visible_date.year(), visible_date.month(), 1);
  int difference = ((first_day_of_month_date.dayOfWeek() -
                     QLocale::system().firstDayOfWeek()) + 7) % 7;
  if (difference == 0) {
    difference += daysInAWeek;
  }

  QDate first_day_to_display = first_day_of_month_date.addDays(-difference);

  int index = first_day_to_display.daysTo(date);
  return index;
}

int EventUtils::lastDays(const QDate &start_date, const QDate &end_date)
{
  return start_date.daysTo(end_date) + 1;
}
