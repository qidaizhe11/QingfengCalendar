#ifndef LOGINMODULE_H
#define LOGINMODULE_H

//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
#include <QNetworkReply>

#include <QObject>
#include <QMap>

#include "network_global.h"

class QNetworkAccessManager;
class QNetworkReply;

class NETWORKSHARED_EXPORT LoginModule : public QObject
{
    Q_OBJECT
public:
    enum RequestType {
        GetCalendarList,
        GetEventsOfCalendar,
        FreshStartGetCalendars
    };

    explicit LoginModule(QObject* parent = 0);
    ~LoginModule();

    void ConnectLoginServer();

private slots:
    void replyFinished(QNetworkReply* reply);
    void slotError(QNetworkReply::NetworkError error_code);
    void slotSslErrors(const QList<QSslError> & errors);

private:
    QNetworkAccessManager* m_network_access_manager;

    QMap<QNetworkReply*, RequestType> m_reply_map;
};

#endif // LOGINMODULE_H
