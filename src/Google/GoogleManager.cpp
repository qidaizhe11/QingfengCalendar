#include "GoogleManager.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QOrganizerManager>
#include "GoogleSettings.h"
//#include "MyPlugins/MyEvent.h"
//#include "MyPlugins/MyCollection.h"
#include "qdeclarativeorganizeritem_p.h"
#include "qdeclarativeorganizercollection_p.h"

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

GoogleManager::GoogleManager(QQuickItem *parent) :
    QQuickItem(parent),
    m_network_access_manager(0)
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

//-------------------------------------------------------------------------
// public

//
// replyFinished
//

void GoogleManager::replyFinished(QNetworkReply *reply)
{
    QString reply_json = reply->readAll();
    QString reply_url = QUrl::fromPercentEncoding(reply->url().toEncoded());

    //  qDebug() << "Reply = " << reply_json;
    qDebug() << "URL = " << reply_url;

    if (reply_json.isEmpty()) {
        qDebug() << "Empty answer";

        QRegExp reg_exp("calendars/*/events", Qt::CaseSensitive, QRegExp::Wildcard);
        if (reply_url.indexOf(reg_exp) != -1) {
            //      emit eventChanged();
        } else {
            //      emit calendarListChanged();
        }

        return;
    }

    RequestType request_type = reply_map.value(reply);

    QJsonParseError error;
    QJsonDocument json_document =
            QJsonDocument::fromJson(reply_json.toUtf8(), &error);

    switch (request_type) {
    case GetCalendarList: {
        if (error.error == QJsonParseError::NoError) {
            if (!(json_document.isNull() || json_document.isEmpty()) &&
                    json_document.isObject()) {
                QVariantMap result = json_document.toVariant().toMap();

                QVariantList calendars = result["items"].toList();
                emit calendarListReady(calendars);
            }
        } else {
            qDebug() << error.errorString();
        }
        break;
    }

    case GetEventsOfCalendar: {
        //    qDebug() << "Reply = " << reply_json;
        if (error.error == QJsonParseError::NoError) {
            if (!(json_document.isNull() || json_document.isEmpty()) &&
                    json_document.isObject()) {
                QVariantMap result = json_document.toVariant().toMap();

                QRegExp reg_exp("calendars/*/events", Qt::CaseSensitive, QRegExp::Wildcard);

                if (reply_url.contains(reg_exp)) {
                    QString cap_str = reg_exp.cap();
                    QStringList list = cap_str.split('/');

                    if (!list.at(1).isNull() && !list.at(1).isEmpty()) {
                        QString calendar_id = list.at(1);
                        qDebug() << "calendar_id:" << calendar_id;
                        QVariantList events = result["items"].toList();

                        emit eventsReady(calendar_id, events);
                    }
                }
            }
        } else {
            qDebug() << "Error:" << error.errorString();
        }
        break;
    }

    case FreshStartGetCalendars: {
        if (error.error == QJsonParseError::NoError) {
            if (!(json_document.isNull() || json_document.isEmpty()) &&
                    json_document.isObject()) {
                QVariantMap result = json_document.toVariant().toMap();

                QVariantList calendars = result["items"].toList();
                initCalendarListDone(calendars);
            }
        } else {
            qDebug() << error.errorString();
        }
        break;
    }
    }

    reply->deleteLater();
}

//
// getCalendarList
//

void GoogleManager::getCalendarList()
{
    //  GoogleSettings settings;
    //  m_access_token = settings.accessToken().toString();

    qDebug() << Q_FUNC_INFO;
    QString str = QString("https://www.googleapis.com/calendar/v3/users/me/"
                          "calendarList?access_token=%1").arg(m_access_token);
    QNetworkReply* reply =
            m_network_access_manager->get(QNetworkRequest(QUrl(str)));
    reply_map.insert(reply, GetCalendarList);
}

//
// getEventsForCalendar
//

void GoogleManager::getEventsOfCalendar(const QString &cal_id)
{
    //  GoogleSettings settings;
    //  m_access_token = settings.accessToken().toString();

    QString str = QString("https://www.googleapis.com/calendar/v3/calendars/%1/"
                          "events").arg(cal_id);

    QUrl url;
    url.setUrl(QUrl::toPercentEncoding(str, "/:"));

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", QString("OAuth %1").arg(m_access_token)
                         .toLatin1());
    request.setRawHeader("X-Javascript-User-Agent", "Google APIs Explorer");

    QNetworkReply* reply = m_network_access_manager->get(request);
    reply_map.insert(reply, GetEventsOfCalendar);
}

//
// parseEvent
//
void GoogleManager::parseEvent(QVariant event_var, QDeclarativeOrganizerEvent *out_event)
{
    if (out_event == NULL) {
        return;
    }

    QVariantMap event = event_var.toMap();

    QDateTime dt_start;
    QDateTime dt_end;
    bool all_day = false;
    QVariantMap start_var = event.value("start").toMap();
    if (start_var.contains("dateTime")) {
        dt_start = QDateTime::fromString(
                    start_var.value("dateTime").toString(), Qt::ISODate);
        all_day = false;
    } else if (start_var.contains("date")) {
        dt_start = QDateTime::fromString(
                    start_var.value("date").toString(), Qt::ISODate);
        all_day = true;
    }

    if (dt_start.isValid()) {
        out_event->setAllDay(all_day);

        QVariantMap end_var = event.value("end").toMap();
        if (end_var.contains("dateTime")) {
            dt_end = QDateTime::fromString(
                        end_var.value("dateTime").toString(), Qt::ISODate);
        } else if (end_var.contains("date")) {
            dt_end = QDateTime::fromString(
                        end_var.value("date").toString(), Qt::ISODate);
        }
        dt_start = dt_start.toLocalTime();
        dt_end = dt_end.toLocalTime();
        out_event->setStartDateTime(dt_start);
        out_event->setEndDateTime(dt_end);

        out_event->setDisplayLabel(event.value("summary").toString());
        out_event->setLocation(event.value("location").toString());

        out_event->setGuid(event.value("iCalUID").toString());

        qDebug() << "OutEvent:" << out_event->displayLabel() <<
                    out_event->startDateTime().toString("yyyy-MM-d hh:mm") <<
                    " End:" <<
                    out_event->endDateTime().toString("yyyy-MM-d hh:mm") <<
                    out_event->collectionId() <<
                    out_event->guid() <<
                    out_event->isAllDay() <<
                    out_event->location();
    }
}

//
// parseCalendar
//
void GoogleManager::parseCalendar(QVariant calendar_var,
                                  QDeclarativeOrganizerCollection *out_collection)
{
    QVariantMap calendar = calendar_var.toMap();

    if (!calendar.value("id").toString().isEmpty()) {
        if (out_collection == NULL) {
            return;
        }

        out_collection->setExtendedId(calendar.value("id").toString());
        out_collection->setName(calendar.value("id").toString());
        out_collection->setDescription(calendar.value("summary").toString());
        out_collection->setColor(QColor(calendar.value("backgroundColor").toString()));
        out_collection->setSecondaryColor(QColor(calendar.value("foregroundColor").toString()));
        out_collection->setStorage("Google");
        out_collection->setAccessRole(calendar.value("accessRole").toString());

        qDebug() << "OutCalendar:" <<
                    out_collection->extendedId() <<
                    out_collection->name() <<
                    out_collection->description() <<
                    out_collection->color().name() <<
                    out_collection->storage() <<
                    out_collection->accessRole();
    }
}

//-------------------------------------------------------------------------
// public slots

void GoogleManager::freshStartSync()
{
    GoogleSettings settings;
    m_access_token = settings.accessToken().toString();

    QString str = QString("https://www.googleapis.com/calendar/v3/users/me/"
                          "calendarList?access_token=%1").arg(m_access_token);
    QNetworkReply* reply = m_network_access_manager->get(QNetworkRequest(QUrl(str)));
    reply_map.insert(reply, FreshStartGetCalendars);
}

//-------------------------------------------------------------------------
// private

void GoogleManager::initCalendarListDone(QVariantList calendars)
{
    emit calendarListReady(calendars);

    foreach (QVariant calendar_var, calendars) {
        QVariantMap calendar = calendar_var.toMap();
        QString calendar_id = calendar.value("id").toString();

        if (!calendar_id.isEmpty()) {
            getEventsOfCalendar(calendar_id);
        }
    }
}
