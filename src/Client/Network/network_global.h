#ifndef NETWORK_GLOBAL_H
#define NETWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NETWORK_LIBRARY)
#  define NETWORKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NETWORKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NETWORK_GLOBAL_H
