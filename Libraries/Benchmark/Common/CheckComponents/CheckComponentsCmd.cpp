/*
 * CheckComponentsCmd.cpp
 *
 *  Created on: 14 novembre 2022
 *      Author: Olivier
 */

#include "CheckComponentsCmd.hpp"

using namespace benchmark;

CheckComponentsCmd::CheckComponentsCmd(IAppCommTxListener& appCommTxListener,
                                       AssemblyHardware& hardwareAssembly,
                                       ITestLog& log,
                                       PackageFileSystem& firmwarePackageFileSystem)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_COMMON_CHECKCOMPONENTS, appCommTxListener)
    , mComponentsTesting(hardwareAssembly, log, firmwarePackageFileSystem)
    , mIOTesting(hardwareAssembly, log)
{
}

void CheckComponentsCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::AppCommErrCode::APPCOMMERR_MALFORMED_DATA;

    if(SubTestIndexes testIndex; (sizeof(testIndex) == dataLen) && (pData != nullptr))
    {
        testIndex = static_cast<SubTestIndexes>(*pData);
        if(static_cast<size_t>(testIndex) >= componentsTest.size())
        {
            retCode = AppCommDefs::APPCOMMERR_INVALID_CONTENT;
        }
        else
        {
            if(componentsTest[static_cast<int>(testIndex)].singleTestSuiteFunction(
                   componentsTest[static_cast<int>(testIndex)].singleTestSuiteIndex))
            {
                retCode = AppCommDefs::AppCommErrCode::APPCOMMERR_SUCCESS;
            }
            else
            {
                retCode = AppCommDefs::AppCommErrCode::APPCOMMERR_UNEXPECTED;
            }
        }
    }

    replyRetCode(retCode);
}

void CheckComponentsCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<const uint8_t*>(&retCode), sizeof(retCode));
}