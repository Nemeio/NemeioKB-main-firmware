
#pragma once

#include "IKeyboardParametersCodec.hpp"
#include "KeyboardParameters.hpp"
#include "qcbor/qcbor.h"
#include "qcbor_util.hpp"

class CborKeyboardParametersCodec : public IKeyboardParametersCodec
{
public:
    CborKeyboardParametersCodec() = default;
    ~CborKeyboardParametersCodec() final = default;

    bool decode(etl::array_view<const std::byte> data,
                KeyboardParameters& keyboardParameters) const final;

    static bool decodeFromContext(QCBORDecodeContext& context,
                                  KeyboardParameters& keyboardParameters);

    ssize_t encode(const KeyboardParameters& keyboardParameters,
                   etl::array_view<std::byte> data) const final;

    static bool encodeFromContext(QCBOREncodeContext& context,
                                  const KeyboardParameters& keyboardParameters);

private:
    enum class CborId
    {
        INACTIVE_TIME = 1,
        SLEEP_TIME,
        INACTIVE_TIME_USB_DISCONNECTED,
        SLEEP_TIME_USB_DISCONNECTED,
        POWER_OFF_TIME_USB_DISCONNECTED,
        LED_POWER_ACTIVE_LEVEL,
        LED_POWER_INACTIVE_LEVEL,
        BUTTON_LONG_PRESS_DELAY,
        BUTTON_REPEAT_LONG_PRESS_DELAY,
        CLEAN_REFRESH_PERIOD,
        DEMO_MODE,
        LOW_BATTERY_BLINK_ON_DELAY_MS,
        LOW_BATTERY_BLINK_OFF_DELAY_MS,
        LOW_BATTERY_LEVEL_THRESHOLD,
        BLE_BLINK_ON_DELAY_MS,
        BLE_BLINK_OFF_DELAY_MS,
        HIGH_QUALITY_MODIFIER,
        BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD,
        BRIGHTNESS_STEP_LIST
    };

    static bool parseInactiveTime(QCBORDecodeContext& context,
                                  KeyboardParameters& keyboardParameters);
    static bool parseSleepTime(QCBORDecodeContext& context, KeyboardParameters& keyboardParameters);
    static bool parseInactiveTimeUSBDisconnected(QCBORDecodeContext& context,
                                                 KeyboardParameters& keyboardParameters);
    static bool parseSleepTimeUSBDisconnected(QCBORDecodeContext& context,
                                              KeyboardParameters& keyboardParameters);
    static bool parsePowerOffTimeUSBDisconnected(QCBORDecodeContext& context,
                                                 KeyboardParameters& keyboardParameters);
    static bool parseLedPowerInactiveLevel(QCBORDecodeContext& context,
                                           KeyboardParameters& keyboardParameters);
    static bool parseLedPowerActiveLevel(QCBORDecodeContext& context,
                                         KeyboardParameters& keyboardParameters);
    static bool parseButtonLongPressDelay(QCBORDecodeContext& context,
                                          KeyboardParameters& keyboardParameters);
    static bool parseButtonRepeatLongPressDelay(QCBORDecodeContext& context,
                                                KeyboardParameters& keyboardParameters);
    static bool parseCleanRefreshPeriod(QCBORDecodeContext& context,
                                        KeyboardParameters& keyboardParameters);
    static bool parseDemoMode(QCBORDecodeContext& context, KeyboardParameters& keyboardParameters);
    static bool parseLowBatteryBlinkOnDelayMs(QCBORDecodeContext& context,
                                              KeyboardParameters& keyboardParameters);
    static bool parseLowBatteryBlinkOffDelayMs(QCBORDecodeContext& context,
                                               KeyboardParameters& keyboardParameters);
    static bool parseLowBatteryLevelThreshold(QCBORDecodeContext& context,
                                              KeyboardParameters& keyboardParameters);
    static bool parseBleBlinkOnDelayMs(QCBORDecodeContext& context,
                                       KeyboardParameters& keyboardParameters);
    static bool parseBleBlinkOffDelayMs(QCBORDecodeContext& context,
                                        KeyboardParameters& keyboardParameters);

    static bool parseHighQualityModifier(QCBORDecodeContext& context,
                                         KeyboardParameters& keyboardParameters);
    static bool parseBlackBackgroundCleanRefreshPeriod(QCBORDecodeContext& context,
                                                       KeyboardParameters& keyboardParameters);
    static bool parseBrightnessStepList(QCBORDecodeContext& context,
                                        KeyboardParameters& keyboardParameters);

    static bool isOptionalParseSuccess(qcbor::Ret ret)
    {
        return ret == qcbor::Ret::SUCCESS || ret == qcbor::Ret::NO_ENT;
    }
};
