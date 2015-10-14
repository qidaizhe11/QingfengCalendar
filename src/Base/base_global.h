#ifndef BASE_GLOBAL_H
#define BASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BASE_LIBRARY)
#  define BASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define BASESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BASE_GLOBAL_H
