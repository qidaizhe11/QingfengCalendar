#ifndef GOOGLEMANAGER_H
#define GOOGLEMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <qorganizerglobal.h>
#include <QOrganizerManager>

QTORGANIZER_USE_NAMESPACE

//class QOrganizerManager;

class GoogleManager : public QObject
{
  Q_OBJECT
public:
  explicit GoogleManager(QObject *parent = 0);
  ~GoogleManager();

  void setOrganizerManager(QOrganizerManager* organizer_manager);

  void getCalendars();
  void getEventsForCalendar(const QString& cal_id);
  void newCalendar(const QString& access_token, const QString& cal_id);
//  void createEvent(const QString& access_token,)

//  QVariantList getCalendars();
//  QVariantList getEvents();

signals:
  void errorOccured(const QString& error);
  void calendarListReady();
  void eventsReady();
  void calendarListChanged();
  void eventChanged(const QString& id);

public slots:

private slots:
  void replyFinished(QNetworkReply* reply);

private:
  QNetworkAccessManager* m_network_access_manager;

  QOrganizerManager* m_organizer_manager;

  QVariantList m_calendars;
  QVariantList m_events;

  QString m_access_token;
};

#endif // GOOGLEMANAGER_H
