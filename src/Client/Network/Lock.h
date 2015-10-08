#ifndef LOCK_H
#define LOCK_H

#include "network_global.h"
#include "ostype.h"

class NETWORKSHARED_EXPORT CFastLock
{
public:
    CFastLock();
    ~CFastLock();

#ifdef _WIN32
    CRITICAL_SECTION m_critical_section;
#else
    pthread_mutex_t m_mutex;
#endif
};

class NETWORKSHARED_EXPORT CLock
{
public:
    void lock();
    void unlock();
#ifndef _WIN32
    virtual bool try_lock();
#endif

private:
    CFastLock m_lock;
};

class NETWORKSHARED_EXPORT CAutoLock
{
public:
    CAutoLock(CLock* pLock);
    virtual ~CAutoLock();

private:
    CLock* m_pLock;
};

#endif // LOCK_H

