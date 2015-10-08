
#include "Lock.h"

CFastLock::CFastLock()
{
#ifdef _WIN32
    InitializeCriticalSection(&m_critical_section);
#else
    pthread_mutex_init(&m_mutex, NULL);
#endif
}

CFastLock::~CFastLock()
{
#ifdef _WIN32
    DeleteCriticalSection(&m_critical_section);
#else
    pthread_mutex_destroy(&m_mutex);
#endif
}

void CLock::lock()
{
#ifdef _WIN32
    EnterCriticalSection(&m_lock.m_critical_section);
#else
    pthread_mutex_lock(&m_lock);
#endif
}

void CLock::unlock()
{
#ifdef _WIN32
    LeaveCriticalSection(&m_lock.m_critical_section);
#else
    pthread_mutex_unlock(&m_lock.m_mutex);
#endif
}

#ifndef _WIN32
bool CLock::try_lock()
{
    return pthread_mutex_trylock(&m_lock.m_mutex) == 0;
}
#endif

CAutoLock::CAutoLock(CLock *pLock)
{
    m_pLock = pLock;
    if (m_pLock != NULL) {
        m_pLock->lock();
    }
}

CAutoLock::~CAutoLock()
{
    if (m_pLock != NULL) {
        m_pLock->unlock();
    }
}
