#pragma once

#include "ITraceLogger.hpp"
#include "Mutex.hpp"
#include "AutoLock.h"

namespace logger
{
class TraceLogger : public ITraceLogger
{
public:
    static constexpr etl::format_spec DEFAULT_FORMAT = etl::format_spec();

    static TraceLogger& instance();

    bool lock(DelayMs timeout) final;
    bool unlock() final;

    void setError() final { m_isErrorLogged = true; }
    void clearError() final { m_isErrorLogged = false; }
    bool isErrorLogged() const final { return m_isErrorLogged; }
    TraceLoggerStream& oss() final { return m_stream; }

private:
    TraceLogger() = default;
    Mutex m_mutex;
    TraceLoggerStream m_stream;
    bool m_isErrorLogged{false};
};

} // namespace logger

#define LOG_H(lvl, log_module, x) \
    do \
    { \
        logger::TraceLogger::instance().oss() \
            << logger::TraceLogger::DEFAULT_FORMAT << "<" lvl "> " log_module ": " << x \
            << embedded_io::endl; \
    } while(0)

#define LOG_H_LOCKED(lvl, log_module, x) \
    do \
    { \
        AutoLock lock(logger::TraceLogger::instance()); \
        LOG_H(lvl, log_module, x); \
    } while(0)

#define LOG_INFO(log_module, x) LOG_H_LOCKED("INF", log_module, x)
#define LOG_WARN(log_module, x) LOG_H_LOCKED("WRN", log_module, x)
#define LOG_ERR(log_module, x) \
    do \
    { \
        AutoLock lock(logger::TraceLogger::instance()); \
        logger::TraceLogger::instance().setError(); \
        LOG_H("ERR", log_module, x); \
    } while(0)
