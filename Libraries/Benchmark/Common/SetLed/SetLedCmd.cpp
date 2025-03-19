/*
 * SetLedCmd.cpp
 *
 *  Created on: 18 octobre 2022
 *      Author: Olivier
 */

#include "SetLedCmd.hpp"

using namespace benchmark;

SetLedCmd::SetLedCmd(IAppCommTxListener& appCommTxListener, AssemblyHardware& hardwareAssembly)
    : AppCommCmd(AppCommDefs::AppCommCmd::BENCH_COMMON_SETLED, appCommTxListener)
    , mHardwareAssembly(hardwareAssembly)
{
}

void SetLedCmd::dataReceived(const uint8_t* pData, size_t dataLen)
{
    AppCommDefs::AppCommErrCode retCode = AppCommDefs::APPCOMMERR_MALFORMED_DATA;
    std::array<std::reference_wrapper<ILED>, static_cast<int>(SetLedIndex::LED_INDEX_MAX)> leds =
        {mHardwareAssembly.getCapsLockLed(),
         mHardwareAssembly.getBatteryLed(),
         mHardwareAssembly.getBleLed()};
    SetLedProperties ledProperties;

    if((sizeof(SetLedProperties) == dataLen) && (pData != nullptr))
    {
        ledProperties = *((const SetLedProperties*) pData);

        if((ledProperties.ledIndex >= SetLedIndex::LED_INDEX_MAX)
           || (ledProperties.ledState >= SetLedState::LED_STATE_MAX))
        {
            retCode = AppCommDefs::APPCOMMERR_INVALID_CONTENT;
        }
        else
        {
            /* Turn led on or off */
            if(ledProperties.ledState == SetLedState::LED_ON)
            {
                leds[static_cast<uint8_t>(ledProperties.ledIndex)].get().turnOn();
            }
            else
            {
                leds[static_cast<uint8_t>(ledProperties.ledIndex)].get().turnOff();
            }
            retCode = AppCommDefs::APPCOMMERR_SUCCESS;
        }
    }

    replyRetCode(retCode);
}

void SetLedCmd::replyRetCode(AppCommDefs::AppCommErrCode retCode)
{
    transmitResponse(reinterpret_cast<uint8_t*>(&retCode), sizeof(retCode));
}
