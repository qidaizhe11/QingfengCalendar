#include "OAuth2.h"
#include <QApplication>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include "LoginDialog.h"

OAuth2::OAuth2(QWidget *parent)
{
  m_end_point = "https://accounts.google.com/o/oauth2/auth";
  m_scope = "https://www.googleapis.com/auth/calendar";
  m_client_id = "135859676517-padra6sp9cd3ahqs0rs1j5l508e05nu1.apps.googleusercontent.com";
  m_redirect_uri = "http://localhost";
  m_response_type = "code";

  m_login_dialog = new LoginDialog(parent);
  m_parent = parent;
  connect(m_login_dialog, SIGNAL(accessTokenObtained()), this,
          SLOT(accessTokenObtained()));
}

void OAuth2::accessTokenObtained()
{
  QSettings settings("Daizhe", "Qingfeng Calendar");
  m_access_token = m_login_dialog->accessToken();
  settings.setValue("access_token", m_access_token);
  m_login_dialog->setLoginUrl("");
  emit loginDone();
}

QString OAuth2::loginUrl()
{
  QString str = QString(
        "%1?client_id=%2&redirect_uri=%3&response_type=%4&scope=%5")
      .arg(m_end_point).arg(m_client_id).arg(m_redirect_uri)
      .arg(m_response_type).arg(m_scope);
  qDebug() << "Login URL " << str;
  return str;
}

QString OAuth2::accessToken()
{
  return m_access_token;
}

bool OAuth2::isAuthorized()
{
  return m_access_token != "";
}

void OAuth2::startLogin(bool force)
{
  qDebug() << "OAuth2::startLogin";
  QSettings settings("Daizhe", "Qingfeng Calendar");
  QString str = settings.value("access_token", "").toString();

  qDebug() << "OAuth2::startLogin, token from Settings: " << str;

  if (str.isEmpty() || force) {
    m_login_dialog->setLoginUrl(loginUrl());
    m_login_dialog->show();
  } else {
    m_access_token = str;
    emit loginDone();
  }
}
