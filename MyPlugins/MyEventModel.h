#ifndef EVENTLISTUTILS_H
#define EVENTLISTUTILS_H

#include <QQuickItem>
#include <QDateTime>
#include <QQmlProperty>
#include <QtOrganizer/qorganizerglobal.h>
#include <QtVersitOrganizer/qversitorganizerglobal.h>
#include "MyEvent.h"

QTORGANIZER_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE_ORGANIZER
class QOrganizerManager;
QT_END_NAMESPACE_ORGANIZER

class MyEventModel : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate
             NOTIFY startDateChanged)
  Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
  Q_PROPERTY(QVariantList events READ events NOTIFY eventsChanged)

public:
  MyEventModel(QQuickItem* parent = 0);
  ~MyEventModel() {}

  QDate startDate() const { return m_start_date; }
  void setStartDate(const QDate& start_date);

  QDate endDate() const { return m_end_date; }
  void setEndDate(const QDate& end_date);

//  QList<QObject*> events() const { return m_events; }
  QVariantList events();

  Q_INVOKABLE void saveEvent(const MyEvent& event);

Q_SIGNALS:
  void startDateChanged(const QDate& date);
  void endDateChanged(const QDate& date);
  void eventsChanged();

private:
  void updateEvents();

  QOrganizerManager* m_manager;

  QDate m_start_date;
  QDate m_end_date;
  QVariantList m_events;
};

#endif // EVENTLISTUTILS_H
