#pragma once

#include "AppCommCmd.hpp"
#include "AssemblyHardware.h"
#include "etl/observer.h"
#include "ExitFunctionalTestsCmdObserver.hpp"

namespace benchmark
{
class ExitFunctionalTestsCmd : public AppCommCmd,
                               public etl::observable<ExitFunctionalTestsCmdObserver, 1>
{
public:
    explicit ExitFunctionalTestsCmd(IAppCommTxListener& appCommTxListener);
    ~ExitFunctionalTestsCmd() final = default;

private:
    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};

} // namespace benchmark
