/*
 * ValidateFunctionalTestsCmd.cpp
 *
 *  Created on: 12 décembre 2022
 *      Author: Olivier
 */

#include "ExitFunctionalTestsCmd.hpp"

using namespace benchmark;

ExitFunctionalTestsCmd::ExitFunctionalTestsCmd(IAppCommTxListener& appCommTxListener)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_VALIDATE_FUNCTIONAL_TESTS, appCommTxListener)
{
}

void ExitFunctionalTestsCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;
    TestBenchStatus functionalTestsStatus = TestBenchStatus::MAX;

    if((sizeof(functionalTestsStatus) == dataLen) && (pData != nullptr))
    {
        functionalTestsStatus = static_cast<TestBenchStatus>(*pData);

        retCode = (functionalTestsStatus < TestBenchStatus::MAX)
                      ? AppCommDefs::APPCOMMERR_SUCCESS
                      : AppCommDefs::APPCOMMERR_INVALID_CONTENT;
    }

    replyRetCode(retCode);

    if(retCode == AppCommDefs::APPCOMMERR_SUCCESS)
    {
        notify_observers(functionalTestsStatus);
    }
}

void ExitFunctionalTestsCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
