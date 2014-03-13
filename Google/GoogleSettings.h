#ifndef GOOGLESETTINGS_H
#define GOOGLESETTINGS_H

#include <QObject>
#include <QtQml/qqml.h>

class GoogleSettings : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QVariant accessToken READ accessToken WRITE setAccessToken
             NOTIFY accessTokenChanged)
  Q_PROPERTY(QVariant refreshToken READ refreshToken WRITE setRefreshToken
             NOTIFY refreshTokenChanged)
public:
  explicit GoogleSettings(QObject *parent = 0);

  QVariant accessToken() const;
  void setAccessToken(const QVariant& token);

  QVariant refreshToken() const;
  void setRefreshToken(const QVariant& token);

  Q_INVOKABLE void openUrl(const QString& url);

Q_SIGNALS:
  void accessTokenChanged();
  void refreshTokenChanged();

public slots:

private:
  QString m_access_token;
  QString m_refresh_token;
  QString m_company_name;
  QString m_app_name;
};

QML_DECLARE_TYPE(GoogleSettings)

#endif // GOOGLESETTINGS_H
