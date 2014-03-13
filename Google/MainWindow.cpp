#include "MainWindow.h"
#include "OAuth2.h"

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent)
{
  resize(800, 600);
  m_oauth2 = new OAuth2(this);
//  connect(m_oauth2, SIGNAL(loginDone()), this,

  m_oauth2->startLogin(true);
}
