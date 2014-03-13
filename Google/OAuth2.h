#ifndef OAUTH2_H
#define OAUTH2_H

#include <QObject>

class LoginDialog;

class OAuth2 : public QObject
{
  Q_OBJECT
public:
  explicit OAuth2(QWidget *parent = 0);
  QString accessToken();
  bool isAuthorized();
  void startLogin(bool force);

  QString loginUrl();

signals:
  void loginDone();

public slots:

private slots:
  void accessTokenObtained();

private:
  QString m_access_token;

  QString m_end_point;
  QString m_scope;
  QString m_client_id;
  QString m_redirect_uri;
  QString m_response_type;

  LoginDialog* m_login_dialog;
  QWidget* m_parent;
};

#endif // OAUTH2_H
