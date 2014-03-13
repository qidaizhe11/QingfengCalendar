#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class OAuth2;

class MainWindow : public QWidget
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private:
  OAuth2* m_oauth2;
};

#endif // MAINWINDOW_H
