/*
 * KeyboardParameters.hpp
 *
 *  Created on: Mar 29, 2018
 *      Author: amongatcheucheu
 */

#ifndef KEYBOARDPARAMETERS_KEYBOARDPARAMETERS_HPP_
#define KEYBOARDPARAMETERS_KEYBOARDPARAMETERS_HPP_

#include <stddef.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <sstream>
#include "SystemTypes.hpp"

#include "etl/vector.h"

class KeyboardParameters
{
public:
    static constexpr uint16_t BRIGHTNESS_STEP_LIST_MAX_SIZE = UINT8_MAX + 1;

    KeyboardParameters();
    KeyboardParameters(uint32_t inactiveTime,
                       uint32_t sleepTime,
                       uint32_t inactiveTimeUSBDisconnected,
                       uint32_t sleepTimeUSBDisconnected,
                       uint32_t powerOffTimeUSBDisconnected,
                       uint8_t ledPowerMaxLevel,
                       uint8_t ledPowerInactiveLevel,
                       uint16_t buttonLongPressDelay,
                       uint16_t buttonRepeatLongPressDelay,
                       uint8_t cleanRefreshPeriod,
                       bool demoMode,
                       uint16_t lowBatteryBlinkOnDelayMs,
                       uint16_t lowBatteryBlinkOffDelayMs,
                       uint8_t lowBatteryLevelThreshold,
                       uint16_t bleBlinkOnDelayMs,
                       uint16_t bleBlinkOffDelayMs,
                       bool highQualityMode,
                       uint8_t blackBackgroundCleanRefreshPeriod,
                       const etl::vector<uint8_t, BRIGHTNESS_STEP_LIST_MAX_SIZE>& brightnessList);
    virtual ~KeyboardParameters() = default;

    uint32_t getInactiveTime() const;
    uint32_t getSleepTime() const;
    uint32_t getInactiveTimeUSBDisconnected() const;
    uint32_t getSleepTimeUSBDisconnected() const;
    uint32_t getPowerOffTimeUSBDisconnected() const;
    bool isHighQualityModifier() const;

    bool operator==(const KeyboardParameters& keyboardParameters) const;
    bool operator!=(const KeyboardParameters& keyboardParameters) const;
    bool isValid() const;

    uint8_t getLedPowerActiveLevel() const;
    uint8_t getCleanRefreshPeriod() const;
    uint16_t getButtonLongPressDelay() const;
    uint16_t getButtonRepeatLongPressDelay() const;
    uint8_t getLedPowerInactiveLevel() const;
    bool isDemoMode() const;
    uint16_t getLowBatteryBlinkOffDelayMs() const;
    uint16_t getLowBatteryBlinkOnDelayMs() const;
    const etl::vector<uint8_t, BRIGHTNESS_STEP_LIST_MAX_SIZE>& getBrightnessStepList() const;
    uint8_t getBlackBackgroundCleanRefreshPeriod() const;

    void setButtonLongPressDelay(uint16_t buttonLongPressDelay);
    void setButtonRepeatLongPressDelay(uint16_t buttonRepeatLongPressDelay);
    void setCleanRefreshPeriod(uint8_t cleanRefreshPeriod);
    void setDemoMode(bool demoMode);
    void setInactiveTime(int inactiveTime);
    void setLedPowerInactiveLevel(uint8_t ledPowerInactiveLevel);
    void setLedPowerActiveLevel(uint8_t ledPowerActiveLevel);
    void setLowBatteryBlinkOffDelayMs(uint16_t lowBatteryBlinkOffDelayMs);
    void setLowBatteryBlinkOnDelayMs(uint16_t lowBatteryBlinkOnDelayMs);
    void setPowerOffTimeUSBDisconnected(uint32_t powerOffTimeUSBDisconnected);
    void setSleepTime(uint32_t sleepTime);
    uint8_t getLowBatteryLevelThresholdPercent() const;
    void setLowBatteryLevelThresholdPercent(uint8_t lowBatteryLevelThresholdPercent);
    uint16_t getBleBlinkOffDelayMs() const;
    void setBleBlinkOffDelayMs(uint16_t bleBlinkOffDelayMs);
    uint16_t getBleBlinkOnDelayMs() const;
    void setBleBlinkOnDelayMs(uint16_t bleBlinkOnDelayMs);
    void setInactiveTimeUSBDisconnected(uint32_t inactiveTimeUSBDisconnected);
    void setSleepTimeUSBDisconnected(uint32_t sleepTimeUSBDisconnected);
    void setHighQualityModifier(bool highQualityModifier);
    void setBrightnessStepList(const etl::vector<uint8_t, BRIGHTNESS_STEP_LIST_MAX_SIZE>& list);
    void setBlackBackgroundCleanRefreshPeriod(uint8_t blackBackgroundCleanRefreshPeriod);

private:
    static constexpr int32_t MAX_POWER_MANAGEMENT_TIME_SEC = DelaySec::getMaxSec();
    static constexpr uint32_t DEFAULT_INACTIVE_TIME_SEC = 12;
    static constexpr uint32_t DEFAULT_SLEEP_TIME_SEC = 60;
    static constexpr uint32_t DEFAULT_INACTIVE_TIME_USB_DISCONNECTED_SEC = 12;
    static constexpr uint32_t DEFAULT_SLEEP_TIME_USB_DISCONNECTED_SEC = 60;
    static constexpr uint32_t DEFAULT_POWER_OFF_TIME_USB_DISCONNECTED_SEC = 300;

    static constexpr uint8_t MAX_LED_POWER_LEVEL = 100;
    static constexpr uint8_t DEFAULT_LED_POWER_ACTIVE_PERCENT = 50;
    static constexpr uint8_t DEFAULT_LED_POWER_INACTIVE_PERCENT_ACTIVE =
        50; /* 50% of DEFAULT_LED_POWER_ACTIVE_PERCENT */

    static constexpr uint8_t DEFAULT_CLEAN_REFRESH_PERIOD = 10;
    static constexpr uint8_t MAX_CLEAN_REFRESH_PERIOD = 255;

    static constexpr uint16_t DEFAULT_BTN_LONG_PRESS_DELAY = 1500;
    static constexpr uint16_t MAX_BTN_LONG_PRESS_DELAY = 10000;

    static constexpr uint16_t DEFAULT_BTN_REPEAT_LONG_PRESS_DELAY = 250;
    static constexpr uint16_t MAX_BTN_REPEAT_LONG_PRESS_DELAY = 10000;

    static constexpr uint16_t DEFAULT_LOW_BATTERY_BLINK_ON_DELAY_MS = 100;
    static constexpr uint16_t DEFAULT_LOW_BATTERY_BLINK_OFF_DELAY_MS = 900;
    static constexpr uint16_t MAX_LOW_BATTERY_BLINK_DELAY_MS = UINT16_MAX;

    static constexpr uint16_t DEFAULT_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT = 20;
    static constexpr uint16_t MAX_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT = 100;

    static constexpr uint16_t DEFAULT_BLE_BLINK_ON_DELAY_MS = 200;
    static constexpr uint16_t DEFAULT_BLE_BLINK_OFF_DELAY_MS = 1800;
    static constexpr uint16_t MAX_BLE_BLINK_DELAY_MS = UINT16_MAX;

    static constexpr uint8_t DEFAULT_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD = 10;
    static constexpr uint8_t MAX_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD = 255;

    static constexpr std::array<uint8_t, 9> BRIGHTNESS_STEP_LIST_DEFAULT_VALUE =
        {20, 30, 40, DEFAULT_LED_POWER_ACTIVE_PERCENT, 60, 70, 80, 90, 100};

    /* Default value must be well placed in brightness list */
    static_assert(BRIGHTNESS_STEP_LIST_DEFAULT_VALUE[3] == DEFAULT_LED_POWER_ACTIVE_PERCENT);
    static_assert(BRIGHTNESS_STEP_LIST_DEFAULT_VALUE[2] < DEFAULT_LED_POWER_ACTIVE_PERCENT);
    static_assert(BRIGHTNESS_STEP_LIST_DEFAULT_VALUE[4] > DEFAULT_LED_POWER_ACTIVE_PERCENT);

    uint32_t mInactiveTime = DEFAULT_INACTIVE_TIME_SEC;
    uint32_t mSleepTime = DEFAULT_SLEEP_TIME_SEC;
    uint32_t mInactiveTimeUSBDisconnected = DEFAULT_INACTIVE_TIME_USB_DISCONNECTED_SEC;
    uint32_t mSleepTimeUSBDisconnected = DEFAULT_SLEEP_TIME_USB_DISCONNECTED_SEC;
    uint32_t mPowerOffTimeUSBDisconnected = DEFAULT_POWER_OFF_TIME_USB_DISCONNECTED_SEC;
    uint8_t mLedPowerActiveLevel = DEFAULT_LED_POWER_ACTIVE_PERCENT;
    uint8_t mLedPowerInactiveLevel =
        DEFAULT_LED_POWER_INACTIVE_PERCENT_ACTIVE; /* This is a percentage of PowerMax */
    uint16_t mButtonLongPressDelay = DEFAULT_BTN_LONG_PRESS_DELAY;
    uint16_t mButtonRepeatLongPressDelay = DEFAULT_BTN_REPEAT_LONG_PRESS_DELAY;
    uint8_t mCleanRefreshPeriod = DEFAULT_CLEAN_REFRESH_PERIOD;
    bool mDemoMode = false;
    uint16_t mLowBatteryBlinkOnDelayMs = DEFAULT_LOW_BATTERY_BLINK_ON_DELAY_MS;
    uint16_t mLowBatteryBlinkOffDelayMs = DEFAULT_LOW_BATTERY_BLINK_OFF_DELAY_MS;
    uint8_t mLowBatteryLevelThresholdPercent = DEFAULT_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT;
    uint16_t mBLEBlinkOnDelayMs = DEFAULT_BLE_BLINK_ON_DELAY_MS;
    uint16_t mBLEBlinkOffDelayMs = DEFAULT_BLE_BLINK_OFF_DELAY_MS;
    bool mHighQualityModifier = false;
    uint8_t mBlackBackgroundCleanRefreshPeriod = DEFAULT_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD;
    etl::vector<uint8_t, BRIGHTNESS_STEP_LIST_MAX_SIZE> mBrightnessStepList;
};

#endif /* KEYBOARDPARAMETERS_KEYBOARDPARAMETERS_HPP_ */
