/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <array>
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "nds3/impl/logStreamGetterImpl.h"

namespace nds
{

/*
 * Thread-local storage that deletes a thread's logger when the thread exits.
 *
 * On Windows this is fiber-local storage, the native counterpart of a pthread
 *  key with a destructor. Unlike pthread_key_delete(), FlsFree() also runs the
 *  destructor for the loggers that other threads still hold.
 */
struct LogStreamGetterImpl::LoggerKeys
{
#ifdef _WIN32
    typedef DWORD tlsKey_t;

    static VOID NTAPI deleteLoggerCallback(PVOID pLogger)
    {
        LogStreamGetterImpl::deleteLogger(pLogger);
    }

    static tlsKey_t createKey()
    {
        return FlsAlloc(&deleteLoggerCallback);
    }

    static void deleteKey(tlsKey_t key)
    {
        FlsFree(key);
    }

    static void* getValue(tlsKey_t key)
    {
        return FlsGetValue(key);
    }

    static void setValue(tlsKey_t key, void* value)
    {
        FlsSetValue(key, value);
    }
#else
    typedef pthread_key_t tlsKey_t;

    static tlsKey_t createKey()
    {
        tlsKey_t key;
        pthread_key_create(&key, &LogStreamGetterImpl::deleteLogger);
        return key;
    }

    static void deleteKey(tlsKey_t key)
    {
        pthread_key_delete(key);
    }

    static void* getValue(tlsKey_t key)
    {
        return pthread_getspecific(key);
    }

    static void setValue(tlsKey_t key, void* value)
    {
        pthread_setspecific(key, value);
    }
#endif

    std::array<tlsKey_t, (size_t)logLevel_t::none> m_keys;
};

LogStreamGetterImpl::LogStreamGetterImpl(): m_loggersKeys(new LoggerKeys)
{
    for(size_t scanLevels(0); scanLevels != m_loggersKeys->m_keys.size(); ++scanLevels)
    {
        m_loggersKeys->m_keys[scanLevels] = LoggerKeys::createKey();
    }

}

LogStreamGetterImpl::~LogStreamGetterImpl()
{
    for(size_t scanLevels(0); scanLevels != m_loggersKeys->m_keys.size(); ++scanLevels)
    {
        std::ostream* pStream = (std::ostream*)LoggerKeys::getValue(m_loggersKeys->m_keys[scanLevels]);
        if(pStream != 0)
        {
            LoggerKeys::setValue(m_loggersKeys->m_keys[scanLevels], 0);
            delete pStream;
        }
        LoggerKeys::deleteKey(m_loggersKeys->m_keys[scanLevels]);
    }
}

std::ostream* LogStreamGetterImpl::getLogStream(const logLevel_t logLevel)
{
    std::ostream* pStream = (std::ostream*)LoggerKeys::getValue(m_loggersKeys->m_keys[(size_t)logLevel]);
    if(pStream == 0)
    {
        pStream = createLogStream(logLevel);
        LoggerKeys::setValue(m_loggersKeys->m_keys[(size_t)logLevel], pStream);
    }

    return pStream;
}

void LogStreamGetterImpl::deleteLogger(void* pLogger)
{
    delete (std::ostream*)pLogger;
}


}
