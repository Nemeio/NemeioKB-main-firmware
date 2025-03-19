#include "EventLogFileDataTransport.hpp"

EventLogFileDataTransport::EventLogFileDataTransport(Logger& logger)
    : AbstractDataTransport(m_dummyRxListener)
    , m_logger(logger)
{
}

void EventLogFileDataTransport::sendData(const uint8_t* pData, std::size_t len)
{
    m_logger.logTestEvent(etl::string_view{reinterpret_cast<const char*>(pData), len});
}
