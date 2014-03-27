#include "MyRecurrenceRule.h"
#include <QtQml/qqmlinfo.h>

MyRecurrenceRule::MyRecurrenceRule(QObject *parent) :
  QObject(parent)
{
}

void MyRecurrenceRule::setFrequency(Frequency freq)
{
  if (freq != frequency()) {
    m_rule.setFrequency(static_cast<MyRecurrenceRule::Frequency>(freq));
    emit recurrenceRuleChanged();
  }
}

MyRecurrenceRule::Frequency MyRecurrenceRule::frequency() const
{
  return static_cast<Frequency>(m_rule.frequency());
}

void MyRecurrenceRule::setLimit(const QVariant &value)
{
  if (!value.isValid()) {
    if (m_rule.limitType() != QOrganizerRecurrenceRule::NoLimit) {
      m_rule.clearLimit();
      emit recurrenceRuleChanged();
    }
  } else if (value.type() == QVariant::DateTime) {
    QDate v = value.toDateTime().toUTC().date();
    if (v != m_rule.limitDate()) {
      m_rule.setLimit(v);
      emit recurrenceRuleChanged();
    }
  } else if (value.type() == QVariant::Date) {
    QDate v = value.value<QDate>();
    if (v != m_rule.limitDate()) {
      m_rule.setLimit(v);
      emit recurrenceRuleChanged();
    }
  } else if ((value.type() == QVariant::Int) ||
             (value.type() == QVariant::Double)) {
    int v = value.value<int>();
    if (v != m_rule.limitCount()) {
      m_rule.setLimit(v);
      emit recurrenceRuleChanged();
    }
  } else {
    qmlInfo(this) << tr("Invalid recurrence rule limit; value ,") << value <<
                     tr(", did not match one of the type: date, integer or double");
  }
}

QVariant MyRecurrenceRule::limit() const
{
  if (m_rule.limitType() == QOrganizerRecurrenceRule::CountLimit) {
    return QVariant::fromValue(m_rule.limitCount());
  } else if (m_rule.limitType() == QOrganizerRecurrenceRule::DateLimit) {
    return QVariant::fromValue(m_rule.limitDate());
  }

  return QVariant();
}

void MyRecurrenceRule::setInterval(int interval)
{
  if (interval != m_rule.interval()) {
    m_rule.setInterval(interval);
    emit recurrenceRuleChanged();
  }
}

int MyRecurrenceRule::interval() const
{
  return m_rule.interval();
}

void MyRecurrenceRule::setDaysOfWeek(const QVariantList &days)
{
  QSet<Qt::DayOfWeek> saved;
  foreach (const QVariant& day, days) {
    saved << static_cast<Qt::DayOfWeek>(day.value<int>());
  }
  if (saved != m_rule.daysOfWeek()) {
    m_rule.setDaysOfWeek(saved);
    emit recurrenceRuleChanged();
  }
}

QVariantList MyRecurrenceRule::daysOfWeek() const
{
  QVariantList days;
  foreach (Qt::DayOfWeek day, m_rule.daysOfWeek()) {
    days << day;
  }
  return days;
}

void MyRecurrenceRule::setDaysOfMonth(const QVariantList &days)
{
  QSet<int> saved;
  foreach (const QVariant& day, days) {
    saved << day.value<int>();
  }
  if (saved != m_rule.daysOfMonth()) {
    m_rule.setDaysOfMonth(saved);
    emit recurrenceRuleChanged();
  }
}

QVariantList MyRecurrenceRule::daysOfMonth() const
{
  QVariantList days;
  foreach (int day, m_rule.daysOfMonth()) {
    days << day;
  }
  return days;
}

/*!
    \qmlproperty list<variant> RecurrenceRule::daysOfYear

    This property holds a list of the days of the year that the item should recur on.
 */
void MyRecurrenceRule::setDaysOfYear(const QVariantList &days)
{
    QSet<int> saved;
    foreach (const QVariant &day, days)
        saved << day.value<int>();
    if (saved != m_rule.daysOfYear()) {
        m_rule.setDaysOfYear(saved);
        emit recurrenceRuleChanged();
    }
}

QVariantList MyRecurrenceRule::daysOfYear() const
{
    QVariantList days;
    foreach (int day, m_rule.daysOfYear())
        days << day;
    return days;
}

/*!
    \qmlproperty list<int> RecurrenceRule::monthsOfYear

    This property holds a list of the months that the item should recur on, the list element value can be one of:
    \list
    \li RecurrenceRule.January
    \li RecurrenceRule.February
    \li RecurrenceRule.March
    \li RecurrenceRule.April
    \li RecurrenceRule.May
    \li RecurrenceRule.June
    \li RecurrenceRule.July
    \li RecurrenceRule.August
    \li RecurrenceRule.September
    \li RecurrenceRule.October
    \li RecurrenceRule.November
    \li RecurrenceRule.December
    \endlist
 */
void MyRecurrenceRule::setMonthsOfYear(const QVariantList &months)
{
    QSet<QOrganizerRecurrenceRule::Month> saved;
    foreach (const QVariant &day, months)
        saved << static_cast<QOrganizerRecurrenceRule::Month>(day.value<int>());
    if (saved != m_rule.monthsOfYear()) {
        m_rule.setMonthsOfYear(saved);
        emit recurrenceRuleChanged();
    }
}

QVariantList MyRecurrenceRule::monthsOfYear() const
{
    QVariantList ms;
    foreach (int m, m_rule.monthsOfYear())
        ms << m;
    return ms;
}

/*!
  \qmlproperty list<int> RecurrenceRule::weeksOfYear

  This property holds a list of the weeks of the year that the item should recur on.
  */
void MyRecurrenceRule::setWeeksOfYear(const QVariantList &weeks)
{
    QSet<int> saved;
    foreach (const QVariant &week, weeks)
        saved << week.value<int>();
    if (saved != m_rule.weeksOfYear()) {
        m_rule.setWeeksOfYear(saved);
        emit recurrenceRuleChanged();
    }
}

QVariantList MyRecurrenceRule::weeksOfYear() const
{
    QVariantList weeks;
    foreach (int week, m_rule.weeksOfYear())
        weeks << week;
    return weeks;
}

void MyRecurrenceRule::setPositions(const QVariantList &pos)
{
  if (pos != positions()) {
    QSet<int> saved;
    foreach (const QVariant& p, pos) {
      saved << p.value<int>();
    }
    m_rule.setPositions(saved);
    emit recurrenceRuleChanged();
  }
}

QVariantList MyRecurrenceRule::positions() const
{
  QVariantList pos;
  foreach (int p, m_rule.positions()) {
    pos << p;
  }
  return pos;
}

void MyRecurrenceRule::setFirstDayOfWeek(Qt::DayOfWeek day)
{
  if (day != firstDayOfWeek()) {
    m_rule.setFirstDayOfWeek(day);
    emit recurrenceRuleChanged();
  }
}

Qt::DayOfWeek MyRecurrenceRule::firstDayOfWeek() const
{
  return m_rule.firstDayOfWeek();
}

QOrganizerRecurrenceRule MyRecurrenceRule::toOrganizerRule() const
{
  return m_rule;
}

void MyRecurrenceRule::setRule(const QOrganizerRecurrenceRule &rule)
{
  m_rule = rule;
}
