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

#include "qversitorganizerexporter.h"
#include "qversitorganizerexporter_p.h"

#include <QtOrganizer/qorganizer.h>

#include <QtVersit/qversitproperty.h>

QTORGANIZER_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSITORGANIZER

/*!
  \class QVersitOrganizerExporter
  \brief The QVersitOrganizerExporter class converts \l {QOrganizerItem}{QOrganizerItems} into
  \l {QVersitDocument}{QVersitDocuments}.

  \ingroup versit
  \inmodule QtVersit

  This class is used to convert a list of \l {QOrganizerItem}{QOrganizerItems} (which may be stored
  in a QOrganizerManager) into a QVersitDocument (which may be written to an I/O device using
  QVersitReader.  While multiple items are provided as input, a single QVersitDocument is produced
  as output.  Unless there is an error, there is a one-to-one mapping between organizer items
  and sub-documents of the result.
 */

/*!
  \class QVersitOrganizerExporterDetailHandler
  \brief The QVersitOrganizerExporterDetailHandler class is an interface for specifying
  custom export behaviour for certain organizer item details.

  \ingroup versit-extension
  \inmodule QtVersit

  For general information on extending Qt Versit, see the document on
  \l{Qt Versit Plugins}.

  \sa QVersitOrganizerExporter
 */

/*!
  \fn QVersitOrganizerExporterDetailHandler::~QVersitOrganizerExporterDetailHandler()
  Frees any memory in use by this handler.
 */

/*!
  \fn void QVersitOrganizerExporterDetailHandler::detailProcessed(const QOrganizerItem& item, const QOrganizerItemDetail& detail, const QVersitDocument& document, QSet<int>* processedFields, QList<QVersitProperty>* toBeRemoved, QList<QVersitProperty>* toBeAdded)

  Process \a detail and provide a list of updated \l{QVersitProperty}{QVersitProperties} by
  modifying the \a toBeRemoved and \a toBeAdded lists.

  This function is called on every QOrganizerItemDetail encountered during an export, after the
  detail has been processed by the QVersitOrganizerExporter.  An implementation of this function can
  be made to provide support for QOrganizerItemDetails not supported by QVersitOrganizerExporter.

  The supplied \a item is the container for the \a detail.  \a processedFields contains a list of
  fields in the \a detail that were considered by the QVersitOrganizerExporter or another handler in
  processing the detail.  \a document holds the state of the document before the detail was
  processed by the exporter.

  \a toBeRemoved and \a toBeAdded are initially filled with a list of properties that the exporter
  will remove from and add to the document.  These lists can be modified (by removing, modifying or
  adding properties) by the handler to control the changes that will actually be made to the
  document.  If a property is to be modified in the document, the old version will appear in the
  \a toBeRemoved list and the new version will appear in the \a toBeAdded list.  When the handler
  uses a field from the detail, it should update the processedFields set to reflect this to inform
  later handlers that the field has already been processed.

  After the handler returns control back to the exporter, the properties in the \a toBeRemoved
  list will be removed and the properties in the \a toBeAdded list will be appended to the document.
 */

/*!
  \fn void QVersitOrganizerExporterDetailHandler::itemProcessed(const QOrganizerItem& item, QVersitDocument* document)
  Perform any final processing on the \a document generated by the \a item.  This can be
  implemented by the handler to clear any internal state before moving onto the next item.

  This function is called after all QOrganizerItemDetails have been handled by the
  QVersitOrganizerExporter.
*/

/*!
  \enum QVersitOrganizerExporter::Error
  This enum specifies an error that occurred during the most recent call to exportItems()
  \value NoError The most recent operation was successful
  \value EmptyOrganizerError One of the organizer items was empty
  \value UnknownComponentTypeError One of the components in the iCalendar file is not supported
  \value UnderspecifiedOccurrenceError An event or todo exception was found which did not specify both its parent and a specifier for which instance to override
  */

/*! Constructs a new exporter */
QVersitOrganizerExporter::QVersitOrganizerExporter()
    : d(new QVersitOrganizerExporterPrivate)
{
}

/*!
 * Constructs a new exporter for the given \a profile.  The profile strings should be one of those
 * defined by QVersitOrganizerHandlerFactory, or a value otherwise agreed to by a \l{Qt Versit
 * Plugins}{Versit plugin}.
 *
 * The profile determines which plugins will be loaded to supplement the exporter.
 */
QVersitOrganizerExporter::QVersitOrganizerExporter(const QString& profile)
    : d(new QVersitOrganizerExporterPrivate(profile))
{
}

/*! Frees the memory used by the exporter */
QVersitOrganizerExporter::~QVersitOrganizerExporter()
{
    delete d;
}

/*!
 * Converts \a items into a QVersitDocument, using the format given by \a versitType.
 * Returns true on success.  If any of the items could not be exported, false is returned and
 * errorMap() will return a list describing the errors that occurred.  The successfully exported
 * components will still be available via document().
 *
 * \sa document(), errorMap()
 */
bool QVersitOrganizerExporter::exportItems(
    const QList<QOrganizerItem>& items,
    QVersitDocument::VersitType versitType)
{
    int itemIndex = 0;
    d->mErrors.clear();
    d->mResult.clear();
    d->mResult.setType(versitType);
    d->mResult.setComponentType(QStringLiteral("VCALENDAR"));
    bool ok = true;
    QList<QTVERSIT_PREPEND_NAMESPACE(QVersitDocument)> results;
    foreach (const QOrganizerItem& item, items) {
        QTVERSIT_PREPEND_NAMESPACE(QVersitDocument) document;
        document.setType(versitType);
        QVersitOrganizerExporter::Error error;
        if (d->exportItem(item, &document, &error)) {
            results.append(document);
        } else {
            d->mErrors.insert(itemIndex, error);
            ok = false;
        }
        itemIndex++;
    }
    d->mResult.setSubDocuments(results);

    return ok;
}

/*!
 *Returns the document exported in the most recent call to exportItems().
 *
 * \sa exportItems()
 */
QVersitDocument QVersitOrganizerExporter::document() const
{
    return d->mResult;
}

/*!
 * \fn QVersitOrganizerExporter::errorMap() const
 *
 *Returns the map of errors encountered in the most recent call to exportItems(). The key is
 * the index into the input list of organizer items and the value is the error that occurred on that
 * item. If errors occur, export does not generate EmptyContactError or NoNameError errors but
 * just succeeds in creating the empty, albeit invalid, vCard. QVersitContactExporter never fails.
 *
 * \sa exportItems()
 */
QMap<int, QVersitOrganizerExporter::Error> QVersitOrganizerExporter::errorMap() const
{
    return d->mErrors;
}

/*!
 * Sets \a handler to be the handler for processing QOrganizerItemDetails, or 0 to have no handler.
 *
 * Does not take ownership of the handler.  The client should ensure the handler remains valid for
 * the lifetime of the exporter.
 *
 * Only one detail handler can be set.  If another detail handler was previously set, it will no
 * longer be associated with the exporter.
 */
void QVersitOrganizerExporter::setDetailHandler(QVersitOrganizerExporterDetailHandler* handler)
{
    d->mDetailHandler = handler;
}

QT_END_NAMESPACE_VERSITORGANIZER
