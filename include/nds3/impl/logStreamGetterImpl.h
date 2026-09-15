/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSLOGSTREAMGETTERIMPL_H
#define NDSLOGSTREAMGETTERIMPL_H

#include <memory>
#include <ostream>

#include "nds3/definitions.h"

namespace nds
{

class BaseImpl;


class NDS3_API LogStreamGetterImpl
{
public:
    LogStreamGetterImpl();

    virtual ~LogStreamGetterImpl();

    /**
     * @brief Allocates a new stream logger and returns it.
     *
     * It is the responsability of the caller to delete the logger when it is no longer
     *  needed.
     *
     * @param logLevel the log level
     * @return         the newly allocate log stream
     */

    std::ostream* getLogStream(const logLevel_t logLevel);

protected:
    virtual std::ostream* createLogStream(const logLevel_t logLevel) = 0;

private:
    /**
     * @brief Registered as the thread-local storage destructor, to remove the
     *        loggers that are specific to a thread when that thread exits
     */
    static void deleteLogger(void* logger);

    /**
     * @brief Used to gain access to the node's loggers (they are different for each thread)
     *
     * Held behind an opaque pointer so that the thread-local storage type stays
     * out of this header: it is installed, and naming pthread_key_t here would
     * force every consumer to have pthread.h on its include path.
     */
    struct LoggerKeys;
    std::unique_ptr<LoggerKeys> m_loggersKeys;


};
}
#endif // NDSLOGSTREAMGETTERIMPL_H
