#pragma once

#include "TraceCircularBufferStreamOutput.hpp"
#include "ILockable.hpp"

namespace logger
{
#ifdef RTT_LOG
template<size_t N>
using TraceLoggerOutput = TraceCircularBufferStreamOutput<N>;
#else
template<size_t N>
using TraceLoggerOutput = embedded_io::CircularBufferStreamOutput<N>;
#endif

static constexpr size_t CIRCULAR_BUFFER_SIZE = 6 * 1024;
static constexpr size_t MAX_TMP_STRING_SIZE = 150;

using TraceLoggerStream =
    embedded_io::circular_ostream<MAX_TMP_STRING_SIZE, TraceLoggerOutput<CIRCULAR_BUFFER_SIZE>>;

class ITraceLogger : public ILockable
{
public:
    ITraceLogger() = default;
    ~ITraceLogger() override = default;

    virtual void setError() = 0;
    virtual void clearError() = 0;
    virtual bool isErrorLogged() const = 0;
    virtual TraceLoggerStream& oss() = 0;
};
} // namespace logger
