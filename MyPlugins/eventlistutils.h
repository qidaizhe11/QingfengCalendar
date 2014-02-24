#ifndef EVENTLISTUTILS_H
#define EVENTLISTUTILS_H

#include <QQuickItem>
#include <QDateTime>
#include <QtOrganizer/qorganizerglobal.h>

QTORGANIZER_USE_NAMESPACE

class EventListUtils : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate
             NOTIFY startDateChanged)
  Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
  Q_PROPERTY(QList<QVariant> events READ events NOTIFY eventsChanged)

public:
  EventListUtils(QQuickItem* parent = 0);
  ~EventListUtils() {}

  QDate startDate() const { return m_start_date; }
  void setStartDate(const QDate& start_date);

  QDate endDate() const { return m_end_date; }
  void setEndDate(const QDate& end_date);

  QList<QVariant> events() const;

private:
  QDate m_start_date;
  QDate m_end_date;
  QVariantList m_events;
};

#endif // EVENTLISTUTILS_H
