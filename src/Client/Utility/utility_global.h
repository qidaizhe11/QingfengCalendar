#ifndef UTILITY_GLOBAL_H
#define UTILITY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTILITY_LIBRARY)
#  define UTILITYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UTILITYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UTILITY_GLOBAL_H
