#include "SetTestBenchIdCmd.hpp"
#include "etl_string_utils.hpp"

using namespace benchmark;

SetTestBenchIdCmd::SetTestBenchIdCmd(IAppCommTxListener& appCommTxListener, TestBenchId& testBenchId)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_FUNCTIONAL_SETTESTBENCHID, appCommTxListener)
    , m_testBenchId(testBenchId)
{
}

void SetTestBenchIdCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(m_testBenchId.isValid())
    {
        // Allow to set test bench id only once
        retCode = AppCommDefs::APPCOMMERR_STATE;
    }
    else if((TestBenchId::SIZE == dataLen) && (pData != nullptr))
    {
        TestBenchId testBenchId(etl_string_utils::arrayToStringView(
            {reinterpret_cast<const char*>(pData), reinterpret_cast<const char*>(pData + dataLen)}));

        if(testBenchId.isValid())
        {
            retCode = AppCommDefs::APPCOMMERR_SUCCESS;
            m_testBenchId.set(testBenchId.get());
        }
        else
        {
            retCode = AppCommDefs::APPCOMMERR_INVALID_CONTENT;
        }
    }

    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}