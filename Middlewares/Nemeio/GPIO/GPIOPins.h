/*
 * GPIOPins.h
 *
 *  Created on: Mar 16, 2017
 *      Author: Labo
 */

#ifndef NEMEIO_GPIO_INC_GPIOPINS_H_
#define NEMEIO_GPIO_INC_GPIOPINS_H_

typedef enum GPIOPin_t
{
    PIN_UNDEFINED = 0,
    PIN_0,
    PIN_1,
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7,
    PIN_8,
    BLE_csn_Pin,
    Pd2,
    Cpu_Wake_Up,
    Vbus_Detect,
    WKup_3,
    Key75_Alt,
    Key73_Fn,
    Key74_Win,
    Key57_Enter,
    Key58_Shift_Left,
    Key71_Shift_Right,
    Key_Ctrl_Left,
    Key_Ctrl_Right,
    Key_Alt_Gr,
    Epd_Fl_On,
    Btn_Left,
    Btn_Right,
    Ble_Hrdy,
    Ble_Dts,
    Ble_Miso,
    Ble_Mosi,
    Led_CapsLock,
    Led_Battery,
    Led_Ble,
    Matrix_Row_One,
    Matrix_Row_Two,
    Matrix_Row_Three,
    Matrix_Row_Four,
    Matrix_Row_Five,
    Matrix_Row_Six,
    Matrix_Column_One,
    Matrix_Column_Two,
    Matrix_Column_Three,
    Matrix_Column_Four,
    Matrix_Column_Five,
    Matrix_Column_Six,
    Matrix_Column_Seven,
    Matrix_Column_Eight,
    Matrix_Column_Nine,
    Matrix_Column_Ten,
    Matrix_Column_Eleven,
    Matrix_Column_Twelve,
    Matrix_Column_Thirteen,
    Matrix_Column_Fourteen,
    Matrix_Column_Fifteen,
    Matrix_Column_Sixteen,
    Electrical_Test_Enter,
    Electrical_Test_Passed,

} GPIOPin;

#endif /* NEMEIO_GPIO_INC_GPIOPINS_H_ */
