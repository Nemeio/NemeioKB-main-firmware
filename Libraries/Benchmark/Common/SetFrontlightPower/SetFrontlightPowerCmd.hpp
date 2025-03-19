/*
 * SetFrontlightPowerCmd.hpp
 *
 *  Created on: 13 novembre 2022
 *      Author: Olivier
 */

#ifndef BENCHMARK_COMMON_SETFRONTLIGHTPOWER_SETFRONTLIGHTPOWERCMD_HPP_
#define BENCHMARK_COMMON_SETFRONTLIGHTPOWER_SETFRONTLIGHTPOWERCMD_HPP_

#include "AppCommCmd.hpp"
#include "AssemblyHardware.h"

namespace benchmark
{
class SetFrontlightPowerCmd : public AppCommCmd
{
public:
    SetFrontlightPowerCmd(IAppCommTxListener& appCommTxListener, AssemblyHardware& hardwareAssembly);
    ~SetFrontlightPowerCmd() final = default;

private:
    AssemblyHardware& mHardwareAssembly;

    const int8_t MAX_PERCENT_POWER = 100;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};

} // namespace benchmark

#endif /* BENCHMARK_COMMON_SETFRONTLIGHTPOWER_SETFRONTLIGHTPOWERCMD_HPP_ */
