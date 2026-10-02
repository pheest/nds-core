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
#ifndef _DLL
#define __PTW32_STATIC_LIB
#endif
#endif
#include <pthread.h>

#include "nds3/impl/logStreamGetterImpl.h"

namespace nds
{

struct LogStreamGetterImpl::LoggerKeys
{
    std::array<pthread_key_t, (size_t)logLevel_t::none> m_keys;
};

LogStreamGetterImpl::LogStreamGetterImpl(): m_loggersKeys(new LoggerKeys)
{
    for(size_t scanLevels(0); scanLevels != m_loggersKeys->m_keys.size(); ++scanLevels)
    {
        pthread_key_create(&(m_loggersKeys->m_keys[scanLevels]), &LogStreamGetterImpl::deleteLogger);
    }

}

LogStreamGetterImpl::~LogStreamGetterImpl()
{
    for(size_t scanLevels(0); scanLevels != m_loggersKeys->m_keys.size(); ++scanLevels)
    {
        std::ostream* pStream = (std::ostream*)pthread_getspecific(m_loggersKeys->m_keys[scanLevels]);
        if(pStream != 0)
        {
            pthread_setspecific(m_loggersKeys->m_keys[scanLevels], 0);
            delete pStream;
        }
        pthread_key_delete(m_loggersKeys->m_keys[scanLevels]);
    }
}

std::ostream* LogStreamGetterImpl::getLogStream(const logLevel_t logLevel)
{
    std::ostream* pStream = (std::ostream*)pthread_getspecific(m_loggersKeys->m_keys[(size_t)logLevel]);
    if(pStream == 0)
    {
        pStream = createLogStream(logLevel);
        pthread_setspecific(m_loggersKeys->m_keys[(size_t)logLevel], pStream);
    }

    return pStream;
}

void LogStreamGetterImpl::deleteLogger(void* pLogger)
{
    delete (std::ostream*)pLogger;
}


}
