#ifndef PRIVATE_PLUGIN_H
#define PRIVATE_PLUGIN_H

#include <QQmlExtensionPlugin>

class PrivatePlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
  void registerTypes(const char *uri);
};

#endif // PRIVATE_PLUGIN_H

