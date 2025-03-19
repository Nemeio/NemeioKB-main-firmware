#pragma once

#include "AppCommCmd.hpp"
#include "AssemblyHardware.h"
#include "etl/observer.h"
#include "ExitElectricalTestsCmdObserver.hpp"

namespace benchmark
{
class ExitElectricalTestsCmd : public AppCommCmd,
                               public etl::observable<ExitElectricalTestsCmdObserver, 1>
{
public:
    explicit ExitElectricalTestsCmd(IAppCommTxListener& appCommTxListener);
    ~ExitElectricalTestsCmd() final = default;

private:
    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};

} // namespace benchmark
