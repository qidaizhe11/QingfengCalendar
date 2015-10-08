#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H

#include <assert.h>
#include <string>
#include <tchar.h>
#include <vector>

#pragma warning(disable : 4267)

typedef signed char             Int8;
typedef unsigned char           UInt8;
typedef signed short            Int16;
typedef unsigned short          UInt16;
typedef signed int              Int32;
typedef unsigned int            UInt32;
typedef signed __int64          Int64;
typedef unsigned __int64        UInt64;
typedef char                    utf8char;
typedef unsigned char           byte;
typedef std::string             utf8String;
typedef std::basic_string<char> ByteBuffer;

#define PTR_NULL(p)     { assert(p); if (0 == p) { return 0; } }
#define PTR_VOID(p)     { assert(p); if (0 == p) { return; } }
#define STR_VOID(p)     { assert(p); if (0 == p || 0 == _tcslen(p)) { return; } }
#define PTR_FALSE(p)    { assert(p); if (0 == p) { return FALSE; } }

#define NAMESPACE_BEGIN(n)  namespace n {
#define NAMESPACE_END(n)    }

#define WARNING_STRING2(x) #x
#define WARNING_STRING(x) WARNING_STRING2(x)
#define WARNING_TARGET(x) "["#x"] "
#define WARNING_TODO(target, desc) \
    message(__FILE__ "(" WARNING_STRING(__LINE__) ") : warning : @TODO " WARNING_TARGET(target)##desc)
#define WARNING_FIX(target, desc) \
    message(__FILE__ "(" WARNING_STRING(__LINE__) ") : warning : @FIXME " WARNING_TARGET(target)##desc)
#define WARNING_WARN(target, desc) \
    message(__FILE__ "(" WARNING_STRING(__LINE__) ") : warning : @WARN " WARNING_TARGET(target)##desc)

#if defined(IN)
#undef IN
#endif // IN

#define IN

#if defined(OUT)
#undef OUT
#endif // OUT

#define OUT

#endif // GLOBALDEFINE_H

