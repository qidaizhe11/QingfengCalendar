
#include "qquickrangeddate_p.h"

// Javascript Date > QDate conversion is not correct for large negative dates.
Q_GLOBAL_STATIC_WITH_ARGS(const QDate, jsMinimumDate, (QDate(1, 1, 1)))
Q_GLOBAL_STATIC_WITH_ARGS(const QDate, jsMaximumDate, (QDate(275759, 10, 25)))

QQuickRangedDate::QQuickRangedDate() :
  QObject(0),
  mDate(QDate::currentDate()),
  mMinimumDate(*jsMinimumDate),
  mMaximumDate(*jsMaximumDate)
{
}

void QQuickRangedDate::setDate(const QDate &date)
{
  if (date == mDate) {
    return;
  }

  if (date < mMinimumDate) {
    mDate = mMinimumDate;
  } else if (date > mMaximumDate) {
    mDate = mMaximumDate;
  } else {
    mDate = date;
  }

  emit dateChanged();
}

void QQuickRangedDate::setMinimumDate(const QDate &minimumDate)
{
  if (minimumDate == mMinimumDate) {
    return;
  }

  mMinimumDate = qMax(mMinimumDate, *jsMinimumDate);
  emit minimumDateChanged();

  // if the new minimumDate makes date invalid, clamp date to it.
  if (mDate < mMinimumDate) {
    mDate = mMinimumDate;
    emit dateChanged();
  }
}

void QQuickRangedDate::setMaximumDate(const QDate &maximumDate)
{
  if (maximumDate == mMaximumDate) {
    return;
  }

  // If the new maximumDate is smaller than minimumDate, clamp maximumDate
  // to it.
  // If the new maximumDate is larger than jsMaximumDate, alse clamp it.
  mMaximumDate = mMaximumDate < mMinimumDate ? mMinimumDate :
                                     qMin(maximumDate, *jsMaximumDate);
  emit maximumDateChanged();

  // If the new maximumDate makes the date invalid, clamp it.
  if (mDate > mMaximumDate) {
    mDate = mMaximumDate;
    emit dateChanged();
  }
}
