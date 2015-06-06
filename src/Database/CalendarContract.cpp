
#include "CalendarContract.h"

// BaseColumns

const QString BaseColumns::_ID = "_id";
const QString BaseColumns::_COUNT = "_count";

// Tables

const QString Tables::CALENDARS = "Calendars";
const QString Tables::EVENTS = "Events";
const QString Tables::INSTANCES = "Instances";
const QString Tables::ATTENDEES = "Attendees";
const QString Tables::REMINDERS = "Reminders";
const QString Tables::COLORS = "Colors";

// Views
const QString Views::EVENTS = "view_events";

// Calendars

const QString Calendars::NAME = "name";
const QString Calendars::ACCOUNT_NAME = "account_name";
const QString Calendars::ACCOUNT_TYPE = "account_type";
const QString Calendars::CALENDAR_COLOR = "calendar_color";
const QString Calendars::CALENDAR_DISPLAYNAME = "calendar_displayName";
const QString Calendars::VISIBLE = "visible";

// Events

const QString Events::CALENDAR_ID = "calendar_id";
const QString Events::TITLE = "title";
const QString Events::DESCRIPTION = "description";
const QString Events::EVENT_LOCATION = "eventLocation";
const QString Events::DT_START = "dtstart";
const QString Events::DT_END = "dtend";
const QString Events::DURATION = "duration";
const QString Events::EVENT_TIMEZONE = "eventTimezone";
const QString Events::EVENT_END_TIMEZONE = "eventEndTimezone";
const QString Events::ALL_DAY = "allday";
const QString Events::AVAILABILITY = "availability";
const QString Events::ACCESS_LEVEL = "accessLevel";
const QString Events::RRULE = "rrule";
const QString Events::RDATE = "rdate";
const QString Events::ORGANIZER = "organizer";
const QString Events::LAST_DATE = "lastDate";

// Instances

const QString Instances::BEGIN = "begin";
const QString Instances::END = "end";
const QString Instances::EVENT_ID = "event_id";
const QString Instances::START_DAY = "startDay";
const QString Instances::END_DAY = "endDay";
const QString Instances::START_MINUTE = "startMinute";
const QString Instances::END_MINUTE = "endMinute";

// Reminders

const QString Reminders::EVENT_ID = "event_id";
const QString Reminders::MINUTES = "minutes";
const QString Reminders::METHOD = "method";
