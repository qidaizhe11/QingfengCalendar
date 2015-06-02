#ifndef MYCOLLECTION_H
#define MYCOLLECTION_H

#include <QObject>
#include <QtQml/qqml.h>
#include <QColor>
#include <QtOrganizer/qorganizercollection.h>
#include <QtOrganizer/qorganizerglobal.h>

//QT_USE_NAMESPACE

//QTORGANIZER_USE_NAMESPACE
using namespace QtOrganizer;

QT_BEGIN_NAMESPACE

class MyCollection : public QObject
{
  Q_OBJECT

  Q_ENUMS(MetaDataKey)
  Q_PROPERTY(QString collectionId READ id WRITE setId
             NOTIFY valueChanged)
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY valueChanged)
  Q_PROPERTY(QString description READ description WRITE setDescription
             NOTIFY valueChanged)
  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY valueChanged)
  Q_PROPERTY(QString extendedId READ extendedId WRITE setExtendedId
             NOTIFY valueChanged)
  Q_PROPERTY(QString storage READ storage WRITE setStorage
             NOTIFY valueChanged)
public:
  enum MetaDataKey {
    KeyName = QOrganizerCollection::KeyName,
    KeyDescription = QOrganizerCollection::KeyDescription,
    KeyColor = QOrganizerCollection::KeyColor
  };

  explicit MyCollection(QObject *parent = 0);
  ~MyCollection() {}

  QString id() const;
  void setId(const QString& id);

  QString name() const;
  void setName(const QString& name);

  QString description() const;
  void setDescription(const QString& description);

  QColor color() const;
  void setColor(const QColor& color);

  QString extendedId() const;
  void setExtendedId(const QString& extended_id);

  QString storage() const;
  void setStorage(const QString& storage);

  Q_INVOKABLE void setMetaData(QOrganizerCollection::MetaDataKey key,
                               const QVariant& value);
  Q_INVOKABLE QVariant metaData(QOrganizerCollection::MetaDataKey key) const;

  Q_INVOKABLE void setExtendedMetaData(const QString& key,
                                       const QVariant& value);
  Q_INVOKABLE QVariant extendedMetaData(const QString& key) const;

  // used by model
  QOrganizerCollection toOrganizerCollection() const;
  void setCollection(const QOrganizerCollection& collection);

Q_SIGNALS:
  void valueChanged();

public slots:

private:
  QOrganizerCollection m_collection;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(MyCollection*)

#endif // MYCOLLECTION_H
