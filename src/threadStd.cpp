/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <thread>

#ifdef _WIN32
#include <string>
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "nds3/impl/threadStd.h"

namespace nds
{

#ifdef _WIN32

namespace
{

/**
 * @brief Give a thread a name that a debugger can display.
 *
 * std::thread::native_handle() is a Win32 HANDLE here, which has nothing in
 * common with the pthread_t of pthreads-win32: that is a structure whose first
 * member points to the library's own thread object. Handing the handle to
 * pthread_setname_np() makes it dereference the handle value as that structure.
 *
 * SetThreadDescription() is the native equivalent. It is resolved at run time
 * because it only exists from Windows 10 1607 onwards; naming a thread is a
 * debugging aid, so doing nothing is an acceptable outcome on older systems.
 */
void setThreadName(void* threadHandle, const std::string& name)
{
    typedef HRESULT (WINAPI *setThreadDescription_t)(HANDLE, PCWSTR);

    static const setThreadDescription_t pSetThreadDescription =
        reinterpret_cast<setThreadDescription_t>(reinterpret_cast<void*>(
            ::GetProcAddress(::GetModuleHandleW(L"kernel32.dll"), "SetThreadDescription")));

    if(pSetThreadDescription == 0)
    {
        return;
    }

    // The thread names used by NDS3 are plain ASCII literals.
    const std::wstring wideName(name.begin(), name.end());
    pSetThreadDescription(static_cast<HANDLE>(threadHandle), wideName.c_str());
}

}

#endif

ThreadStd::ThreadStd(FactoryBaseImpl* pFactory, const std::string &name, threadFunction_t function):
    ThreadBaseImpl(pFactory, name), m_thread(function)
{
#ifdef _WIN32
    setThreadName(m_thread.native_handle(), name);
#else
    pthread_setname_np(m_thread.native_handle(), name.c_str());
#endif

}

void ThreadStd::join()
{
    m_thread.join();
}

}
