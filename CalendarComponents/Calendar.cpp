
#include "Calendar.h"

#define DEBUG

Calendar::Calendar(QObject *parent) :
  QObject(parent),
  m_types(Holiday | Event | Todo | Journal),
  m_dayList(),
  m_weekList(),
  m_weeks(),
  m_startDay(Qt::Monday),
  m_errorMessage()
{
  m_model = new DaysModel(this);
  m_model->setSourceData(&m_dayList);

#ifdef DEBUG
  m_startDate = QDate::currentDate();
#endif
}

QDate Calendar::startDate() const
{
  return m_startDate;
}

void Calendar::setStartDate(const QDate &dateTime)
{
  if (m_startDate == dateTime) {
    return;
  }
  m_startDate = dateTime;
  updateData();
  emit startDateChanged();
}

int Calendar::types() const
{
  return m_types;
}

void Calendar::setTypes(int types)
{
  if (m_types == types)
    return;
  emit typesChanged();
}

int Calendar::weeks() const
{
  return m_weeks;
}

void Calendar::setWeeks(int weeks)
{
  if (m_weeks != weeks) {
    m_weeks = weeks;
    updateData();
    emit weeksChanged();
  }
}

int Calendar::startDay() const
{
  return m_startDay;
}

void Calendar::setStartDay(int day)
{
  if (day > 7 || day < 1)
    return;

  if (m_startDay != day) {
    m_startDay = day;
    emit startDayChanged();
  }
}

QString Calendar::errorMessage() const
{
  return m_errorMessage;
}

int Calendar::year() const
{
  return m_startDate.year();
}

QAbstractListModel* Calendar::daysModel() const
{
  return m_model;
}

QList<int> Calendar::weeksModel() const
{
  return m_weekList;
}

void Calendar::updateData()
{
  if (m_weeks == 0) {
    return;
  }

  m_dayList.clear();
  m_weekList.clear();

  int totalDays = 7 * m_weeks;

  int daysBeforeCurrentMonth = 0;
  int daysAfterCurrentMonth = 0;

  QDate firstDay(m_startDate.year(), m_startDate.month(), 1);

  daysBeforeCurrentMonth = firstDay.dayOfWeek();

  int daysThusFar = daysBeforeCurrentMonth + m_startDate.daysInMonth();
  if (daysThusFar < totalDays) {
    daysAfterCurrentMonth = totalDays - daysThusFar;
  }

  if (daysBeforeCurrentMonth > 0) {
    QDate previousMonth = m_startDate.addMonths(-1);

    for (int i = 0; i < daysBeforeCurrentMonth; ++i) {
      DayData day;
      day.isCurrentMonth = false;
      day.isNextMonth = false;
      day.isPreviousMonth = true;
      day.dayNumber = previousMonth.daysInMonth() -
          (daysBeforeCurrentMonth - (i + 1));
      day.monthNumber = previousMonth.month();
      day.yearNumber = previousMonth.year();
      day.containsEventItems = false;
      m_dayList << day;
    }
  }

  for (int i = 0; i < m_startDate.daysInMonth(); ++i) {
    DayData day;
    day.isCurrentMonth = true;
    day.isNextMonth = false;
    day.isPreviousMonth = false;
    day.dayNumber = i + 1;
    day.monthNumber = m_startDate.month();
    day.yearNumber = m_startDate.year();
    day.containsEventItems = false;
    m_dayList << day;
  }

  if (daysAfterCurrentMonth > 0) {
    for (int i = 0; i < daysAfterCurrentMonth; ++i) {
      DayData day;
      day.isCurrentMonth = false;
      day.isNextMonth = true;
      day.isPreviousMonth = false;
      day.dayNumber = i + 1;
      day.monthNumber = m_startDate.addMonths(1).month();
      day.yearNumber = m_startDate.addMonths(1).year();
      day.containsEventItems = false;
      m_dayList << day;
    }
  }

  const int numOfDaysInCalendar = m_dayList.count();

  for (int i = 0; i < numOfDaysInCalendar; i += 7) {
    const DayData& data = m_dayList.at(i);
    m_weekList <<
      QDate(data.yearNumber, data.monthNumber, data.dayNumber).weekNumber();
  }

  m_model->update();
}

void Calendar::nextMonth()
{
  m_startDate = m_startDate.addMonths(1);
  updateData();
  emit startDateChanged();
}

void Calendar::nextYear()
{
  m_startDate = m_startDate.addYears(1);
  updateData();
  emit startDateChanged();
}

void Calendar::previousYear()
{
  m_startDate = m_startDate.addYears(-1);
  updateData();
  emit startDateChanged();
}

void Calendar::previousMonth()
{
  m_startDate = m_startDate.addMonths(-1);
  updateData();
  emit startDateChanged();
}
