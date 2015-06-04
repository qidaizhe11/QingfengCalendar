#ifndef EVENTRECURRENCE_H
#define EVENTRECURRENCE_H

#include <libical/ical.h>

class EventRecurrence
{
public:
  EventRecurrence();

  static int calculateByXXCount(const short* by_XX_array, int by_XX_size);

  static int icalWeekday2TimeDay(const icalrecurrencetype_weekday& ical_weekday);
  static int icalByDay2WeekDay(short ical_by_day_day);
};

#endif // EVENTRECURRENCE_H
