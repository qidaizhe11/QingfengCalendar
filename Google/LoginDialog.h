#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QUrl>

class QWebView;
class QDialogButtonBox;

class LoginDialog : public QDialog
{
  Q_OBJECT
public:
  explicit LoginDialog(QWidget *parent = 0);
  ~LoginDialog() {}

  void setLoginUrl(const QString& url);
  QString accessToken() const;

signals:
  void accessTokenObtained();

public slots:

private slots:
  void urlChanged(const QUrl& url);
  void loadStarted();
  void loadFinished(bool);

private:
  QString m_access_token;

  QWebView* web_view;
  QDialogButtonBox* button_box;
};

#endif // LOGINDIALOG_H
