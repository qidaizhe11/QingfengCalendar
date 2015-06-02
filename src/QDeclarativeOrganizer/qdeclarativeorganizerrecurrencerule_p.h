/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEORGANIZERITEMRECURRENCERULE_H
#define QDECLARATIVEORGANIZERITEMRECURRENCERULE_H

#include <QtQml/qqml.h>

#include <QtOrganizer/qorganizerrecurrencerule.h>

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeOrganizerRecurrenceRule : public QObject
{
    Q_OBJECT
    Q_ENUMS(Frequency)
    Q_ENUMS(Month)
    Q_PROPERTY(Frequency frequency READ frequency WRITE setFrequency NOTIFY recurrenceRuleChanged)
    Q_PROPERTY(QVariant limit READ limit WRITE setLimit NOTIFY recurrenceRuleChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY recurrenceRuleChanged)
    Q_PROPERTY(QVariantList daysOfWeek READ daysOfWeek WRITE setDaysOfWeek NOTIFY recurrenceRuleChanged)
    Q_PROPERTY(QVariantList daysOfMonth READ daysOfMonth WRITE setDaysOfMonth NOTIFY recurrenceRuleChanged)
    Q_PROPERTY(QVariantList daysOfYear READ daysOfYear WRITE setDaysOfYear NOTIFY recurrenceRuleChanged)
    Q_PROPERTY(QVariantList monthsOfYear READ monthsOfYear WRITE setMonthsOfYear NOTIFY recurrenceRuleChanged)
    Q_PROPERTY(QVariantList positions READ positions WRITE setPositions NOTIFY recurrenceRuleChanged)
    Q_PROPERTY(Qt::DayOfWeek firstDayOfWeek READ firstDayOfWeek WRITE setFirstDayOfWeek NOTIFY recurrenceRuleChanged)

public:
    enum Frequency {
        Invalid = QOrganizerRecurrenceRule::Invalid,
        Daily = QOrganizerRecurrenceRule::Daily,
        Weekly = QOrganizerRecurrenceRule::Weekly,
        Monthly = QOrganizerRecurrenceRule::Monthly,
        Yearly = QOrganizerRecurrenceRule::Yearly
    };

    enum Month {
        January = QOrganizerRecurrenceRule::January,
        February = QOrganizerRecurrenceRule::February,
        March = QOrganizerRecurrenceRule::March,
        April = QOrganizerRecurrenceRule::April,
        May = QOrganizerRecurrenceRule::May,
        June = QOrganizerRecurrenceRule::June,
        July = QOrganizerRecurrenceRule::July,
        August = QOrganizerRecurrenceRule::August,
        September = QOrganizerRecurrenceRule::September,
        October = QOrganizerRecurrenceRule::October,
        November = QOrganizerRecurrenceRule::November,
        December = QOrganizerRecurrenceRule::December
    };

    QDeclarativeOrganizerRecurrenceRule(QObject *parent = 0);

    void setFrequency(Frequency freq);
    Frequency frequency() const;

    void setLimit(const QVariant &value);
    QVariant limit() const;

    void setInterval(int interval);
    int interval() const;

    void setDaysOfWeek(const QVariantList &days);
    QVariantList daysOfWeek() const;

    void setDaysOfMonth(const QVariantList &days);
    QVariantList daysOfMonth() const;

    void setDaysOfYear(const QVariantList &days);
    QVariantList daysOfYear() const;

    void setMonthsOfYear(const QVariantList &months);
    QVariantList monthsOfYear() const;

    void setWeeksOfYear(const QVariantList &weeks);
    QVariantList weeksOfYear() const;

    void setPositions(const QVariantList &pos);
    QVariantList positions() const;

    void setFirstDayOfWeek(Qt::DayOfWeek day);
    Qt::DayOfWeek firstDayOfWeek() const;

    // used by recurrence detail
    QOrganizerRecurrenceRule rule() const;
    void setRule(const QOrganizerRecurrenceRule &rule);

Q_SIGNALS:
    void recurrenceRuleChanged();

private:
    QOrganizerRecurrenceRule m_rule;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeOrganizerRecurrenceRule)

#endif // QDECLARATIVEORGANIZERITEMRECURRENCERULE_H
