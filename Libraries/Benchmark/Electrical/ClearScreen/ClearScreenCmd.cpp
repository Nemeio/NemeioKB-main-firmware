#include "ClearScreenCmd.hpp"

using namespace benchmark;

ClearScreenCmd::ClearScreenCmd(IAppCommTxListener& appCommTxListener, IScreen& screen)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_ELECTRICAL_CLEARSCREEN, appCommTxListener)
    , m_screen(screen)
{
}

void ClearScreenCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;

    if(0 == dataLen)
    {
        retCode = (ScreenDefs::ScreenErrCode::SUCCESS
                   == m_screen.getAssociatedScreenController().clearScreen(m_screen))
                      ? AppCommDefs::APPCOMMERR_SUCCESS
                      : AppCommDefs::APPCOMMERR_DRAW_CONFIGURATION;
    }

    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}