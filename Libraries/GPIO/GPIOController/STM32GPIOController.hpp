/*
 * STM32GPIO.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Labo
 */

#ifndef GPIO_STM32GPIO_STM32GPIOCONTROLLER_HPP_
#define GPIO_STM32GPIO_STM32GPIOCONTROLLER_HPP_

#include "IGPIOController.hpp"
#include "pinConfig.h"

#include "usbd_conf.h"

class STM32GPIOController : public IGPIOController
{
public:
    STM32GPIOController() = default;
    virtual ~STM32GPIOController() = default;

    void configureOutput(GPIOPin gpioPin, GPIOOutConfig gpioOutConfig);
    void configureInput(GPIOPin gpioPin, GPIOInConfig gpioInConfig);
    void deInit();

    GPIOPinState read(GPIOPin gpioPin);
    void set(GPIOPin gpioPin);
    void clear(GPIOPin gpioPin);
    void deinit(GPIOPin gpioPin);

    static GPIOIntrCallback& getCallbackFromSTM32Pin(uint16_t stm32Pin);

private:
    static GPIOIntrCallback mIntrCallbacks[STM32_NB_EXTI];

    STM32Pin convertPinToSTM32(GPIOPin gpioPin);
    uint32_t convertInModeToSTM32(GPIOInMode gpioMode, GPIOInterruptMode gpioIntrMode);
    uint32_t convertPullToSTM32(GPIOPull gpioPull);
    uint32_t convertOutModeToSTM32(GPIOOutType gpioOutType);
    GPIOPinState convertPinStateFromSTM2(GPIO_PinState gpioPinState);

    static uint32_t getCallbackIdxFromSTM32Pin(uint16_t stm32Pin);
    uint32_t getCallbackIdxFromPin(GPIOPin gpioPin);
};

#endif /* GPIO_STM32GPIO_STM32GPIOCONTROLLER_HPP_ */
