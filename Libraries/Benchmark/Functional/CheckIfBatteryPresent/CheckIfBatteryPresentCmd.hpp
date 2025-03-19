/*
 * CheckIfBatteryPresentCmd.hpp
 *
 *  Created on: 9 décembre 2022
 *      Author: Olivier
 */

#ifndef BENCHMARK_ELECTRICAL_CHECKIFBATTERYPRESENTCMD_HPP_
#define BENCHMARK_ELECTRICAL_CHECKIFBATTERYPRESENTCMD_HPP_

#include "AppCommCmd.hpp"
#include "AssemblyHardware.h"

namespace benchmark
{
class CheckIfBatteryPresentCmd : public AppCommCmd
{
public:
    CheckIfBatteryPresentCmd(IAppCommTxListener& appCommTxListener,
                             AssemblyHardware& hardwareAssembly);
    ~CheckIfBatteryPresentCmd() final = default;

private:
    AssemblyHardware& mHardwareAssembly;

    static constexpr uint16_t MIN_BATTERY_VOLTAGE_DETECTION_MV = 100;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};

PACK((struct CheckBatteryPresenceProperties
      {
          int8_t retCode;
          int8_t isBatteryPresent;
      }));

} // namespace benchmark

#endif /* BENCHMARK_ELECTRICAL_CHECKIFBATTERYPRESENTCMD_HPP_ */
