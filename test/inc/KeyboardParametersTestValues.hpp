#ifndef KEYBOARD_PARAMETERS_TEST_VALUES
#define KEYBOARD_PARAMETERS_TEST_VALUES

#include <cstdint>

static constexpr uint32_t DEFAULT_INACTIVE_TIME_SEC = 12;
static constexpr uint32_t DEFAULT_SLEEP_TIME_SEC = 60;
static constexpr uint32_t DEFAULT_INACTIVE_TIME_USB_DISCONNECTED_SEC = 12;
static constexpr uint32_t DEFAULT_SLEEP_TIME_USB_DISCONNECTED_SEC = 60;
static constexpr uint32_t DEFAULT_POWER_OFF_TIME_USB_DISCONNECTED_SEC = 300;

inline constexpr uint8_t MAX_LED_POWER_LEVEL = 100;
inline constexpr uint8_t DEFAULT_LED_POWER_LEVEL = 50;
inline constexpr uint8_t DEFAULT_LED_POWER_INACTIVE_LEVEL = 50;

inline constexpr uint8_t DEFAULT_BRIGHTNESS = 89;
inline constexpr uint8_t MAX_BRIGHTNESS = 255;

inline constexpr uint8_t DEFAULT_CLEAN_REFRESH_PERIOD = 10;
inline constexpr uint8_t MAX_CLEAN_REFRESH_PERIOD = 255;

inline constexpr uint16_t DEFAULT_BTN_LONG_PRESS_DELAY = 1500;
inline constexpr uint16_t MAX_BTN_LONG_PRESS_DELAY = 10000;

inline constexpr uint16_t DEFAULT_BTN_REPEAT_LONG_PRESS_DELAY = 250;
inline constexpr uint16_t MAX_BTN_REPEAT_LONG_PRESS_DELAY = 10000;

inline constexpr uint16_t DEFAULT_DISPLAY_LOW_POW_DELAY_SEC = 3;
inline constexpr uint16_t MAX_DISPLAY_LOW_POW_DELAY_SEC = 1000;

inline constexpr uint16_t DEFAULT_LOW_BATTERY_BLINK_ON_DELAY_MS = 100;
inline constexpr uint16_t DEFAULT_LOW_BATTERY_BLINK_OFF_DELAY_MS = 900;

inline constexpr uint16_t DEFAULT_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT = 20;
inline constexpr uint16_t MAX_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT = 100;

inline constexpr uint16_t DEFAULT_BLE_BLINK_ON_DELAY_MS = 200;
inline constexpr uint16_t DEFAULT_BLE_BLINK_OFF_DELAY_MS = 1800;

inline constexpr uint8_t DEFAULT_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD = 10;
inline constexpr uint8_t MAX_BLACK_BLACKGROUND_CLEAN_REFRESH_PERIOD = 255;

inline constexpr uint32_t TEST_INACTIVE_TIME_SEC = 50;
inline constexpr uint32_t TEST_SLEEP_TIME_SEC = 65;
inline constexpr uint32_t TEST_INACTIVE_TIME_USB_DISCONNECTED_SEC = 70;
inline constexpr uint32_t TEST_SLEEP_TIME_USB_DISCONNECTED_SEC = 75;
inline constexpr uint32_t TEST_POWER_OFF_TIME_USB_DISCONNECTED_SEC = 200;
inline constexpr uint8_t TEST_LED_POWER_LEVEL = 60;
inline constexpr uint8_t TEST_LED_POWER_INACTIVE_LEVEL = 20;
inline constexpr uint16_t TEST_BTN_LONG_PRESS_DELAY_MS = 1000;
inline constexpr uint16_t TEST_BTN_REPEAT_LONG_PRESS_DELAY_MS = 300;
inline constexpr uint8_t TEST_CLEAN_REFRESH_PERIOD = 7;
inline constexpr bool TEST_DEMO_MODE = true;
inline constexpr uint16_t TEST_LOW_BATTERY_BLINK_ON_DELAY_MS = 500;
inline constexpr uint16_t TEST_LOW_BATTERY_BLINK_OFF_DELAY_MS = 600;
inline constexpr uint16_t TEST_LOW_BATTERY_LEVEL_THRESHOLD_PERCENT = 30;
inline constexpr uint16_t TEST_BLE_BLINK_ON_DELAY_MS = 550;
inline constexpr uint16_t TEST_BLE_BLINK_OFF_DELAY_MS = 650;
inline constexpr bool TEST_HIGH_QUALITY_MODIFIER = false;
inline constexpr uint8_t TEST_BLACK_BACKGROUND_CLEAN_REFRESH_PERIOD = 5;

#define TEST_BRIGHTNESS_LIST {20, 30, 40, 50, 60, 70, 80, 90, 100};


#endif //KEYBOARD_PARAMETERS_TEST_VALUES
