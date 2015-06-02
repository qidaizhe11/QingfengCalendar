#ifndef MYRECURRENCERULE_H
#define MYRECURRENCERULE_H

#include <QObject>
#include <QtQml/qqml.h>
#include <QtOrganizer/qorganizerrecurrencerule.h>

using namespace QtOrganizer;

class MyRecurrenceRule : public QObject
{
  Q_OBJECT
  Q_ENUMS(Frequency)
  Q_ENUMS(Month)
  Q_PROPERTY(Frequency frequency READ frequency WRITE setFrequency
             NOTIFY recurrenceRuleChanged)
  Q_PROPERTY(QVariant limit READ limit WRITE setLimit
             NOTIFY recurrenceRuleChanged)
  Q_PROPERTY(int interval READ interval WRITE setInterval
             NOTIFY recurrenceRuleChanged)
  Q_PROPERTY(QVariantList daysOfWeek READ daysOfWeek WRITE setDaysOfWeek
             NOTIFY recurrenceRuleChanged)
  Q_PROPERTY(QVariantList daysOfMonth READ daysOfMonth WRITE setDaysOfMonth
             NOTIFY recurrenceRuleChanged)
  Q_PROPERTY(QVariantList daysOfYear READ daysOfYear WRITE setDaysOfYear
             NOTIFY recurrenceRuleChanged)
  Q_PROPERTY(QVariantList monthsOfYear READ monthsOfYear WRITE setMonthsOfYear
             NOTIFY recurrenceRuleChanged)
  Q_PROPERTY(QVariantList positions READ positions WRITE setPositions
             NOTIFY recurrenceRuleChanged)
  Q_PROPERTY(Qt::DayOfWeek firstDayOfWeek READ firstDayOfWeek
             WRITE setFirstDayOfWeek NOTIFY recurrenceRuleChanged)
public:
  enum Frequency {
    Invalid = QOrganizerRecurrenceRule::Invalid,
    Daily = QOrganizerRecurrenceRule::Daily,
    Weekly = QOrganizerRecurrenceRule::Weekly,
    Monthly = QOrganizerRecurrenceRule::Monthly,
    Yearly = QOrganizerRecurrenceRule::Yearly
  };

  enum Month {
    January = QOrganizerRecurrenceRule::January,
    February = QOrganizerRecurrenceRule::February,
    March = QOrganizerRecurrenceRule::March,
    April = QOrganizerRecurrenceRule::April,
    May = QOrganizerRecurrenceRule::May,
    June = QOrganizerRecurrenceRule::June,
    July = QOrganizerRecurrenceRule::July,
    August = QOrganizerRecurrenceRule::August,
    September = QOrganizerRecurrenceRule::September,
    October = QOrganizerRecurrenceRule::October,
    November = QOrganizerRecurrenceRule::November,
    December = QOrganizerRecurrenceRule::December
  };

  MyRecurrenceRule(QObject *parent = 0);

  void setFrequency(Frequency freq);
  Frequency frequency() const;

  void setLimit(const QVariant &value);
  QVariant limit() const;

  void setInterval(int interval);
  int interval() const;

  void setDaysOfWeek(const QVariantList &days);
  QVariantList daysOfWeek() const;

  void setDaysOfMonth(const QVariantList &days);
  QVariantList daysOfMonth() const;

  void setDaysOfYear(const QVariantList &days);
  QVariantList daysOfYear() const;

  void setMonthsOfYear(const QVariantList &months);
  QVariantList monthsOfYear() const;

  void setWeeksOfYear(const QVariantList &weeks);
  QVariantList weeksOfYear() const;

  void setPositions(const QVariantList &pos);
  QVariantList positions() const;

  void setFirstDayOfWeek(Qt::DayOfWeek day);
  Qt::DayOfWeek firstDayOfWeek() const;

  QOrganizerRecurrenceRule toOrganizerRule() const;
  void setRule(const QOrganizerRecurrenceRule& rule);

Q_SIGNALS:
  void recurrenceRuleChanged();

private:
  QOrganizerRecurrenceRule m_rule;
};

QML_DECLARE_TYPE(MyRecurrenceRule)

#endif // MYRECURRENCERULE_H
