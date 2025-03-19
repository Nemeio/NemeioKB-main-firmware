#include "TraceLogger.hpp"

using namespace logger;

TraceLogger& TraceLogger::instance()
{
    static TraceLogger instance;

    return instance;
}

bool TraceLogger::lock(DelayMs timeout)
{
    return m_mutex.lock(timeout);
}

bool TraceLogger::unlock()
{
    return m_mutex.unlock();
}