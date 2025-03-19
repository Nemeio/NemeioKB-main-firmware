#pragma once

#include "ReceiveDataListener.hpp"
#include "ITraceLogger.hpp"

class ReceiveTraceLogCmd : public ReceiveDataListener
{
public:
    explicit ReceiveTraceLogCmd(logger::ITraceLogger& logger);
    ~ReceiveTraceLogCmd() final = default;

private:
    logger::ITraceLogger& m_logger;
    uint8_t doPrepareReceive(const uint8_t* pHeader,
                             size_t headerLen,
                             std::size_t& retDataLen) final;
    uint8_t doReceiveData(uint8_t* pData, size_t len, std::size_t& retReceiveLength) final;
    uint8_t doTerminateReceive() final;
    void doReset() final;
};