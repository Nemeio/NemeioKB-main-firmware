/*
 * SetLedCmd.hpp
 *
 *  Created on: 18 octobre 2022
 *      Author: Olivier
 */

#ifndef BENCHMARK_COMMON_SETLED_SETLEDCMD_HPP_
#define BENCHMARK_COMMON_SETLED_SETLEDCMD_HPP_

#include "AppCommCmd.hpp"
#include "AssemblyHardware.h"

namespace benchmark
{
class SetLedCmd : public AppCommCmd
{
public:
    SetLedCmd(IAppCommTxListener& appCommTxListener, AssemblyHardware& hardwareAssembly);
    ~SetLedCmd() final = default;

private:
    AssemblyHardware& mHardwareAssembly;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
    void replyRetCode(AppCommDefs::AppCommErrCode retCode);
};


enum class SetLedIndex : uint8_t
{
    LED_CAPSLOCK,
    LED_BATTERY,
    LED_BLE,
    LED_INDEX_MAX
};

enum class SetLedState : uint8_t
{
    LED_OFF,
    LED_ON,
    LED_STATE_MAX
};

struct SetLedProperties
{
    SetLedIndex ledIndex;
    SetLedState ledState;
};

} // namespace benchmark

#endif /* BENCHMARK_COMMON_SETLED_SETLEDCMD_HPP_ */
