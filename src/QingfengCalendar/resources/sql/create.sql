PRAGMA user_version = 1;

CREATE TABLE Calendars (
    _id INTEGER PRIMARY KEY,
    account_name TEXT,
    account_type TEXT,
    name TEXT,
    calendar_displayName TEXT,
    calendar_color INTEGER,
    calendar_color_index TEXT,
    visible INTEGER NOT NULL DEFAULT 1,
    calendar_timezone TEXT,
    maxReminders INTEGER DEFAULT 5,
);

CREATE TABLE Events (
    _id INTEGER PRIMARY KEY AUTOINCREMENT,
    calendar_id INTEGER NOT NULL,
    title TEXT,
    eventLocation TEXT,
    description TEXT,
    eventStatus INTEGER,
    dtstart INTEGER,
    dtend INTEGER,
    eventTimezone TEXT,
    duration TEXT,
    allDay INTEGER NOT NULL DEFAULT 0,
    accessLevel INTEGER NOT NULL DEFAULT 0,
    availability INTEGER NOT NULL DEFAULT 0,
    hasAlarm INTEGER NOT NULL DEFAULT 0,
    rrule TEXT,
    rdate TEXT,
    lastDate INTEGER,
    hasAttendeeData INTEGER NOT NULL DEFAULT 0,
    organizer STRING,
    eventEndTimezone TEXT,
);

CREATE TABLE Instances (
    _id INTEGER PRIMARY KEY,
    event_id INTEGER,
    begin INTEGER,
    end INTEGER,
    startDay INTEGER,
    endDay INTEGER,
    startMinute INTEGER,
    endMinute INTEGER,
    UNIQUE (event_id, begin, end));

CREATE TABLE Attendees (
    _id INTEGER PRIMARY KEY,
    event_id INTEGER,
    attendeeName TEXT,
    attendeeEmail TEXT,
    attendeeStatus INTEGER,
    attendeeRelationship INTEGER,
    attendeeType INTEGER,
    attendeeIdentity TEXT,
    attendeeIdNamespace TEXT);

CREATE TABLE Reminders (
    _id INTEGER PRIMARY KEY,
    event_id INTEGER,
    minutes INTEGER,
    method INTEGER NOT NULL DEFAULT 0);

CREATE TRIGGER calendar_cleanup
DELETE ON Calendars BEGIN
    DELETE FROM Events WHERE calendar_id=old._id;
END;

CREATE INDEX eventsCalendarIdIndex ON Events (calendar_id);
CREATE INDEX instancesStartDayIndex ON Instances (startDay);
CREATE INDEX attendeesEventIdIndex ON Attendees (event_id);
CREATE INDEX remindersEventIdIndex ON Reminders (event_id);
-- CREATE INDEX calendarAlertsEventIdIndex ON CalendarAlerts (event_id);

CREATE VIEW view_events AS
SELECT Events._id AS
_id,
title,
description,
eventLocation,
eventStatus,
dtstart,
dtend,
duration,
eventTimezone,
eventEndTimezone,
allDay,
accessLevel,
availability,
hasAlarm,
rrule,
rdate,
hasAttendeeData,
calendar_id,
organizer,
Calendars.account_name AS account_name,
Calendars.account_type AS account_type,
calendar_timezone,
calendar_displayName,
visible,
calendar_color,
calendar_color_index,
maxReminders,
FROM Events JOIN Calendars ON (Events.calendar_id=Calendars._id)

CREATE TRIGGER events_cleanup_delete
DELETE ON Events BEGIN
    DELETE FROM Instances WHERE event_id=old._id;
    DELETE FROM Attendees WHERE event_id=old._id;
    DELETE FROM Reminders WHERE event_id=old._id;
END;

-- CREATE TRIGGER event_color_update
-- UPDATE OF eventColor_index ON Events
-- WHEN new.eventColor_index NOT NULL BEGIN
--     UPDATE Events SET eventColor=(
--         SELECT color FROM Colors WHERE account_name=(
--             SELECT account_name FROM Calendars WHERE _id=new.calendar_id)
--         AND account_type=(
--             SELECT account_type FROM Calendars WHERE _id=new.calendar_id)
--         AND color_index=new.eventColor_index AND color_type=1)
--     WHERE _id=old._id;
-- END;

CREATE TRIGGER calendar_color_update
UPDATE OF calendar_color_index ON Calendars
WHEN new.calendar_color_index NOT NULL BEGIN
    UPDATE Calendars SET calendar_color=(
        SELECT color FROM Colors WHERE account_name=new.account_name
        AND account_type=new.account_type
        AND color_index=new.calendar_color_index
        AND color_type=0)
    WHERE _id=old._id;
END;
