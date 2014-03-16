#include "MyCollection.h"
#include <QDebug>

MyCollection::MyCollection(QObject *parent) :
  QObject(parent)
{
}

//-------------------------------------------------------------------------
// public

QString MyCollection::id() const
{
  return m_collection.id().toString();
}

void MyCollection::setId(const QString &id)
{
  if (m_collection.id().toString() != id) {
    qDebug() << "MyCollection::setId, input id:" << id;
    m_collection.setId(QOrganizerCollectionId::fromString(id));
    qDebug() << "After set, id:" << m_collection.id().toString();
    emit valueChanged();
  }
}

QString MyCollection::name() const
{
  return metaData(QOrganizerCollection::KeyName).toString();
}

void MyCollection::setName(const QString &name)
{
  setMetaData(QOrganizerCollection::KeyName, name);
}

QString MyCollection::description() const
{
  return metaData(QOrganizerCollection::KeyDescription).toString();
}

void MyCollection::setDescription(const QString &description)
{
  setMetaData(QOrganizerCollection::KeyDescription, description);
}

QColor MyCollection::color() const
{
  return metaData(QOrganizerCollection::KeyColor).value<QColor>();
}

void MyCollection::setColor(const QColor &color)
{
  setMetaData(QOrganizerCollection::KeyColor, color);
}

//-------------------------------------------------------------------------
// my extended property, used for online sync.

QString MyCollection::extendedId() const
{
  return extendedMetaData("ExtendedId").toString();
}

void MyCollection::setExtendedId(const QString &extended_id)
{
  setExtendedMetaData("ExtendedId", extended_id);
}

QString MyCollection::storage() const
{
  return extendedMetaData("Storage").toString();
}

void MyCollection::setStorage(const QString &storage)
{
  setExtendedMetaData("Storage", storage);
}

//-------------------------------------------------------------------------

void MyCollection::setMetaData(QOrganizerCollection::MetaDataKey key,
                               const QVariant &value)
{
  if (metaData(key) != value) {
    m_collection.setMetaData(key, value);
    emit valueChanged();
  }
}

QVariant MyCollection::metaData(QOrganizerCollection::MetaDataKey key) const
{
  return m_collection.metaData(key);
}

void MyCollection::setExtendedMetaData(const QString &key,
                                       const QVariant &value)
{
  if (extendedMetaData(key) != value) {
    m_collection.setExtendedMetaData(key, value);
    emit valueChanged();
  }
}

QVariant MyCollection::extendedMetaData(const QString &key) const
{
  return m_collection.extendedMetaData(key);
}

QOrganizerCollection MyCollection::collection() const
{
  return m_collection;
}

void MyCollection::setCollection(const QOrganizerCollection &collection)
{
  m_collection = collection;
}
