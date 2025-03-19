/*
 * pinConfig.h
 *
 *  Created on: Mar 16, 2017
 *      Author: Labo
 */

#ifndef PINCONFIG_H_
#define PINCONFIG_H_

//#include "common.h"
#include "main.h"
#include "stm32l4xx_hal.h"

// Number of external interrupts available for STM32
#define STM32L4_NB_EXTI 16
#define STM32_NB_EXTI STM32L4_NB_EXTI

// Pin definition
#define HOLITECH_SCREEN_BUSYN_PIN PIN_0
#define HOLITECH_SCREEN_VCI_PIN PIN_UNDEFINED
#define HOLITECH_SCREEN_CSN_PIN PIN_2
#define HOLITECH_SCREEN_CSN1_PIN PIN_3
#define HOLITECH_SCREEN_RESN_PIN PIN_4
#define HOLITECH_SCREEN_BS1_PIN PIN_UNDEFINED
#define HOLITECH_SCREEN_DCN_PIN PIN_6
#define HOLITECH_SCREEN_EPD_ON_PIN PIN_8

#define PD2 Pd2

typedef struct STM32Pin_t
{
    uint16_t pin;
    GPIO_TypeDef* port;
} STM32Pin;

typedef struct STM32PinAssoc_t
{
    GPIOPin gpioPin;
    STM32Pin stm32Pin;
} STM32PinAssoc;

#define STM32_GPIO_PIN_UNDEFINED 0
#define STM32_PIN_UNDEFINED \
    { \
        STM32_GPIO_PIN_UNDEFINED, 0 \
    }

const STM32PinAssoc gSTM32PinAssoc[] = {
    {HOLITECH_SCREEN_BUSYN_PIN, {EPD_BUSYN_Pin, EPD_BUSYN_GPIO_Port}},
    {HOLITECH_SCREEN_VCI_PIN, STM32_PIN_UNDEFINED},
    {HOLITECH_SCREEN_CSN_PIN, {EPD_CSN_Pin, EPD_CSN_GPIO_Port}},
    {HOLITECH_SCREEN_RESN_PIN, {EPD_CTRL_RST__Pin, EPD_CTRL_RST__GPIO_Port}},
    {HOLITECH_SCREEN_EPD_ON_PIN, {EPD_ON_Pin, EPD_ON_GPIO_Port}},
    {BLE_csn_Pin, {BLE_NSS_Pin, BLE_NSS_GPIO_Port}},
    {Cpu_Wake_Up, {CPU_WAKE_UP_Pin, CPU_WAKE_UP_GPIO_Port}},
    {Vbus_Detect, {VBUS_DET_Pin, VBUS_DET_GPIO_Port}},
    {WKup_3, {Button_ON_OFF_Pin, Button_ON_OFF_GPIO_Port}},
    {Key75_Alt, {KB_KEY75_ALT_Pin, KB_KEY75_ALT_GPIO_Port}},
    {Key73_Fn, {KB_KEY73_FN_Pin, KB_KEY73_FN_GPIO_Port}},
    {Key74_Win, {KB_KEY74_WIN_Pin, KB_KEY74_WIN_GPIO_Port}},
    {Key57_Enter, {KB_KEY57_ENTER_Pin, KB_KEY57_ENTER_GPIO_Port}},
    {Key58_Shift_Left, {KB_KEY58_SHIFT_LEFT_Pin, KB_KEY58_SHIFT_LEFT_GPIO_Port}},
    {Key71_Shift_Right, {KB_KEY71_SHIFT_RIGHT_Pin, KB_KEY71_SHIFT_RIGHT_GPIO_Port}},
    {Key_Ctrl_Left, {KB_KEY72_CTRL_LEFT_Pin, KB_KEY72_CTRL_LEFT_GPIO_Port}},
    {Key_Ctrl_Right, {KB_KEY78_CTRL_RIGHT_Pin, KB_KEY78_CTRL_RIGHT_GPIO_Port}},
    {Key_Alt_Gr, {KB_KEY77_ALT_GR_Pin, KB_KEY77_ALT_GR_GPIO_Port}},
    {Epd_Fl_On, {EPD_FL_ON_Pin, EPD_FL_ON_GPIO_Port}},
    {Btn_Left, {Button_LEFT_Pin, Button_LEFT_GPIO_Port}},
    {Btn_Right, {Button_RIGHT_Pin, Button_RIGHT_GPIO_Port}},
    {Ble_Hrdy, {BLE_HRDY_Pin, BLE_HRDY_GPIO_Port}},
    {Ble_Dts, {BLE_DTS_Pin, BLE_DTS_GPIO_Port}},
    {Ble_Miso, {BLE_MISO_Pin, BLE_MISO_GPIO_Port}},
    {Ble_Mosi, {BLE_MOSI_Pin, BLE_MOSI_GPIO_Port}},
    {Led_CapsLock, {LED_CAPSLOCK_Pin, LED_CAPSLOCK_GPIO_Port}},
    {Led_Battery, {PWR_STAT_Pin, PWR_STAT_GPIO_Port}},
    {Led_Ble, {LED_BT_Pin, LED_BT_GPIO_Port}},
    /* Keyboard matrix */
    {Matrix_Row_One, {GPIO_PIN_0, GPIOE}},
    {Matrix_Row_Two, {GPIO_PIN_1, GPIOE}},
    {Matrix_Row_Three, {GPIO_PIN_2, GPIOE}},
    {Matrix_Row_Four, {GPIO_PIN_3, GPIOE}},
    {Matrix_Row_Five, {GPIO_PIN_4, GPIOE}},
    {Matrix_Row_Six, {GPIO_PIN_5, GPIOE}},
    {Matrix_Column_One, {GPIO_PIN_0, GPIOD}},
    {Matrix_Column_Two, {GPIO_PIN_1, GPIOD}},
    {Matrix_Column_Three, {GPIO_PIN_2, GPIOD}},
    {Matrix_Column_Four, {GPIO_PIN_3, GPIOD}},
    {Matrix_Column_Five, {GPIO_PIN_4, GPIOD}},
    {Matrix_Column_Six, {GPIO_PIN_5, GPIOD}},
    {Matrix_Column_Seven, {GPIO_PIN_6, GPIOD}},
    {Matrix_Column_Eight, {GPIO_PIN_7, GPIOD}},
    {Matrix_Column_Nine, {GPIO_PIN_8, GPIOD}},
    {Matrix_Column_Ten, {GPIO_PIN_9, GPIOD}},
    {Matrix_Column_Eleven, {GPIO_PIN_10, GPIOD}},
    {Matrix_Column_Twelve, {GPIO_PIN_11, GPIOD}},
    {Matrix_Column_Thirteen, {GPIO_PIN_12, GPIOD}},
    {Matrix_Column_Fourteen, {GPIO_PIN_13, GPIOD}},
    {Matrix_Column_Fifteen, {GPIO_PIN_14, GPIOD}},
    {Matrix_Column_Sixteen, {GPIO_PIN_15, GPIOD}},
    {Electrical_Test_Enter, {GPIO_PIN_9, GPIOB}},
    {Electrical_Test_Passed, {GPIO_PIN_14, GPIOC}},
};


#endif /* PINCONFIG_H_ */
