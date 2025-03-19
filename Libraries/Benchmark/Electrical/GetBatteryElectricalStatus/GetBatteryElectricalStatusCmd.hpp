/*
 * GetBatteryElectricalStatusCmd.hpp
 *
 *  Created on: 10 novembre 2022
 *      Author: Olivier
 */

#ifndef BENCHMARK_ELECTRICAL_GETBATTERYELECTRICALSTATUSCMD_HPP_
#define BENCHMARK_ELECTRICAL_GETBATTERYELECTRICALSTATUSCMD_HPP_

#include "AppCommCmd.hpp"
#include "AssemblyHardware.h"

namespace benchmark
{
class GetBatteryElectricalStatusCmd : public AppCommCmd
{
public:
    GetBatteryElectricalStatusCmd(IAppCommTxListener& appCommTxListener,
                                  AssemblyHardware& hardwareAssembly);
    ~GetBatteryElectricalStatusCmd() final = default;

private:
    AssemblyHardware& mHardwareAssembly;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};

PACK((struct GetBatteryProperties
      {
          int8_t retCode;
          int16_t current;
          uint16_t voltage;
      }));

} // namespace benchmark

#endif /* BENCHMARK_ELECTRICAL_GETBATTERYELECTRICALSTATUSCMD_HPP_ */
