#ifndef CALENDARCONTRACT_H
#define CALENDARCONTRACT_H

#include <QString>

class BaseColumns {
public:
  static const QString _ID;
  static const QString _COUNT;
};

class Tables {
public:
  static const QString CALENDARS;
  static const QString EVENTS;
  static const QString INSTANCES;
  static const QString ATTENDEES;
  static const QString REMINDERS;
  static const QString COLORS;
};

class Views {
  static const QString EVENTS;
};

class Calendars
{
public:
  static const QString NAME;
  static const QString CALENDAR_COLOR;
  static const QString CALENDAR_DISPLAYNAME;
  static const QString VISIBLE;
};

class Events : public BaseColumns
{
public:
  static const QString CALENDAR_ID;
  static const QString TITLE;
  static const QString DESCRIPTION;
  static const QString EVENT_LOCATION;
  static const QString DT_START;
  static const QString DT_END;
  static const QString DURATION;
  static const QString EVENT_TIMEZONE;
  static const QString EVENT_END_TIMEZONE;
  static const QString ALL_DAY;
  static const QString AVAILABILITY;
  static const QString ACCESS_LEVEL;
  static const QString RRULE;
  static const QString RDATE;
  static const QString ORGANIZER;
  static const QString LAST_DATE;

  static const int ACCESS_DEFAULT;
  static const int ACCESS_CONFIDENTIAL;
  static const int ACCESS_PRIVATE;
  static const int ACCESS_PUBLIC;

  static const int AVAILABILITY_BUSY;
  static const int AVAILABILITY_FREE;
  static const int AVAILABILITY_TENTATIVE;
};

class Instances
{
public:
  static const QString BEGIN;
  static const QString END;
  static const QString EVENT_ID;
  static const QString START_DAY;
  static const QString END_DAY;
  static const QString START_MINUTE;
  static const QString END_MINUTE;
};

class Reminders
{
public:
  static const QString EVENT_ID;
  static const QString MINUTES;
  static const QString METHOD;

  static const int METHOD_DEFAULT;
  static const int METHOD_ALERT;
  static const int METHOD_EMAIL;
  static const int METHOD_SMS;
  static const int METHOD_ALARM;
};

#endif // CALENDARCONTRACT_H
