#ifndef XDLGLOBAL_H
#define XDLGLOBAL_H

#include <qglobal.h>

#if defined(XDL_LIBRARY)
#  define XDL_EXPORT Q_DECL_EXPORT
#else
#  define XDL_EXPORT Q_DECL_IMPORT
#endif

#endif // XDLGLOBAL_H

