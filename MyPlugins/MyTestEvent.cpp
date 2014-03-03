#include "MyTestEvent.h"

MyTestEvent::MyTestEvent()
{
  m_start_datetime = QDateTime::currentDateTime();
  m_end_datetime = QDateTime::currentDateTime();
}
