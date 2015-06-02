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

#include "qversitorganizerpluginloader_p.h"
#include "qversitorganizerpluginsearch_p.h"

#include <QtCore/qpluginloader.h>

#include "qversitorganizerhandler.h"
#include "qversittimezonehandler.h"

QT_BEGIN_NAMESPACE_VERSITORGANIZER

/*!
   A less-than function for factory indices (see QVersitOrganizerHandlerFactory::index()).
   Positive values come first (ascendingly), then zero, then negative values (ascendingly).
 */
bool factoryLessThan(QVersitOrganizerHandlerFactory* a, QVersitOrganizerHandlerFactory* b) {
    if ((a->index() > 0 && b->index() > 0)
            || (a->index() < 0 && b->index() < 0))
        // same sign
        return a->index() < b->index();
    else
        // a is zero
        // or b is zero
        // or opposite sign
        return b->index() < a->index();
}

QVersitOrganizerPluginLoader* QVersitOrganizerPluginLoader::mInstance = NULL;

/*!
 * \class QVersitOrganizerPluginLoader
 * \internal
 * This is a singleton class that loads Versit plugins for organizer item processing
 */

QVersitOrganizerPluginLoader::QVersitOrganizerPluginLoader() : mTimeZoneHandler(NULL)
{
}

/*!
 * Returns the singleton instance of the QVersitOrganizerPluginLoader.
 */
QVersitOrganizerPluginLoader* QVersitOrganizerPluginLoader::instance()
{
    if (!mInstance)
        mInstance = new QVersitOrganizerPluginLoader;
    return mInstance;
}

void QVersitOrganizerPluginLoader::loadPlugins() {
    QStringList plugins = mobilityPlugins(QStringLiteral("versit"));
    if (plugins != mPluginPaths) {
        mPluginPaths = plugins;

        foreach (const QString& pluginPath, mPluginPaths) {
            QPluginLoader qpl(pluginPath);
            QObject* plugin = qpl.instance();
            QVersitOrganizerHandlerFactory* organizerPlugin =
                qobject_cast<QVersitOrganizerHandlerFactory*>(plugin);
            if (organizerPlugin && !mLoadedFactories.contains(organizerPlugin->name())) {
                mLoadedFactories.insert(organizerPlugin->name());
                mOrganizerHandlerFactories.append(organizerPlugin);
            } else if (!mTimeZoneHandler) {
                QVersitTimeZoneHandler* timeZonePlugin =
                    qobject_cast<QVersitTimeZoneHandler*>(plugin);
                if (timeZonePlugin) {
                    mTimeZoneHandler = timeZonePlugin;
                }
            }
        }
        std::sort(mOrganizerHandlerFactories.begin(), mOrganizerHandlerFactories.end(), factoryLessThan);
    }
}

/*!
 * Creates and returns handlers from the plugin.  If \a profile is the empty string, only handlers
 * with an empty profile list are returned.  If \a profile is nonempty, only handlers with either
 * an empty profile list or a profile list that contains the given \a profile are returned.
 *
 * The caller is responsible for deleting all returned handlers.
 */
QList<QVersitOrganizerHandler*> QVersitOrganizerPluginLoader::createOrganizerHandlers(const QString& profile)
{
    loadPlugins();

    QList<QVersitOrganizerHandler*> handlers;
    foreach (const QVersitOrganizerHandlerFactory* factory, mOrganizerHandlerFactories) {
        if (factory->profiles().isEmpty() ||
                (!profile.isEmpty() && factory->profiles().contains(profile))) {
            QVersitOrganizerHandler* handler = factory->createHandler();
            handlers.append(handler);
        }
    }
    return handlers;
}

QVersitTimeZoneHandler* QVersitOrganizerPluginLoader::timeZoneHandler()
{
    loadPlugins();

    return mTimeZoneHandler;
}

QT_END_NAMESPACE_VERSITORGANIZER
