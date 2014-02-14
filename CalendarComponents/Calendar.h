#ifndef CALENDAR_H
#define CALENDAR_H

#include <QObject>
#include <QDate>
#include <QAbstractListModel>
//#include <QAbstractItemModel>
#include "DayData.h"
#include "DaysModel.h"

class Calendar : public QObject
{
  Q_OBJECT

  //
  // This property is used to determine which date from which month to show.
  // although the Day is less importance.
  //
  Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate
             NOTIFY startDateChanged)

  //
  // This determines which type of data types should be contained in
  // selectedDayModel and upcomingEventsModel.
  //
  Q_PROPERTY(int types READ types WRITE setTypes NOTIFY typesChanged)

  //
  // The number of days a week contains.
  // TODO: perhaps this one can just be removed. A week is 7 days by definition.
  //
//  Q_PROPERTY(int days READ days WRITE setDays NOTIFY daysChanged)

  //
  // The number of weeks that the model property should contain.
  //
  Q_PROPERTY(int weeks READ weeks WRITE setWeeks NOTIFY weeksChanged)

  //
  // The start day of a week. By default this is Monday (Qt::Monday).
  //
  Q_PROPERTY(int startDay READ startDay WRITE setStartDay NOTIFY startDayChanged)

  //
  // The full year in a numberic value. For example 2013, not 13.
  //
  Q_PROPERTY(int year READ year CONSTANT)

  //
  // If an error occured, it will be set in this string as human readable text.
  //
  Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

  //
  // This model contains the actual grid data of days.
  //
  // For example, if you have set:
  //  - days = 7 (7 days in one week)
  //  - weeks = 6 (6 weeks in one month view)
  // then this model will contain 42 entries. Each entry contains
  // metadata about the current day. The exact metadata can be found in
  // "DaysModel.cpp" where the exact names usable in QML are being set.
  //
  Q_PROPERTY(QAbstractItemModel* daysModel READ daysModel CONSTANT)

  //
  // This model contains the week numbers for the given data grid.
  Q_PROPERTY(QList<int> weeksModel READ weeksModel CONSTANT)

  //
  // This model contains the events that occur on this day.
  // This also includes events that might not occuring today, but overlap it.
  //
//  Q_PROPERTY(QAbstractItemModel* selectedDayModel READ selectedDayModel
//             CONSTANT)

  //
  // This model contains the events that are upcoming from the date you
  // selected.
  // By default the selected date is your current date.
  // This model will then contain the dates from the selected date + 1 month.
  // Not changable for the moment.
  //
//  Q_PROPERTY(QAbstractItemModel* upcomingEventsModel READ upcomingEventsModel
//             CONSTANT)

  Q_ENUMS(Types)

public:
  enum Type {
    Holiday = 1,
    Event = 2,
    Todo = 4,
    Journal = 8
  };
  Q_DECLARE_FLAGS(Types, Type)

  explicit Calendar(QObject *parent = 0);

  // Start date
  QDate startDate() const;
  void setStartDate(const QDate &dateTime);

  // Types
  int types() const;
  void setTypes(int types);

  // Weeks
  int weeks() const;
  void setWeeks(int weeks);

  // Start day
  int startDay() const;
  void setStartDay(int day);

  // Error message
  QString errorMessage() const;

  // 这个属性应该可以拿掉
  int year() const;

  // Model containing all events for the given date range
  QAbstractListModel* daysModel() const;

  // Model filter that only gived the events for a selected day
//  QAbstractItemModel* selectedDayModel() const;

//  QAbstractItemModel* upcomingEventsModel() const;

  QList<int> weeksModel() const;

  Q_INVOKABLE void nextMonth();
  Q_INVOKABLE void nextYear();
  Q_INVOKABLE void previousMonth();
  Q_INVOKABLE void previousYear();

  // Get the day name by the day possible days (1 - 7).
  // see Qt DayofWeek enum
//  Q_INVOKABLE QString dayName(int weekday) const;

  // This sets the currently selected date. This will also the data of the
  // selectedDayModel property since it will contain date of the selected day.
  //
  // TODO: why const?
  //
//  Q_INVOKABLE void setSelectedDay(int year, int month, int day) const;

  // Changing this will update the upcomingEventModel based on the date you
  // provided.
//  Q_INVOKABLE void upcommingEventsFromDay(int year, int month, int day) const;

signals:
  void startDateChanged();
  void typesChanged();
//  void daysChanged();
  void weeksChanged();
  void startDayChanged();
  void errorMessageChanged();

public slots:
  //
  // This function udpates the actual data in the model (m_model) member.
  //
  void updateData();

private:
  QDate m_startDate;
  Types m_types;
  QList<DayData> m_dayList;
  QList<int> m_weekList;
  DaysModel* m_model;
//  int m_days;
  int m_weeks;
  int m_startDay;
  QString m_errorMessage;

};

#endif // CALENDAR_H
