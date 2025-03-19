#pragma once

#include "AppCommCmd.hpp"
#include "CheckerBoard.hpp"

namespace benchmark
{
class DisplayCheckerBoardCmd : public AppCommCmd
{
public:
    explicit DisplayCheckerBoardCmd(IAppCommTxListener& appCommTxListener,
                                    CheckerBoard& checkerBoard);
    ~DisplayCheckerBoardCmd() final = default;

private:
    CheckerBoard& m_checkerBoard;
    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};

} // namespace benchmark
