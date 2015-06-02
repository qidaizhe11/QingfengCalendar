#include "MyWeekModel.h"

namespace {
static const int daysInAWeek = 7;
}

MyWeekModel::MyWeekModel(QObject *parent) :
  QAbstractListModel(parent)
{
}

QDate MyWeekModel::visibleDate() const
{
  return mVisibleDate;
}

void MyWeekModel::setVisibleDate(const QDate &visibleDate)
{
  if (visibleDate != mVisibleDate && visibleDate.isValid()) {
    const QDate previousDate = mVisibleDate;
    mVisibleDate = visibleDate;
    populateFromVisibleDate(previousDate);
    emit visibleDateChanged(visibleDate);
  }
}

QLocale MyWeekModel::locale() const
{
  return mLocale;
}

void MyWeekModel::setLocale(const QLocale &locale)
{
  if (locale != mLocale) {
    Qt::DayOfWeek previousFirstDayOfWeek = mLocale.firstDayOfWeek();
    mLocale = locale;
    emit localeChanged(mLocale);
    if (mLocale.firstDayOfWeek() != previousFirstDayOfWeek) {
      populateFromVisibleDate(mVisibleDate, true);
    }
  }
}

QVariant MyWeekModel::data(const QModelIndex &index, int role) const
{
  if (role == DateRole) {
    return mVisibleDates.at(index.row());
  }
  return QVariant();
}

int MyWeekModel::rowCount(const QModelIndex &/*parent*/) const
{
  return mVisibleDates.isEmpty() ? 0 : daysInAWeek;
}

QHash<int, QByteArray> MyWeekModel::roleNames() const
{
  QHash<int, QByteArray> names;
  names[DateRole] = QByteArrayLiteral("date");
  return names;
}

QDate MyWeekModel::dateAt(int index) const
{
  return index >= 0 && index < mVisibleDates.size() ? mVisibleDates.at(index) :
                                                      QDate();
}

//
// Returns the index for @date, or -1 if @date is outside of our range.
//
int MyWeekModel::indexAt(const QDate &date)
{
  if (mVisibleDates.size() == 0 || date < mFirstVisibleDate ||
      date > mLastVisibleDate) {
    return -1;
  }

  return qMax(qint64(0), mFirstVisibleDate.daysTo(date));
}

void MyWeekModel::populateFromVisibleDate(const QDate &previousDate,
                                                  bool force)
{
  if (!force && mVisibleDate.year() == previousDate.year() &&
      mVisibleDate.month() == previousDate.month() &&
      mVisibleDate.weekNumber() == previousDate.weekNumber()) {
    return;
  }

  bool isEmpty = mVisibleDates.isEmpty();
  if (isEmpty) {
    beginResetModel();
    mVisibleDates.fill(QDate(), daysInAWeek);
  }

//  int difference = ((mVisibleDate.dayOfWeek() -
//                     mLocale.firstDayOfWeek()) + 7) % 7;
  int difference = ((mVisibleDate.dayOfWeek() - 1 + 7) % 7);

  mFirstVisibleDate = mVisibleDate.addDays(-difference);
  for (int i = 0; i < daysInAWeek; ++i) {
    mVisibleDates[i] = mFirstVisibleDate.addDays(i);
  }

  mLastVisibleDate = mVisibleDates.at(mVisibleDates.size() - 1);

  if (!isEmpty) {
    emit dataChanged(index(0, 0), index(rowCount() - 1, 0));
  } else {
    endResetModel();
    emit countChanged(rowCount());
  }
}
