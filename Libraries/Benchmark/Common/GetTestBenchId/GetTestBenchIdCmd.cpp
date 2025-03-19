#include "GetTestBenchIdCmd.hpp"

using namespace benchmark;

GetTestBenchIdCmd::GetTestBenchIdCmd(IAppCommTxListener& appCommTxListener,
                                     const TestBenchId& testBenchId)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_FUNCTIONAL_GETTESTBENCHID, appCommTxListener)
    , m_testBenchId(testBenchId)
{
}

void GetTestBenchIdCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    Response response;

    if(dataLen != 0)
    {
        response.retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;
    }
    else
    {
        memset(response.id, 0, sizeof(response.id));
        etl::copy(m_testBenchId.get().begin(), m_testBenchId.get().end(), response.id);
        response.retCode = AppCommDefs::APPCOMMERR_SUCCESS;
    }

    size_t sizeResponse = (response.retCode != AppCommDefs::APPCOMMERR_SUCCESS)
                              ? sizeof(response.retCode)
                              : sizeof(response);
    transmitResponse(reinterpret_cast<uint8_t*>(&response), sizeResponse);
}