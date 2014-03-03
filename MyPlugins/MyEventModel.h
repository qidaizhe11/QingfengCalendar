#ifndef EVENTLISTUTILS_H
#define EVENTLISTUTILS_H

#include <QQuickItem>
#include <QDateTime>
#include <QQmlProperty>
#include <QtOrganizer/QOrganizerManager>
#include <QtOrganizer/qorganizerglobal.h>
#include <QtOrganizer/qorganizerabstractrequest.h>
#include <QtVersitOrganizer/qversitorganizerglobal.h>
#include "MyEvent.h"

QTORGANIZER_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE_ORGANIZER
//class QOrganizerManager;
QT_END_NAMESPACE_ORGANIZER

class MyEventModel : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate
             NOTIFY startDateChanged)
  Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
  Q_PROPERTY(QVariantList events READ events NOTIFY eventsChanged)
  Q_PROPERTY(QString error READ error NOTIFY errorChanged)

public:
  MyEventModel(QQuickItem* parent = 0);
  ~MyEventModel() {}

  QDateTime startDate() const { return m_start_date; }
  void setStartDate(const QDateTime& start_date);

  QDateTime endDate() const { return m_end_date; }
  void setEndDate(const QDateTime& end_date);

//  QList<QObject*> events() const { return m_events; }
  QVariantList events() const;

  QString error() const;

  Q_INVOKABLE void saveEvent(MyEvent* my_event);

Q_SIGNALS:
  void startDateChanged();
  void endDateChanged();
  void eventsChanged();
  void errorChanged();

public slots:
  void updateEvents();

private slots:
  void onRequestStateChanged(QOrganizerAbstractRequest::State new_state);

private:
  void checkError(const QOrganizerAbstractRequest* request);

  QOrganizerManager* m_manager;

  QDateTime m_start_date;
  QDateTime m_end_date;
  QVariantList m_events;
  QOrganizerManager::Error m_error;
};

#endif // EVENTLISTUTILS_H
