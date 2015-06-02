#ifndef MYPLUGINS_PLUGIN_H
#define MYPLUGINS_PLUGIN_H

#include <QQmlExtensionPlugin>

class MyPluginsPlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
  void registerTypes(const char *uri);
};

#endif // MYPLUGINS_PLUGIN_H

