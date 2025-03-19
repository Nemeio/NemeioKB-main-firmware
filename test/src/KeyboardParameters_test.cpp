#include "gtest/gtest.h"
#include "KeyboardParametersTestValues.hpp"
#include "KeyboardParameters.hpp"


class KeyboardParametersTest : public ::testing::Test
{
protected:
    etl::vector<uint8_t, KeyboardParameters::BRIGHTNESS_STEP_LIST_MAX_SIZE> mBrightnessDefaultList =
        TEST_BRIGHTNESS_LIST;
};

TEST_F(KeyboardParametersTest, DefaultConstructor)
{
    KeyboardParameters keyboardParameters;

    EXPECT_EQ(keyboardParameters.getInactiveTime(), DEFAULT_INACTIVE_TIME_SEC);
    EXPECT_EQ(keyboardParameters.getSleepTime(), DEFAULT_SLEEP_TIME_SEC);
    EXPECT_EQ(keyboardParameters.getInactiveTimeUSBDisconnected(),
              DEFAULT_INACTIVE_TIME_USB_DISCONNECTED_SEC);
    EXPECT_EQ(keyboardParameters.getSleepTimeUSBDisconnected(),
              DEFAULT_SLEEP_TIME_USB_DISCONNECTED_SEC);
    EXPECT_EQ(keyboardParameters.getPowerOffTimeUSBDisconnected(),
              DEFAULT_POWER_OFF_TIME_USB_DISCONNECTED_SEC);
    EXPECT_EQ(keyboardParameters.getLedPowerActiveLevel(), DEFAULT_LED_POWER_LEVEL);
    EXPECT_EQ(keyboardParameters.getLedPowerInactiveLevel(), DEFAULT_LED_POWER_INACTIVE_LEVEL);
    EXPECT_EQ(keyboardParameters.getButtonLongPressDelay(), DEFAULT_BTN_LONG_PRESS_DELAY);
    EXPECT_EQ(keyboardParameters.getButtonRepeatLongPressDelay(),
              DEFAULT_BTN_REPEAT_LONG_PRESS_DELAY);
    EXPECT_EQ(keyboardParameters.getCleanRefreshPeriod(), DEFAULT_CLEAN_REFRESH_PERIOD);
    EXPECT_EQ(keyboardParameters.isDemoMode(), false);
    EXPECT_EQ(keyboardParameters.getLowBatteryBlinkOnDelayMs(),
              DEFAULT_LOW_BATTERY_BLINK_ON_DELAY_MS);
    EXPECT_EQ(keyboardParameters.getLowBatteryBlinkOffDelayMs(),
              DEFAULT_LOW_BATTERY_BLINK_OFF_DELAY_MS);
    EXPECT_EQ(keyboardParameters.getLowBatteryLevelThresholdPercent(),
              DEFAULT_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT);
    EXPECT_EQ(keyboardParameters.getBleBlinkOnDelayMs(), DEFAULT_BLE_BLINK_ON_DELAY_MS);
    EXPECT_EQ(keyboardParameters.getBleBlinkOffDelayMs(), DEFAULT_BLE_BLINK_OFF_DELAY_MS);
    EXPECT_EQ(keyboardParameters.isHighQualityModifier(), false);
    EXPECT_EQ(keyboardParameters.getBlackBackgroundCleanRefreshPeriod(),
              DEFAULT_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD);
    EXPECT_EQ(keyboardParameters.getBrightnessStepList(), mBrightnessDefaultList);

    EXPECT_TRUE(keyboardParameters.isValid());
}

TEST_F(KeyboardParametersTest, Constructor)
{
    etl::vector<uint8_t, KeyboardParameters::BRIGHTNESS_STEP_LIST_MAX_SIZE> mBrightnessList = {0,
                                                                                               10,
                                                                                               20};

    KeyboardParameters keyboardParameters(TEST_INACTIVE_TIME_SEC,
                                          TEST_SLEEP_TIME_SEC,
                                          TEST_INACTIVE_TIME_USB_DISCONNECTED_SEC,
                                          TEST_SLEEP_TIME_USB_DISCONNECTED_SEC,
                                          TEST_POWER_OFF_TIME_USB_DISCONNECTED_SEC,
                                          TEST_LED_POWER_LEVEL,
                                          TEST_LED_POWER_INACTIVE_LEVEL,
                                          TEST_BTN_LONG_PRESS_DELAY_MS,
                                          TEST_BTN_REPEAT_LONG_PRESS_DELAY_MS,
                                          TEST_CLEAN_REFRESH_PERIOD,
                                          TEST_DEMO_MODE,
                                          TEST_LOW_BATTERY_BLINK_ON_DELAY_MS,
                                          TEST_LOW_BATTERY_BLINK_OFF_DELAY_MS,
                                          TEST_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT,
                                          TEST_BLE_BLINK_ON_DELAY_MS,
                                          TEST_BLE_BLINK_OFF_DELAY_MS,
                                          TEST_HIGH_QUALITY_MODIFIER,
                                          TEST_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD,
                                          mBrightnessList);

    EXPECT_EQ(keyboardParameters.getInactiveTime(), TEST_INACTIVE_TIME_SEC);
    EXPECT_EQ(keyboardParameters.getSleepTime(), TEST_SLEEP_TIME_SEC);
    EXPECT_EQ(keyboardParameters.getInactiveTimeUSBDisconnected(),
              TEST_INACTIVE_TIME_USB_DISCONNECTED_SEC);
    EXPECT_EQ(keyboardParameters.getSleepTimeUSBDisconnected(),
              TEST_SLEEP_TIME_USB_DISCONNECTED_SEC);
    EXPECT_EQ(keyboardParameters.getPowerOffTimeUSBDisconnected(),
              TEST_POWER_OFF_TIME_USB_DISCONNECTED_SEC);
    EXPECT_EQ(keyboardParameters.getLedPowerActiveLevel(), TEST_LED_POWER_LEVEL);
    EXPECT_EQ(keyboardParameters.getLedPowerInactiveLevel(), TEST_LED_POWER_INACTIVE_LEVEL);
    EXPECT_EQ(keyboardParameters.getButtonLongPressDelay(), TEST_BTN_LONG_PRESS_DELAY_MS);
    EXPECT_EQ(keyboardParameters.getButtonRepeatLongPressDelay(),
              TEST_BTN_REPEAT_LONG_PRESS_DELAY_MS);
    EXPECT_EQ(keyboardParameters.getCleanRefreshPeriod(), TEST_CLEAN_REFRESH_PERIOD);
    EXPECT_EQ(keyboardParameters.isDemoMode(), TEST_DEMO_MODE);
    EXPECT_EQ(keyboardParameters.getLowBatteryBlinkOnDelayMs(), TEST_LOW_BATTERY_BLINK_ON_DELAY_MS);
    EXPECT_EQ(keyboardParameters.getLowBatteryBlinkOffDelayMs(),
              TEST_LOW_BATTERY_BLINK_OFF_DELAY_MS);
    EXPECT_EQ(keyboardParameters.getLowBatteryLevelThresholdPercent(),
              TEST_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT);
    EXPECT_EQ(keyboardParameters.getBleBlinkOnDelayMs(), TEST_BLE_BLINK_ON_DELAY_MS);
    EXPECT_EQ(keyboardParameters.getBleBlinkOffDelayMs(), TEST_BLE_BLINK_OFF_DELAY_MS);
    EXPECT_EQ(keyboardParameters.isHighQualityModifier(), TEST_HIGH_QUALITY_MODIFIER);
    EXPECT_EQ(keyboardParameters.getBlackBackgroundCleanRefreshPeriod(),
              TEST_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD);
    EXPECT_EQ(keyboardParameters.getBrightnessStepList(), mBrightnessList);
}

TEST_F(KeyboardParametersTest, Equality)
{
    KeyboardParameters keyboardParameters;
    KeyboardParameters keyboardParameters2;

    EXPECT_TRUE(keyboardParameters == keyboardParameters2);
    EXPECT_FALSE(keyboardParameters != keyboardParameters2);

    KeyboardParameters keyboardParametersInactiveTime;
    keyboardParametersInactiveTime.setInactiveTime(DEFAULT_INACTIVE_TIME_SEC - 1);
    EXPECT_FALSE(keyboardParametersInactiveTime == keyboardParameters);

    KeyboardParameters keyboardParametersSleepTime;
    keyboardParametersSleepTime.setSleepTime(DEFAULT_SLEEP_TIME_SEC - 1);
    EXPECT_FALSE(keyboardParametersSleepTime == keyboardParameters);

    KeyboardParameters keyboardParametersInactiveTimeUSBDisconnected;
    keyboardParametersInactiveTime.setInactiveTimeUSBDisconnected(
        DEFAULT_INACTIVE_TIME_USB_DISCONNECTED_SEC - 1);
    EXPECT_FALSE(keyboardParametersInactiveTime == keyboardParameters);

    KeyboardParameters keyboardParametersSleepTimeUSBDisconnected;
    keyboardParametersSleepTime.setSleepTimeUSBDisconnected(DEFAULT_SLEEP_TIME_USB_DISCONNECTED_SEC
                                                            - 1);
    EXPECT_FALSE(keyboardParametersSleepTime == keyboardParameters);

    KeyboardParameters keyboardParametersPowerOffTime;
    keyboardParametersPowerOffTime.setPowerOffTimeUSBDisconnected(
        DEFAULT_POWER_OFF_TIME_USB_DISCONNECTED_SEC - 1);
    EXPECT_FALSE(keyboardParametersPowerOffTime == keyboardParameters);

    KeyboardParameters keyboardParametersLedPowerMaxLevel;
    keyboardParametersLedPowerMaxLevel.setLedPowerActiveLevel(DEFAULT_LED_POWER_LEVEL - 1);
    EXPECT_FALSE(keyboardParametersLedPowerMaxLevel == keyboardParameters);

    KeyboardParameters keyboardParametersLedPowerInactiveLevel;
    keyboardParametersLedPowerInactiveLevel.setLedPowerInactiveLevel(
        DEFAULT_LED_POWER_INACTIVE_LEVEL - 1);
    EXPECT_FALSE(keyboardParametersLedPowerInactiveLevel == keyboardParameters);

    KeyboardParameters keyboardParametersButtonLongPressDelay;
    keyboardParametersButtonLongPressDelay.setButtonLongPressDelay(DEFAULT_BTN_LONG_PRESS_DELAY - 1);
    EXPECT_FALSE(keyboardParametersButtonLongPressDelay == keyboardParameters);

    KeyboardParameters keyboardParametersButtonRepeatLongPressDelay;
    keyboardParametersButtonRepeatLongPressDelay.setButtonRepeatLongPressDelay(
        DEFAULT_BTN_REPEAT_LONG_PRESS_DELAY - 1);
    EXPECT_FALSE(keyboardParametersButtonRepeatLongPressDelay == keyboardParameters);

    KeyboardParameters keyboardParametersCleanRefreshPeriod;
    keyboardParametersCleanRefreshPeriod.setCleanRefreshPeriod(DEFAULT_CLEAN_REFRESH_PERIOD - 1);
    EXPECT_FALSE(keyboardParametersCleanRefreshPeriod == keyboardParameters);

    KeyboardParameters keyboardParametersDemoMode;
    keyboardParametersDemoMode.setDemoMode(true);
    EXPECT_FALSE(keyboardParametersDemoMode == keyboardParameters);

    KeyboardParameters keyboardParametersLowBatteryBlinkOnDelayMs;
    keyboardParametersLowBatteryBlinkOnDelayMs.setLowBatteryBlinkOnDelayMs(
        DEFAULT_LOW_BATTERY_BLINK_ON_DELAY_MS - 1);
    EXPECT_FALSE(keyboardParametersLowBatteryBlinkOnDelayMs == keyboardParameters);

    KeyboardParameters keyboardParametersLowBatteryBlinkOffDelayMs;
    keyboardParametersLowBatteryBlinkOffDelayMs.setLowBatteryBlinkOffDelayMs(
        DEFAULT_LOW_BATTERY_BLINK_OFF_DELAY_MS - 1);
    EXPECT_FALSE(keyboardParametersLowBatteryBlinkOffDelayMs == keyboardParameters);

    KeyboardParameters keyboardParametersLowBatteryLevelThresholdPercent;
    keyboardParametersLowBatteryLevelThresholdPercent.setLowBatteryLevelThresholdPercent(
        DEFAULT_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT - 1);
    EXPECT_FALSE(keyboardParametersLowBatteryLevelThresholdPercent == keyboardParameters);

    KeyboardParameters keyboardParametersBLEBlinkOnDelayMs;
    keyboardParametersLowBatteryBlinkOnDelayMs.setBleBlinkOnDelayMs(DEFAULT_BLE_BLINK_ON_DELAY_MS
                                                                    - 1);
    EXPECT_FALSE(keyboardParametersLowBatteryBlinkOnDelayMs == keyboardParameters);

    KeyboardParameters keyboardParametersBLEBlinkOffDelayMs;
    keyboardParametersLowBatteryBlinkOffDelayMs.setBleBlinkOffDelayMs(DEFAULT_BLE_BLINK_OFF_DELAY_MS
                                                                      - 1);
    EXPECT_FALSE(keyboardParametersLowBatteryBlinkOffDelayMs == keyboardParameters);

    KeyboardParameters keyboardParametersBlackBackgroundCleanRefreshPeriod;
    keyboardParametersBlackBackgroundCleanRefreshPeriod.setBlackBackgroundCleanRefreshPeriod(
        DEFAULT_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD - 1);
    EXPECT_FALSE(keyboardParametersBlackBackgroundCleanRefreshPeriod == keyboardParameters);

    KeyboardParameters keyboardParametersHighQualityModifier;
    keyboardParametersHighQualityModifier.setHighQualityModifier(true);
    EXPECT_FALSE(keyboardParametersHighQualityModifier == keyboardParameters);
}

TEST_F(KeyboardParametersTest, Assignment)
{
    KeyboardParameters keyboardParameters;
    KeyboardParameters keyboardParameters2(TEST_INACTIVE_TIME_SEC,
                                           TEST_SLEEP_TIME_SEC,
                                           TEST_INACTIVE_TIME_USB_DISCONNECTED_SEC,
                                           TEST_SLEEP_TIME_USB_DISCONNECTED_SEC,
                                           TEST_POWER_OFF_TIME_USB_DISCONNECTED_SEC,
                                           TEST_LED_POWER_LEVEL,
                                           TEST_LED_POWER_INACTIVE_LEVEL,
                                           TEST_BTN_LONG_PRESS_DELAY_MS,
                                           TEST_BTN_REPEAT_LONG_PRESS_DELAY_MS,
                                           TEST_CLEAN_REFRESH_PERIOD,
                                           TEST_DEMO_MODE,
                                           TEST_LOW_BATTERY_BLINK_ON_DELAY_MS,
                                           TEST_LOW_BATTERY_BLINK_OFF_DELAY_MS,
                                           TEST_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT,
                                           TEST_BLE_BLINK_ON_DELAY_MS,
                                           TEST_BLE_BLINK_OFF_DELAY_MS,
                                           TEST_HIGH_QUALITY_MODIFIER,
                                           TEST_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD,
                                           mBrightnessDefaultList);

    EXPECT_FALSE(keyboardParameters == keyboardParameters2);
    keyboardParameters = keyboardParameters2;
    EXPECT_TRUE(keyboardParameters == keyboardParameters2);
}

TEST_F(KeyboardParametersTest, InvalidPowerManagementDelays)
{
    KeyboardParameters keyboardParameters;
    keyboardParameters.setSleepTime(DEFAULT_INACTIVE_TIME_SEC);
    EXPECT_FALSE(keyboardParameters.isValid());

    keyboardParameters = KeyboardParameters();
    keyboardParameters.setSleepTime(DEFAULT_INACTIVE_TIME_SEC - 1);
    EXPECT_FALSE(keyboardParameters.isValid());

    keyboardParameters = KeyboardParameters();
    keyboardParameters.setSleepTime(DelaySec::getMaxSec());
    EXPECT_TRUE(keyboardParameters.isValid());

    keyboardParameters = KeyboardParameters();
    keyboardParameters.setSleepTime(DelaySec::getMaxSec() + 1);
    EXPECT_FALSE(keyboardParameters.isValid());
}

TEST_F(KeyboardParametersTest, InvalidPowerManagementDelaysUSBDisconnected)
{
    KeyboardParameters keyboardParameters;
    keyboardParameters.setSleepTimeUSBDisconnected(DEFAULT_INACTIVE_TIME_USB_DISCONNECTED_SEC);
    EXPECT_FALSE(keyboardParameters.isValid());

    keyboardParameters = KeyboardParameters();
    keyboardParameters.setSleepTimeUSBDisconnected(DEFAULT_INACTIVE_TIME_USB_DISCONNECTED_SEC - 1);
    EXPECT_FALSE(keyboardParameters.isValid());

    keyboardParameters = KeyboardParameters();
    keyboardParameters.setPowerOffTimeUSBDisconnected(DEFAULT_SLEEP_TIME_USB_DISCONNECTED_SEC);
    EXPECT_FALSE(keyboardParameters.isValid());

    keyboardParameters = KeyboardParameters();
    keyboardParameters.setPowerOffTimeUSBDisconnected(DEFAULT_SLEEP_TIME_USB_DISCONNECTED_SEC - 1);
    EXPECT_FALSE(keyboardParameters.isValid());

    keyboardParameters = KeyboardParameters();
    keyboardParameters.setPowerOffTimeUSBDisconnected(DelaySec::getMaxSec());
    EXPECT_TRUE(keyboardParameters.isValid());

    keyboardParameters = KeyboardParameters();
    keyboardParameters.setPowerOffTimeUSBDisconnected(DelaySec::getMaxSec() + 1);
    EXPECT_FALSE(keyboardParameters.isValid());
}

TEST_F(KeyboardParametersTest, InvalidLowBatteryLevelThreshold)
{
    KeyboardParameters keyboardParameters;
    keyboardParameters.setLowBatteryLevelThresholdPercent(MAX_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT);
    EXPECT_TRUE(keyboardParameters.isValid());

    keyboardParameters = KeyboardParameters();
    keyboardParameters.setLowBatteryLevelThresholdPercent(MAX_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT
                                                          + 1);
    EXPECT_FALSE(keyboardParameters.isValid());
}

TEST_F(KeyboardParametersTest, InvalidBrightnessList)
{
    KeyboardParameters keyboardParameters;
    keyboardParameters.setLedPowerActiveLevel(
        32); /* Power max is not in BRIGHTNESS_STEP_LIST_DEFAULT_VALUE */
    keyboardParameters.setBrightnessStepList(mBrightnessDefaultList);
    EXPECT_FALSE(keyboardParameters.isValid());
}
