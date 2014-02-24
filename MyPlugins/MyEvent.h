#ifndef MYEVENT_H
#define MYEVENT_H

#include <QObject>

class MyEvent : public QObject
{
  Q_OBJECT
public:
  explicit MyEvent(QObject *parent = 0);

signals:

public slots:

};

#endif // MYEVENT_H
