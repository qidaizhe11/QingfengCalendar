#include "GoogleManager.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QOrganizerManager>
#include "GoogleSettings.h"

GoogleManager::GoogleManager(QObject *parent) :
  QObject(parent),
  m_organizer_manager(0)
{
  GoogleSettings settings;
  m_access_token = settings.accessToken().toString();

  m_network_access_manager = new QNetworkAccessManager(this);
  connect(m_network_access_manager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(replyFinished(QNetworkReply*)));
}

GoogleManager::~GoogleManager()
{
  delete m_network_access_manager;
}

void GoogleManager::setOrganizerManager(QOrganizerManager *organizer_manager)
{
  m_organizer_manager = organizer_manager;
}

void GoogleManager::replyFinished(QNetworkReply *reply)
{
  QString json = reply->readAll();
  qDebug() << "Reply = " << json;
  qDebug() << "URL = " << reply->url();
  QString url_str = reply->url().toString();

  if (json.isEmpty()) {
    qDebug() << "Empty answer.";
  }

  QJsonParseError error;
  QJsonDocument json_document = QJsonDocument::fromJson(json.toUtf8(), &error);
  if (error.error == QJsonParseError::NoError) {
    if (json_document.isObject()) {
      QVariantMap result = json_document.toVariant().toMap();
      qDebug() << "encoding:" << result["encoding"].toString();
      qDebug() << "plugins:";

      if (result["kind"].toString() == "calendar#calendarList") {
        m_calendars = result["items"].toList();
        emit calendarListReady();
      } else if (result["kind"].toString() == "calendar#calendar") {
        emit calendarListChanged();
      } else if (result["kind"].toString() == "calendar#events") {
        m_events = result["items"].toList();
        emit eventsReady();
      } else if (result["kind"].toString() == "calendar#event") {
        emit eventChanged(result["id"].toString());
      }

      qDebug() << "Reply finished.";
      qDebug() << m_calendars;
      qDebug() << m_events;
    }
  } else {
    qDebug() << error.errorString();
  }
}

void GoogleManager::getCalendars()
{
  qDebug() << Q_FUNC_INFO;
  QString str = QString("https://www.googleapis.com/calendar/v3/users/me/"
                        "calendarList?access_token=%1").arg(m_access_token);
  m_network_access_manager->get(QNetworkRequest(QUrl(str)));
}

void GoogleManager::getEventsForCalendar(const QString &cal_id)
{
  Q_UNUSED(cal_id);
  QString m_cal_id = "qidaizhe11@gmail.com";
  qDebug() << Q_FUNC_INFO;
  QString str = QString("https://www.googleapis.com/calendar/v3/calendars/%1/"
                        "events").arg(m_cal_id);

  QUrl url;
  url.setUrl(QUrl::toPercentEncoding(str, "/:"));

  QNetworkRequest request;
  request.setUrl(url);
  request.setRawHeader("Authorization", QString("OAuth %1").arg(m_access_token)
                       .toLatin1());
  request.setRawHeader("X-Javascript-User-Agent", "Google APIs Explorer");
  m_network_access_manager->get(request);
}
