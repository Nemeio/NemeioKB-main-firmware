#include "DisplayCheckerBoardCmd.hpp"

using namespace benchmark;

DisplayCheckerBoardCmd::DisplayCheckerBoardCmd(IAppCommTxListener& appCommTxListener,
                                               CheckerBoard& checkerBoard)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_FUNCTIONAL_DISPLAYCHECKERBOARD, appCommTxListener)
    , m_checkerBoard(checkerBoard)
{
}

void DisplayCheckerBoardCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if((sizeof(bool) == dataLen) && (pData != nullptr))
    {
        bool isFirstColorWhite = *pData != 0 ? true : false;

        m_checkerBoard.draw(isFirstColorWhite);
        retCode = AppCommDefs::APPCOMMERR_SUCCESS;
    }

    replyRetCode(retCode);
}

void DisplayCheckerBoardCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
