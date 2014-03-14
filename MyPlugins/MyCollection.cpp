#include "MyCollection.h"

MyCollection::MyCollection(QObject *parent) :
  QObject(parent)
{
}

QString MyCollection::id() const
{
  return m_collection.id().toString();
}

void MyCollection::setId(const QString &id)
{
  if (m_collection.id().toString() != id) {
    m_collection.setId(QOrganizerCollectionId::fromString(id));
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

QOrganizerCollection MyCollection::collection() const
{
  return m_collection;
}

void MyCollection::setCollection(const QOrganizerCollection &collection)
{
  m_collection = collection;
}
