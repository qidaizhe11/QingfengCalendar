#ifndef GOOGLEPLUGIN_H
#define GOOGLEPLUGIN_H

#include <QQmlExtensionPlugin>

class GooglePlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
  void registerTypes(const char *uri);
};

#endif // GOOGLEPLUGIN_H
