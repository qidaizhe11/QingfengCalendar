#ifndef GOOGLEMANAGER_H
#define GOOGLEMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <qorganizerglobal.h>
#include <QOrganizerManager>
#include <QMap>

QTORGANIZER_USE_NAMESPACE

class MyEvent;
class MyCollection;

class GoogleManager : public QObject
{
  Q_OBJECT
public:
  enum RequestType {
    GetCalendarList,
    GetEventsOfCalendar,
    FreshStartGetCalendars
  };

  explicit GoogleManager(QObject *parent = 0);
  ~GoogleManager();

  void setOrganizerManager(QOrganizerManager* organizer_manager);

  void getCalendarList();
  void getEventsOfCalendar(const QString& cal_id);
//  void newCalendar(const QString& cal_id);
//  void createEvent(const QString& access_token,)

  void parseEvent(QVariant event_var, MyEvent* out_event);
  void parseCalendar(QVariant calendar_var, MyCollection* out_collection);

signals:
  void errorOccured(const QString& error);
  void eventsReady(const QString& cal_id, QVariantList events);
  void calendarListReady(QVariantList calendars);

public slots:
  void freshStartSync();

private slots:
  void replyFinished(QNetworkReply* reply);

private:

  void initCalendarListDone(QVariantList calendars);
//  void initEventsDone(const QString& cal_id, QVariantList events);

  QNetworkAccessManager* m_network_access_manager;

  QOrganizerManager* m_organizer_manager;

  QMap<QNetworkReply*, RequestType> reply_map;

//  QVariantList m_calendars;
//  QStringList m_cal_ids;
//  QVariantList m_events;

  QString m_access_token;
};

#endif // GOOGLEMANAGER_H