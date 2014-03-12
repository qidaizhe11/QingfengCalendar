/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtVersitOrganizer module of the Qt Toolkit.
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

#include "qtimezones_p.h"

#include <QtOrganizer/qorganizer.h>

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSITORGANIZER

QOrganizerManager* TimeZone::getManager()
{
    // We use the memory engine to do time zone recurrence calculations
    static QOrganizerManager* manager(new QOrganizerManager(QString::fromLatin1("memory")));
    return manager;
}

QDateTime TimeZone::convert(const QDateTime& dateTime) const
{
    Q_ASSERT(isValid());
    QOrganizerManager* manager = getManager();
    int offset = 100000; // impossible value
    QDateTime latestPhase;
    foreach(const TimeZonePhase& phase, mPhases) {
        QOrganizerEvent event;
        event.setStartDateTime(phase.startDateTime());
        event.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << phase.recurrenceRule());
        event.setRecurrenceDates(phase.recurrenceDates());
        QList<QOrganizerItem> occurrences =
            manager->itemOccurrences(event, phase.startDateTime(), dateTime, 500);
        if (!occurrences.isEmpty()) {
            QDateTime phaseStart(static_cast<const QOrganizerEventOccurrence &>(occurrences.last()).startDateTime());
            if (phaseStart > latestPhase) {
                latestPhase = phaseStart;
                offset = phase.utcOffset();
            }
        }
        else {
            offset = phase.utcOffset();
        }
    }
    QDateTime retn(dateTime);
    retn.setTimeSpec(Qt::UTC);
    if (offset >= -86400 && offset <= 86400) // offset must be within -24hours to +24hours
        return retn.addSecs(-offset);
    else
        return retn;
}

QDateTime TimeZones::convert(const QDateTime& dateTime, const QString& tzid) const
{
    if (!mTimeZones.contains(tzid))
        return QDateTime();
    TimeZone tz = mTimeZones.value(tzid);
    if (!tz.isValid())
        return QDateTime();
    return tz.convert(dateTime);
}

QT_END_NAMESPACE_VERSITORGANIZER
