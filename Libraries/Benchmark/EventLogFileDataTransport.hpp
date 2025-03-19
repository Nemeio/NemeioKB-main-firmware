#pragma once

#include "AbstractDataTransport.hpp"
#include "Logger.hpp"

class EventLogFileDataTransport : public AbstractDataTransport
{
public:
    explicit EventLogFileDataTransport(Logger& logger);
    ~EventLogFileDataTransport() final = default;

    void sendData(const uint8_t* pData, std::size_t len) final;

private:
    Logger& m_logger;
    class DummyRxListener : public IDataTransportRxListener
    {
        void dataReceived(Identifiable& dataSource, const uint8_t* pData, std::size_t len) final
        {
            // Nothing to do
        }
    };
    DummyRxListener m_dummyRxListener;
};