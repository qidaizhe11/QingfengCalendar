#include "GoogleSettings.h"
#include <QSettings>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

GoogleSettings::GoogleSettings(QObject *parent) :
  QObject(parent)
{
  QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope,
                     "qml/QingfengCalendar");

  m_company_name = "Daizhe";
  m_app_name = "QingfengCalendar";
  QSettings settings(QSettings::UserScope, m_company_name, m_app_name);
  m_access_token = settings.value("access_token", "").toString();
  m_refresh_token = settings.value("refresh_token", "").toString();
}

QVariant GoogleSettings::accessToken() const
{
  return m_access_token;
}

void GoogleSettings::setAccessToken(const QVariant &token)
{
  m_access_token = token.toString();
  QSettings settings(QSettings::UserScope, m_company_name, m_app_name);
  settings.setValue("access_token", m_access_token);
}

QVariant GoogleSettings::refreshToken() const
{
  return m_refresh_token;
}

void GoogleSettings::setRefreshToken(const QVariant &token)
{
  m_refresh_token = token.toString();
  QSettings settings(QSettings::UserScope, m_company_name, m_app_name);
  settings.setValue("refresh_token", m_refresh_token);
}

void GoogleSettings::openUrl(const QString &url)
{
  qDebug() << "GoogleSettings::openUrl " << url;
  QDesktopServices::openUrl(url);
}
