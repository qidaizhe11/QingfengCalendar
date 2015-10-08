#ifndef UTIL_H
#define UTIL_H

#define _CRT_SECURE_NO_DEPRECATE  // remove warning C4996

#include "ostype.h"
#include "Lock.h"
#include "GlobalConfig.h"
//#include "yaolog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <strings.h>
#endif

#include <sys/stat.h>
#include <assert.h>

#ifdef _WIN32
#define snprintf    sprintf_s
#else
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#endif

#ifdef __GNUC__
#include <ext/hash_map>
using namespace __gnu_cxx;
namespace __gnu_cxx {
template<> struct hash<std::string> {
    size_t operator()(const std::string& x) const {
        return hash<const char*>()(x.c_str());
    }
};
}
#else
#include <hash_map>
using namespace stdext;
#endif

#ifdef ANDROID
#include <android/log.h>
#endif

using namespace std;

#define NOTUSED_ARG(v) ((void)v)

class CLock;
class CRefObject
{
public:
    CRefObject();
    virtual ~CRefObject();

    void SetLock(CLock* lock) { m_lock = lock; }
    void AddRef();
    void ReleaseRef();

private:
    int m_refCount;
    CLock* m_lock;
};

uint64_t get_tick_count();
void util_sleep(uint32_t millisecond);

class CStrExplode
{
public:
    CStrExplode(char* str, char seperator);
    virtual ~CStrExplode();

    uint32_t GetItemCnt() { return m_item_cnt; }
    char* GetItem(uint32_t idx) { return m_item_list[idx]; }

private:
    uint32_t m_item_cnt;
    char**   m_item_list;
};

string int2string(uint32_t user_id);
uint32_t string2int(const string& value);

void replace_mark(string& str, string& new_value, uint32_t& strart_pos);
void replace_mark(string& str, uint32_t new_value, uint32_t& start_pos);
char* replaceStr(char* pSrc, char oldChar, char newChar);
size_t get_file_size(const char* path);
unsigned int ip2long(const char* ip);
char* long2ip(const unsigned int in);
void writePid();

#endif // UTIL_H

