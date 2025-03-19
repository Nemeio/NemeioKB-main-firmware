#include "ReceiveTraceLogCmd.hpp"
#include "TraceLogger.hpp"

ReceiveTraceLogCmd::ReceiveTraceLogCmd(logger::ITraceLogger& logger)
    : ReceiveDataListener(AppCommDefs::RCVDATAID_TRACE_LOG)
    , m_logger(logger)
{
}

uint8_t ReceiveTraceLogCmd::doPrepareReceive(const uint8_t*, size_t, std::size_t& retDataLen)
{
    AutoLock lock(logger::TraceLogger::instance());

    retDataLen = m_logger.oss().size();
    m_logger.clearError();

    return AppCommDefs::APPCOMMERR_SUCCESS;
}

uint8_t ReceiveTraceLogCmd::doReceiveData(uint8_t* pData, size_t len, std::size_t& retReceiveLength)
{
    AutoLock lock(logger::TraceLogger::instance());

    auto pDataChar = reinterpret_cast<char*>(pData);
    retReceiveLength = m_logger.oss().pop_to(etl::array_view{pDataChar, pDataChar + len});

    return AppCommDefs::APPCOMMERR_SUCCESS;
}

uint8_t ReceiveTraceLogCmd::doTerminateReceive()
{
    return AppCommDefs::APPCOMMERR_SUCCESS;
}

void ReceiveTraceLogCmd::doReset()
{
    // Nothing to do
}