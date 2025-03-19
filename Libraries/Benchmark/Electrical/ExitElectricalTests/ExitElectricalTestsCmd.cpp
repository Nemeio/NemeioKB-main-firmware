/*
 * ValidateElectricalTestsCmd.cpp
 *
 *  Created on: 28 octobre 2022
 *      Author: Olivier
 */

#include "ExitElectricalTestsCmd.hpp"

using namespace benchmark;

ExitElectricalTestsCmd::ExitElectricalTestsCmd(IAppCommTxListener& appCommTxListener)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_EXIT_ELECTRICAL_TESTS, appCommTxListener)
{
}

void ExitElectricalTestsCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;
    TestBenchStatus electricalTestsStatus = TestBenchStatus::MAX;

    if((sizeof(electricalTestsStatus) == dataLen) && (pData != nullptr))
    {
        electricalTestsStatus = static_cast<TestBenchStatus>(*pData);

        retCode = (electricalTestsStatus < TestBenchStatus::MAX)
                      ? AppCommDefs::APPCOMMERR_SUCCESS
                      : AppCommDefs::APPCOMMERR_INVALID_CONTENT;
    }

    replyRetCode(retCode);

    if(retCode == AppCommDefs::APPCOMMERR_SUCCESS)
    {
        notify_observers(electricalTestsStatus);
    }
}

void ExitElectricalTestsCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
