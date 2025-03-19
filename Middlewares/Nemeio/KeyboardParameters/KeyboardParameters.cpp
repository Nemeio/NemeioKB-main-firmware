/*
 * KeyboardParameters.cpp
 *
 *  Created on: Mar 29, 2018
 *      Author: amongatcheucheu
 */

#include <algorithm>
#include "KeyboardParameters.hpp"

#include "IKeyboardParametersStorage.hpp"

KeyboardParameters::KeyboardParameters()
    : mBrightnessStepList(BRIGHTNESS_STEP_LIST_DEFAULT_VALUE.begin(),
                          BRIGHTNESS_STEP_LIST_DEFAULT_VALUE.end())
{
}

bool KeyboardParameters::operator==(const KeyboardParameters& keyboardParameters) const
{
    return (mInactiveTime == keyboardParameters.mInactiveTime
            && mSleepTime == keyboardParameters.mSleepTime
            && mInactiveTimeUSBDisconnected == keyboardParameters.mInactiveTimeUSBDisconnected
            && mSleepTimeUSBDisconnected == keyboardParameters.mSleepTimeUSBDisconnected
            && mPowerOffTimeUSBDisconnected == keyboardParameters.mPowerOffTimeUSBDisconnected
            && mLedPowerActiveLevel == keyboardParameters.mLedPowerActiveLevel
            && mLedPowerInactiveLevel == keyboardParameters.mLedPowerInactiveLevel
            && mButtonLongPressDelay == keyboardParameters.mButtonLongPressDelay
            && mButtonRepeatLongPressDelay == keyboardParameters.mButtonRepeatLongPressDelay
            && mCleanRefreshPeriod == keyboardParameters.mCleanRefreshPeriod
            && mDemoMode == keyboardParameters.mDemoMode
            && mLowBatteryBlinkOnDelayMs == keyboardParameters.mLowBatteryBlinkOnDelayMs
            && mLowBatteryBlinkOffDelayMs == keyboardParameters.mLowBatteryBlinkOffDelayMs
            && mLowBatteryLevelThresholdPercent
                   == keyboardParameters.mLowBatteryLevelThresholdPercent
            && mBLEBlinkOnDelayMs == keyboardParameters.mBLEBlinkOnDelayMs
            && mBLEBlinkOffDelayMs == keyboardParameters.mBLEBlinkOffDelayMs
            && mHighQualityModifier == keyboardParameters.mHighQualityModifier
            && mBlackBackgroundCleanRefreshPeriod
                   == keyboardParameters.mBlackBackgroundCleanRefreshPeriod
            && mBrightnessStepList == keyboardParameters.mBrightnessStepList);
}

bool KeyboardParameters::operator!=(const KeyboardParameters& keyboardParameters) const
{
    return !(keyboardParameters == *this);
}

bool KeyboardParameters::isValid() const
{
    return (
        mInactiveTime < mSleepTime && mSleepTime <= MAX_POWER_MANAGEMENT_TIME_SEC
        && mInactiveTimeUSBDisconnected < mSleepTimeUSBDisconnected
        && mSleepTimeUSBDisconnected < mPowerOffTimeUSBDisconnected
        && mPowerOffTimeUSBDisconnected <= MAX_POWER_MANAGEMENT_TIME_SEC
        && mLedPowerInactiveLevel < MAX_LED_POWER_LEVEL
        && mLedPowerActiveLevel <= MAX_LED_POWER_LEVEL
        && mButtonLongPressDelay <= MAX_BTN_LONG_PRESS_DELAY
        && mButtonRepeatLongPressDelay <= MAX_BTN_REPEAT_LONG_PRESS_DELAY
        && mCleanRefreshPeriod <= MAX_CLEAN_REFRESH_PERIOD
        && mLowBatteryBlinkOnDelayMs <= MAX_LOW_BATTERY_BLINK_DELAY_MS
        && mLowBatteryBlinkOffDelayMs <= MAX_LOW_BATTERY_BLINK_DELAY_MS
        && mLowBatteryLevelThresholdPercent <= MAX_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT
        && mBLEBlinkOnDelayMs <= MAX_BLE_BLINK_DELAY_MS
        && mBLEBlinkOffDelayMs <= MAX_BLE_BLINK_DELAY_MS && !mBrightnessStepList.empty()
        && mBlackBackgroundCleanRefreshPeriod <= MAX_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD
        && std::is_sorted(mBrightnessStepList.begin(), mBrightnessStepList.end())
        && etl::find(mBrightnessStepList.begin(), mBrightnessStepList.end(), mLedPowerActiveLevel)
               != mBrightnessStepList.end());
}

uint32_t KeyboardParameters::getInactiveTime() const
{
    return mInactiveTime;
}

uint32_t KeyboardParameters::getSleepTime() const
{
    return mSleepTime;
}

uint32_t KeyboardParameters::getPowerOffTimeUSBDisconnected() const
{
    return mPowerOffTimeUSBDisconnected;
}

uint8_t KeyboardParameters::getLedPowerActiveLevel() const
{
    return mLedPowerActiveLevel;
}

uint16_t KeyboardParameters::getButtonLongPressDelay() const
{
    return mButtonLongPressDelay;
}

uint16_t KeyboardParameters::getButtonRepeatLongPressDelay() const
{
    return mButtonRepeatLongPressDelay;
}

uint16_t KeyboardParameters::getLowBatteryBlinkOffDelayMs() const
{
    return mLowBatteryBlinkOffDelayMs;
}

void KeyboardParameters::setButtonLongPressDelay(uint16_t buttonLongPressDelay)
{
    mButtonLongPressDelay = buttonLongPressDelay;
}

void KeyboardParameters::setButtonRepeatLongPressDelay(uint16_t buttonRepeatLongPressDelay)
{
    mButtonRepeatLongPressDelay = buttonRepeatLongPressDelay;
}

void KeyboardParameters::setCleanRefreshPeriod(uint8_t cleanRefreshPeriod)
{
    mCleanRefreshPeriod = cleanRefreshPeriod;
}

void KeyboardParameters::setDemoMode(bool demoMode)
{
    mDemoMode = demoMode;
}

void KeyboardParameters::setInactiveTime(int inactiveTime)
{
    mInactiveTime = inactiveTime;
}

void KeyboardParameters::setLedPowerInactiveLevel(uint8_t ledPowerInactiveLevel)
{
    mLedPowerInactiveLevel = ledPowerInactiveLevel;
}

void KeyboardParameters::setLedPowerActiveLevel(uint8_t ledPowerActiveLevel)
{
    mLedPowerActiveLevel = ledPowerActiveLevel;
}

void KeyboardParameters::setLowBatteryBlinkOffDelayMs(uint16_t lowBatteryBlinkOffDelayMs)
{
    mLowBatteryBlinkOffDelayMs = lowBatteryBlinkOffDelayMs;
}

void KeyboardParameters::setLowBatteryBlinkOnDelayMs(uint16_t lowBatteryBlinkOnDelayMs)
{
    mLowBatteryBlinkOnDelayMs = lowBatteryBlinkOnDelayMs;
}

void KeyboardParameters::setSleepTime(uint32_t sleepTime)
{
    mSleepTime = sleepTime;
}

uint16_t KeyboardParameters::getLowBatteryBlinkOnDelayMs() const
{
    return mLowBatteryBlinkOnDelayMs;
}

bool KeyboardParameters::isDemoMode() const
{
    return mDemoMode;
}

uint8_t KeyboardParameters::getLedPowerInactiveLevel() const
{
    return mLedPowerInactiveLevel;
}

uint8_t KeyboardParameters::getCleanRefreshPeriod() const
{
    return mCleanRefreshPeriod;
}

KeyboardParameters::KeyboardParameters(
    uint32_t inactiveTime,
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
    bool highQualityModifier,
    uint8_t blackBackgroundCleanRefreshPeriod,
    const etl::vector<uint8_t, BRIGHTNESS_STEP_LIST_MAX_SIZE>& brightnessList)
    : mInactiveTime(inactiveTime)
    , mSleepTime(sleepTime)
    , mInactiveTimeUSBDisconnected(inactiveTimeUSBDisconnected)
    , mSleepTimeUSBDisconnected(sleepTimeUSBDisconnected)
    , mPowerOffTimeUSBDisconnected(powerOffTimeUSBDisconnected)
    , mLedPowerActiveLevel(ledPowerMaxLevel)
    , mLedPowerInactiveLevel(ledPowerInactiveLevel)
    , mButtonLongPressDelay(buttonLongPressDelay)
    , mButtonRepeatLongPressDelay(buttonRepeatLongPressDelay)
    , mCleanRefreshPeriod(cleanRefreshPeriod)
    , mDemoMode(demoMode)
    , mLowBatteryBlinkOnDelayMs(lowBatteryBlinkOnDelayMs)
    , mLowBatteryBlinkOffDelayMs(lowBatteryBlinkOffDelayMs)
    , mLowBatteryLevelThresholdPercent(lowBatteryLevelThreshold)
    , mBLEBlinkOnDelayMs(bleBlinkOnDelayMs)
    , mBLEBlinkOffDelayMs(bleBlinkOffDelayMs)
    , mHighQualityModifier(highQualityModifier)
    , mBlackBackgroundCleanRefreshPeriod(blackBackgroundCleanRefreshPeriod)
    , mBrightnessStepList(brightnessList)
{
}

uint8_t KeyboardParameters::getLowBatteryLevelThresholdPercent() const
{
    return mLowBatteryLevelThresholdPercent;
}

void KeyboardParameters::setLowBatteryLevelThresholdPercent(uint8_t lowBatteryLevelThresholdPercent)
{
    mLowBatteryLevelThresholdPercent = lowBatteryLevelThresholdPercent;
}

uint16_t KeyboardParameters::getBleBlinkOffDelayMs() const
{
    return mBLEBlinkOffDelayMs;
}

void KeyboardParameters::setBleBlinkOffDelayMs(uint16_t bleBlinkOffDelayMs)
{
    mBLEBlinkOffDelayMs = bleBlinkOffDelayMs;
}

uint16_t KeyboardParameters::getBleBlinkOnDelayMs() const
{
    return mBLEBlinkOnDelayMs;
}

void KeyboardParameters::setBleBlinkOnDelayMs(uint16_t bleBlinkOnDelayMs)
{
    mBLEBlinkOnDelayMs = bleBlinkOnDelayMs;
}

uint32_t KeyboardParameters::getInactiveTimeUSBDisconnected() const
{
    return mInactiveTimeUSBDisconnected;
}

uint32_t KeyboardParameters::getSleepTimeUSBDisconnected() const
{
    return mSleepTimeUSBDisconnected;
}

void KeyboardParameters::setPowerOffTimeUSBDisconnected(uint32_t powerOffTimeUSBDisconnected)
{
    mPowerOffTimeUSBDisconnected = powerOffTimeUSBDisconnected;
}

void KeyboardParameters::setInactiveTimeUSBDisconnected(uint32_t inactiveTimeUSBDisconnected)
{
    mInactiveTimeUSBDisconnected = inactiveTimeUSBDisconnected;
}

void KeyboardParameters::setSleepTimeUSBDisconnected(uint32_t sleepTimeUSBDisconnected)
{
    mSleepTimeUSBDisconnected = sleepTimeUSBDisconnected;
}

void KeyboardParameters::setHighQualityModifier(bool highQualityModifier)
{
    mHighQualityModifier = highQualityModifier;
}

bool KeyboardParameters::isHighQualityModifier() const
{
    return mHighQualityModifier;
}

const etl::vector<uint8_t, KeyboardParameters::BRIGHTNESS_STEP_LIST_MAX_SIZE>&
KeyboardParameters::getBrightnessStepList() const
{
    return mBrightnessStepList;
}

void KeyboardParameters::setBrightnessStepList(
    const etl::vector<uint8_t, BRIGHTNESS_STEP_LIST_MAX_SIZE>& list)
{
    mBrightnessStepList = list;
}

uint8_t KeyboardParameters::getBlackBackgroundCleanRefreshPeriod() const
{
    return mBlackBackgroundCleanRefreshPeriod;
}

void KeyboardParameters::setBlackBackgroundCleanRefreshPeriod(
    uint8_t blackBackgroundCleanRefreshPeriod)
{
    mBlackBackgroundCleanRefreshPeriod = blackBackgroundCleanRefreshPeriod;
}