
#include "EventRecurrence.h"
#include <QString>

EventRecurrence::EventRecurrence()
{
}

int EventRecurrence::calculateByXXCount(const short* by_XX_array,
                                                int by_XX_size)
{
  int count = 0;
  for (int i = 0; i < by_XX_size && by_XX_array[i] != ICAL_RECURRENCE_ARRAY_MAX;
       ++i) {
    ++count;
  }
  return count;
}

//
// @return  1 = Monday, ..., 7 = Sunday.
// (Qt中QDate的dayOfWeek, 1 = Monday, 7 = Sunday)
//
int EventRecurrence::icalByDay2WeekDay(short ical_by_day_day) {
  ical_by_day_day = static_cast<int>(ical_by_day_day);
  ical_by_day_day %= 8;

  if (ical_by_day_day == 1) { // Sunday
    return 7;
  } else {
    return ical_by_day_day - 1;
  }
}

// 用于解析WKST
int EventRecurrence::icalWeekday2TimeDay(
    const icalrecurrencetype_weekday& ical_weekday)
{
  switch (ical_weekday) {
  case ICAL_SUNDAY_WEEKDAY:
    return 0;
  case ICAL_MONDAY_WEEKDAY:
    return 1;
  case ICAL_TUESDAY_WEEKDAY:
    return 2;
  case ICAL_WEDNESDAY_WEEKDAY:
    return 3;
  case ICAL_THURSDAY_WEEKDAY:
    return 4;
  case ICAL_FRIDAY_WEEKDAY:
    return 5;
  case ICAL_SATURDAY_WEEKDAY:
    return 6;
  default:
    throw QString("bad type: ICAL_NO_WEEKDAY");
  }
}
