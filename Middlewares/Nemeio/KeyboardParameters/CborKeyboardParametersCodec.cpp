#include "CborKeyboardParametersCodec.hpp"
#include "qcbor/qcbor_spiffy_decode.h"
#include "etl/limits.h"

bool CborKeyboardParametersCodec::decode(etl::array_view<const std::byte> data,
                                         KeyboardParameters& keyboardParameters) const
{
    UsefulBufC inputBuffer = {data.data(), data.size()};
    QCBORDecodeContext context;
    QCBORDecode_Init(&context, inputBuffer, QCBOR_DECODE_MODE_NORMAL);

    bool ret = decodeFromContext(context, keyboardParameters);

    QCBORDecode_Finish(&context);

    return ret;
}

bool CborKeyboardParametersCodec::decodeFromContext(QCBORDecodeContext& context,
                                                    KeyboardParameters& keyboardParameters)
{
    QCBORItem item;

    QCBORDecode_EnterMap(&context, &item);
    bool success = !QCBORDecode_GetAndResetError(&context);

    etl::optional<uint32_t> inactiveTime;
    etl::optional<uint32_t> sleepTime;
    etl::optional<uint32_t> inactiveTimeUsbDisconnected;
    etl::optional<uint32_t> sleepTimeUsbDisconnected;
    etl::optional<uint32_t> powerOffTimeUsbDisconnected;
    etl::optional<uint8_t> ledPowerMaxLevel;
    etl::optional<uint8_t> ledPowerInactiveLevel;
    etl::optional<uint16_t> buttonLongPressDelay;
    etl::optional<uint16_t> buttonRepeatLongPressDelay;
    etl::optional<uint8_t> cleanRefreshPeriod;
    etl::optional<bool> demoMode;
    etl::optional<uint16_t> lowBatteryBlinkOnDelayMs;
    etl::optional<uint16_t> lowBatteryBlinkOffDelayMs;
    etl::optional<uint8_t> lowBatteryLevelThreshold;
    etl::optional<uint16_t> bleBlinkOnDelayMs;
    etl::optional<uint16_t> bleBlinkOffDelayMs;
    etl::optional<bool> highQualityModifier;
    etl::optional<uint8_t> blackBackgroundCleanRefreshPeriod;

    success = success && parseInactiveTime(context, keyboardParameters)
              && parseSleepTime(context, keyboardParameters)
              && parseInactiveTimeUSBDisconnected(context, keyboardParameters)
              && parseSleepTimeUSBDisconnected(context, keyboardParameters)
              && parsePowerOffTimeUSBDisconnected(context, keyboardParameters)
              && parseLedPowerActiveLevel(context, keyboardParameters)
              && parseLedPowerInactiveLevel(context, keyboardParameters)
              && parseButtonLongPressDelay(context, keyboardParameters)
              && parseButtonRepeatLongPressDelay(context, keyboardParameters)
              && parseCleanRefreshPeriod(context, keyboardParameters)
              && parseDemoMode(context, keyboardParameters)
              && parseLowBatteryBlinkOnDelayMs(context, keyboardParameters)
              && parseLowBatteryBlinkOffDelayMs(context, keyboardParameters)
              && parseLowBatteryLevelThreshold(context, keyboardParameters)
              && parseBleBlinkOnDelayMs(context, keyboardParameters)
              && parseBleBlinkOffDelayMs(context, keyboardParameters)
              && parseHighQualityModifier(context, keyboardParameters)
              && parseBlackBackgroundCleanRefreshPeriod(context, keyboardParameters)
              && parseBrightnessStepList(context, keyboardParameters);

    QCBORDecode_ExitMap(&context);
    success = !QCBORDecode_GetAndResetError(&context) && success;

    return success;
}

ssize_t CborKeyboardParametersCodec::encode(const KeyboardParameters& keyboardParameters,
                                            etl::array_view<std::byte> data) const
{
    UsefulBuf outputBuffer = {data.data(), data.size()};
    QCBOREncodeContext context;
    QCBOREncode_Init(&context, outputBuffer);

    encodeFromContext(context, keyboardParameters);

    size_t cborSize;
    QCBORError err = QCBOREncode_FinishGetSize(&context, &cborSize);
    ssize_t ret = -1;
    if(!err && cborSize <= etl::numeric_limits<ssize_t>::max())
    {
        ret = static_cast<ssize_t>(cborSize);
    }

    return ret;
}

bool CborKeyboardParametersCodec::encodeFromContext(QCBOREncodeContext& context,
                                                    const KeyboardParameters& keyboardParameters)
{
    QCBOREncode_OpenMap(&context);

    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::INACTIVE_TIME),
                                keyboardParameters.getInactiveTime());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::SLEEP_TIME),
                                keyboardParameters.getSleepTime());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::INACTIVE_TIME_USB_DISCONNECTED),
                                keyboardParameters.getInactiveTimeUSBDisconnected());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::SLEEP_TIME_USB_DISCONNECTED),
                                keyboardParameters.getSleepTimeUSBDisconnected());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::POWER_OFF_TIME_USB_DISCONNECTED),
                                keyboardParameters.getPowerOffTimeUSBDisconnected());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::LED_POWER_ACTIVE_LEVEL),
                                keyboardParameters.getLedPowerActiveLevel());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::LED_POWER_INACTIVE_LEVEL),
                                keyboardParameters.getLedPowerInactiveLevel());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::BUTTON_LONG_PRESS_DELAY),
                                keyboardParameters.getButtonLongPressDelay());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::BUTTON_REPEAT_LONG_PRESS_DELAY),
                                keyboardParameters.getButtonRepeatLongPressDelay());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::CLEAN_REFRESH_PERIOD),
                                keyboardParameters.getCleanRefreshPeriod());
    QCBOREncode_AddBoolToMapN(&context,
                              static_cast<int64_t>(CborId::DEMO_MODE),
                              keyboardParameters.isDemoMode());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::LOW_BATTERY_BLINK_ON_DELAY_MS),
                                keyboardParameters.getLowBatteryBlinkOnDelayMs());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::LOW_BATTERY_BLINK_OFF_DELAY_MS),
                                keyboardParameters.getLowBatteryBlinkOffDelayMs());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::LOW_BATTERY_LEVEL_THRESHOLD),
                                keyboardParameters.getLowBatteryLevelThresholdPercent());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::BLE_BLINK_ON_DELAY_MS),
                                keyboardParameters.getBleBlinkOnDelayMs());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::BLE_BLINK_OFF_DELAY_MS),
                                keyboardParameters.getBleBlinkOffDelayMs());
    QCBOREncode_AddBoolToMapN(&context,
                              static_cast<int64_t>(CborId::HIGH_QUALITY_MODIFIER),
                              keyboardParameters.isHighQualityModifier());
    QCBOREncode_AddUInt64ToMapN(&context,
                                static_cast<int64_t>(CborId::BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD),
                                keyboardParameters.getBlackBackgroundCleanRefreshPeriod());

    QCBOREncode_OpenArrayInMapN(&context, static_cast<int64_t>(CborId::BRIGHTNESS_STEP_LIST));
    for(auto& step: keyboardParameters.getBrightnessStepList())
    {
        QCBOREncode_AddInt64(&context, step);
    }
    QCBOREncode_CloseArray(&context);

    QCBOREncode_CloseMap(&context);

    return !QCBOREncode_GetErrorState(&context);
}

bool CborKeyboardParametersCodec::parseInactiveTime(QCBORDecodeContext& context,
                                                    KeyboardParameters& keyboardParameters)
{
    etl::optional<uint32_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint32_t>(context,
                                                           static_cast<int64_t>(
                                                               CborId::INACTIVE_TIME),
                                                           value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setInactiveTime(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseSleepTime(QCBORDecodeContext& context,
                                                 KeyboardParameters& keyboardParameters)
{
    etl::optional<uint32_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint32_t>(context,
                                                           static_cast<int64_t>(CborId::SLEEP_TIME),
                                                           value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setSleepTime(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseInactiveTimeUSBDisconnected(
    QCBORDecodeContext& context, KeyboardParameters& keyboardParameters)
{
    etl::optional<uint32_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint32_t>(
        context, static_cast<int64_t>(CborId::INACTIVE_TIME_USB_DISCONNECTED), value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setInactiveTimeUSBDisconnected(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseSleepTimeUSBDisconnected(
    QCBORDecodeContext& context, KeyboardParameters& keyboardParameters)
{
    etl::optional<uint32_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint32_t>(context,
                                                           static_cast<int64_t>(
                                                               CborId::SLEEP_TIME_USB_DISCONNECTED),
                                                           value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setSleepTimeUSBDisconnected(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parsePowerOffTimeUSBDisconnected(
    QCBORDecodeContext& context, KeyboardParameters& keyboardParameters)
{
    etl::optional<uint32_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint32_t>(
        context, static_cast<int64_t>(CborId::POWER_OFF_TIME_USB_DISCONNECTED), value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setPowerOffTimeUSBDisconnected(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseLedPowerInactiveLevel(QCBORDecodeContext& context,
                                                             KeyboardParameters& keyboardParameters)
{
    etl::optional<uint8_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint8_t>(context,
                                                          static_cast<int64_t>(
                                                              CborId::LED_POWER_INACTIVE_LEVEL),
                                                          value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setLedPowerInactiveLevel(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseLedPowerActiveLevel(QCBORDecodeContext& context,
                                                           KeyboardParameters& keyboardParameters)
{
    etl::optional<uint8_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint8_t>(context,
                                                          static_cast<int64_t>(
                                                              CborId::LED_POWER_ACTIVE_LEVEL),
                                                          value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setLedPowerActiveLevel(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseButtonLongPressDelay(QCBORDecodeContext& context,
                                                            KeyboardParameters& keyboardParameters)
{
    etl::optional<uint16_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint16_t>(context,
                                                           static_cast<int64_t>(
                                                               CborId::BUTTON_LONG_PRESS_DELAY),
                                                           value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setButtonLongPressDelay(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseButtonRepeatLongPressDelay(
    QCBORDecodeContext& context, KeyboardParameters& keyboardParameters)
{
    etl::optional<uint16_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint16_t>(
        context, static_cast<int64_t>(CborId::BUTTON_REPEAT_LONG_PRESS_DELAY), value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setButtonRepeatLongPressDelay(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseCleanRefreshPeriod(QCBORDecodeContext& context,
                                                          KeyboardParameters& keyboardParameters)
{
    etl::optional<uint8_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint8_t>(context,
                                                          static_cast<int64_t>(
                                                              CborId::CLEAN_REFRESH_PERIOD),
                                                          value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setCleanRefreshPeriod(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseDemoMode(QCBORDecodeContext& context,
                                                KeyboardParameters& keyboardParameters)
{
    etl::optional<bool> value;
    qcbor::Ret ret = qcbor::Util::parseBoolFromN(context,
                                                 static_cast<int64_t>(CborId::DEMO_MODE),
                                                 value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setDemoMode(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseLowBatteryBlinkOnDelayMs(
    QCBORDecodeContext& context, KeyboardParameters& keyboardParameters)
{
    etl::optional<uint16_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint16_t>(context,
                                                           static_cast<int64_t>(
                                                               CborId::LOW_BATTERY_BLINK_ON_DELAY_MS),
                                                           value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setLowBatteryBlinkOnDelayMs(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseLowBatteryBlinkOffDelayMs(
    QCBORDecodeContext& context, KeyboardParameters& keyboardParameters)
{
    etl::optional<uint16_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint16_t>(
        context, static_cast<int64_t>(CborId::LOW_BATTERY_BLINK_OFF_DELAY_MS), value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setLowBatteryBlinkOffDelayMs(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseLowBatteryLevelThreshold(
    QCBORDecodeContext& context, KeyboardParameters& keyboardParameters)
{
    etl::optional<uint8_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint8_t>(context,
                                                          static_cast<int64_t>(
                                                              CborId::LOW_BATTERY_LEVEL_THRESHOLD),
                                                          value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setLowBatteryLevelThresholdPercent(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseBleBlinkOnDelayMs(QCBORDecodeContext& context,
                                                         KeyboardParameters& keyboardParameters)
{
    etl::optional<uint16_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint16_t>(context,
                                                           static_cast<int64_t>(
                                                               CborId::BLE_BLINK_ON_DELAY_MS),
                                                           value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setBleBlinkOnDelayMs(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseBleBlinkOffDelayMs(QCBORDecodeContext& context,
                                                          KeyboardParameters& keyboardParameters)
{
    etl::optional<uint16_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint16_t>(context,
                                                           static_cast<int64_t>(
                                                               CborId::BLE_BLINK_OFF_DELAY_MS),
                                                           value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setBleBlinkOffDelayMs(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseHighQualityModifier(QCBORDecodeContext& context,
                                                           KeyboardParameters& keyboardParameters)
{
    etl::optional<bool> value;
    qcbor::Ret ret = qcbor::Util::parseBoolFromN(context,
                                                 static_cast<int64_t>(CborId::HIGH_QUALITY_MODIFIER),
                                                 value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setHighQualityModifier(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseBlackBackgroundCleanRefreshPeriod(
    QCBORDecodeContext& context, KeyboardParameters& keyboardParameters)
{
    etl::optional<uint8_t> value;
    qcbor::Ret ret = qcbor::Util::parseUintFromN<uint8_t>(
        context, static_cast<int64_t>(CborId::BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD), value);
    if(ret == qcbor::Ret::SUCCESS && value.has_value())
    {
        keyboardParameters.setBlackBackgroundCleanRefreshPeriod(value.value());
    }

    return isOptionalParseSuccess(ret);
}

bool CborKeyboardParametersCodec::parseBrightnessStepList(QCBORDecodeContext& context,
                                                          KeyboardParameters& keyboardParameters)
{
    etl::vector<uint8_t, KeyboardParameters::BRIGHTNESS_STEP_LIST_MAX_SIZE> list;

    auto addItem = [&list](QCBORDecodeContext& c)
    {
        etl::optional<uint8_t> value;
        bool addItemSuccess = !list.full()
                              && qcbor::Util::parseUint<uint8_t>(c, value) == qcbor::Ret::SUCCESS;
        if(addItemSuccess)
        {
            list.push_back(value.value());
        }
        return addItemSuccess;
    };
    qcbor::Ret ret = qcbor::Util::parseArrayFromN(context,
                                                  static_cast<int64_t>(CborId::BRIGHTNESS_STEP_LIST),
                                                  addItem);

    if(ret == qcbor::Ret::SUCCESS)
    {
        keyboardParameters.setBrightnessStepList(list);
    }

    return isOptionalParseSuccess(ret);
}