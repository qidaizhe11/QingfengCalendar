#ifndef IMCORE_H
#define IMCORE_H

#include "GlobalDefine.h"
#include "util.h"
#include <string.h>
#include <iostream>
#include <functional>

using namespace std;

class ClientConn;
struct ITcpSocketCallback;

NAMESPACE_BEGIN(imcore)

class Operation;

#ifdef __cplusplus
extern "C" {
#endif

NETWORKSHARED_EXPORT bool IMLibCoreRunEvent();
NETWORKSHARED_EXPORT bool IMLibCoreStopEvent();
NETWORKSHARED_EXPORT bool IMLibCoreIsRunning();

NETWORKSHARED_EXPORT int IMLibCoreConnect(string ip, int port);
NETWORKSHARED_EXPORT int IMLibCoreWrite(int key, ucahr_t* data, uint32_t size);
NETWORKSHARED_EXPORT void IMLibCoreShutdown(int key);
NETWORKSHARED_EXPORT void IMLibCoreClose(int key);
NETWORKSHARED_EXPORT void IMLibCoreRegisterCallback(int handle, ITcpSocketCallback* pCB);
NETWORKSHARED_EXPORT void IMLibCoreUnRegisterCallback(int handle);

NETWORKSHARED_EXPORT void IMLibCoreStartOperation(IN Operation* pOperation, Int32 delay = 0);

NETWORKSHARED_EXPORT void IMLibCoreStartOperationWithLamba(std::function<void()> operationRun,
                                                           Int32 delay = 0);

#ifdef _WIN32
unsigned int __stdcall event_run(void* threadArgu);
#else
void* event_run(void* arg);
#endif

#ifdef __cplusplus
}
#endif

NAMESPACE_END(imcore)

#endif // IMCORE_H

