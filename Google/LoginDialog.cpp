#include "LoginDialog.h"

#include <QtWebKitWidgets/QWebView>
#include <QtWidgets>

LoginDialog::LoginDialog(QWidget *parent) :
  QDialog(parent)
{
  web_view = new QWebView();
  button_box = new QDialogButtonBox(QDialogButtonBox::Close);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(web_view);
  layout->addWidget(button_box);
  setLayout(layout);

  connect(button_box, SIGNAL(accepted()), this, SLOT(accept()));
  connect(button_box, SIGNAL(rejected()), this, SLOT(reject()));

  connect(web_view, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
  connect(web_view, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
  connect(web_view, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
}

void LoginDialog::loadStarted()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  qDebug() << "load started.";
}

void LoginDialog::loadFinished(bool is_load_success)
{
  QApplication::restoreOverrideCursor();
  qDebug() << "loaad finished with " << is_load_success;
}

void LoginDialog::urlChanged(const QUrl &url)
{
  qDebug() << "URL = " << url;
  QString str = url.toString();
  if (str.indexOf("access_token") != -1) {
    QStringList query = str.split("#");
    QStringList list = query[1].split("&");

    for (int i = 0; i < list.count(); ++i) {
      QStringList pair = list[i].split("=");
      if (pair[0] == "access_token") {
        m_access_token = pair[1];
        emit accessTokenObtained();
        QDialog::accept();
      }
    }
  }
}

QString LoginDialog::accessToken() const
{
  return m_access_token;
}

void LoginDialog::setLoginUrl(const QString &url)
{
  web_view->setUrl(QUrl(url));
}
