
#include "LoginModule.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

LoginModule::LoginModule(QObject* parent)
    : QObject(parent)
    , m_network_access_manager(0)
{
    m_network_access_manager = new QNetworkAccessManager(this);
    connect(m_network_access_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

LoginModule::~LoginModule()
{
    delete m_network_access_manager;
}

void LoginModule::ConnectLoginServer()
{
    QString url_str = QString("http://access.daizhe.xyz:8080/msg_server");

    QNetworkReply* reply = m_network_access_manager->get(QNetworkRequest(QUrl(url_str)));
    reply->ignoreSslErrors();
//    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
}

void LoginModule::replyFinished(QNetworkReply *reply)
{
    QString reply_json = reply->readAll();
    QString reply_url = QUrl::fromPercentEncoding(reply->url().toEncoded());

    qDebug() << "Reply = " << reply_json;
    qDebug() << "URL = " << reply_url;
}

void LoginModule::slotError(QNetworkReply::NetworkError error_code)
{
    qDebug() << "Reply Error, error_code: " << error_code;
}

void LoginModule::slotSslErrors(const QList<QSslError> &errors)
{
    foreach (QSslError error, errors) {
        qDebug() << "QSslError: " << error.errorString();
    }
}

