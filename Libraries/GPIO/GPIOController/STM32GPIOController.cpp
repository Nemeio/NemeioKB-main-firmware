/*
 * STM32GPIO.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: Labo
 */

#include "STM32GPIOController.hpp"
#include "stm32l4xx_hal.h"


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t stm32Pin)
{
    // TODO: use GPIO class to handle USB module GPIO interrupts !
    // TODO: Check against HAL_GPIO_EXTI_Callback_USB
    HAL_GPIO_EXTI_Callback_USB(stm32Pin);

    // Use a reference to the callback and not a copy. Otherwise cloning the std::function
    // will perform dynamic memory allocation, which is not allowed in ISR context
    GPIOIntrCallback& intrCallback = STM32GPIOController::getCallbackFromSTM32Pin(stm32Pin);

    if(NULL != intrCallback)
    {
        intrCallback();
    }
}

GPIOIntrCallback STM32GPIOController::mIntrCallbacks[STM32_NB_EXTI] = {0};

GPIOIntrCallback& STM32GPIOController::getCallbackFromSTM32Pin(uint16_t stm32Pin)
{
    return mIntrCallbacks[getCallbackIdxFromSTM32Pin(stm32Pin)];
}

void STM32GPIOController::configureOutput(GPIOPin gpioPin, GPIOOutConfig gpioOutConfig)
{
    GPIO_InitTypeDef GPIOInitStruct;
    STM32Pin stm32Pin = convertPinToSTM32(gpioPin);

    if(STM32_GPIO_PIN_UNDEFINED != stm32Pin.pin)
    {
        GPIOInitStruct.Pin = stm32Pin.pin;
        GPIOInitStruct.Mode = convertOutModeToSTM32(gpioOutConfig.outType);
        GPIOInitStruct.Pull = convertPullToSTM32(gpioOutConfig.pull);
        GPIOInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        HAL_GPIO_Init(stm32Pin.port, &GPIOInitStruct);
    }
}

void STM32GPIOController::configureInput(GPIOPin gpioPin, GPIOInConfig gpioInConfig)
{
    GPIO_InitTypeDef GPIOInitStruct = {0};
    STM32Pin stm32Pin = convertPinToSTM32(gpioPin);

    if(STM32_GPIO_PIN_UNDEFINED != stm32Pin.pin)
    {
        GPIOInitStruct.Pin = stm32Pin.pin;
        GPIOInitStruct.Mode = convertInModeToSTM32(gpioInConfig.mode, gpioInConfig.itMode);
        GPIOInitStruct.Pull = convertPullToSTM32(gpioInConfig.pull);
        HAL_GPIO_Init(stm32Pin.port, &GPIOInitStruct);

        if(IGPIO_INMODE_EXTINT == gpioInConfig.mode)
        {
            mIntrCallbacks[getCallbackIdxFromPin(gpioPin)] = gpioInConfig.itCallback;
        }
    }
}

void STM32GPIOController::deInit()
{
}

GPIOPinState STM32GPIOController::read(GPIOPin gpioPin)
{
    STM32Pin stm32Pin = convertPinToSTM32(gpioPin);

    if(STM32_GPIO_PIN_UNDEFINED == stm32Pin.pin)
    {
        return IGPIO_STATE_LOW;
    }
    GPIO_PinState stm32PinState = HAL_GPIO_ReadPin(stm32Pin.port, stm32Pin.pin);

    return convertPinStateFromSTM2(stm32PinState);
}

void STM32GPIOController::set(GPIOPin gpioPin)
{
    STM32Pin stm32Pin = convertPinToSTM32(gpioPin);

    if(STM32_GPIO_PIN_UNDEFINED == stm32Pin.pin)
    {
        return;
    }

    HAL_GPIO_WritePin(stm32Pin.port, stm32Pin.pin, GPIO_PIN_SET);
}

void STM32GPIOController::clear(GPIOPin gpioPin)
{
    STM32Pin stm32Pin = convertPinToSTM32(gpioPin);

    if(STM32_GPIO_PIN_UNDEFINED == stm32Pin.pin)
    {
        return;
    }

    HAL_GPIO_WritePin(stm32Pin.port, stm32Pin.pin, GPIO_PIN_RESET);
}


void STM32GPIOController::deinit(GPIOPin gpioPin)
{
    STM32Pin stm32Pin = convertPinToSTM32(gpioPin);

    if(STM32_GPIO_PIN_UNDEFINED != stm32Pin.pin)
    {
        HAL_GPIO_DeInit(stm32Pin.port, stm32Pin.pin);
    }
}

STM32Pin STM32GPIOController::convertPinToSTM32(GPIOPin gpioPin)
{
    STM32Pin stm32Pin = STM32_PIN_UNDEFINED;

    if(PIN_UNDEFINED != gpioPin)
    {
        for(uint8_t i = 0; i < sizeof(gSTM32PinAssoc) / sizeof(gSTM32PinAssoc[0]); i++)
        {
            if(gpioPin == gSTM32PinAssoc[i].gpioPin)
            {
                stm32Pin = gSTM32PinAssoc[i].stm32Pin;
                break;
            }
        }
    }

    return stm32Pin;
}

uint32_t STM32GPIOController::getCallbackIdxFromSTM32Pin(uint16_t stm32Pin)
{
    switch(stm32Pin)
    {
    case GPIO_PIN_0:
        return 0;
    case GPIO_PIN_1:
        return 1;
    case GPIO_PIN_2:
        return 2;
    case GPIO_PIN_3:
        return 3;
    case GPIO_PIN_4:
        return 4;
    case GPIO_PIN_5:
        return 5;
    case GPIO_PIN_6:
        return 6;
    case GPIO_PIN_7:
        return 7;
    case GPIO_PIN_8:
        return 8;
    case GPIO_PIN_9:
        return 9;
    case GPIO_PIN_10:
        return 10;
    case GPIO_PIN_11:
        return 11;
    case GPIO_PIN_12:
        return 12;
    case GPIO_PIN_13:
        return 13;
    case GPIO_PIN_14:
        return 14;
    case GPIO_PIN_15:
        return 15;
    default:
        return 0;
    }
}

uint32_t STM32GPIOController::convertInModeToSTM32(GPIOInMode gpioInMode,
                                                   GPIOInterruptMode gpioIntrMode)
{
    switch(gpioInMode)
    {
    case IGPIO_INMODE_EXTINT:
        break;
    case IGPIO_INMODE_IN:
    default:
        return GPIO_MODE_INPUT;
    }

    switch(gpioIntrMode)
    {
    case IGPIO_ITMODE_RISING:
        return GPIO_MODE_IT_RISING;
    case IGPIO_ITMODE_RISING_FALLING:
        return GPIO_MODE_IT_RISING_FALLING;
    case IGPIO_ITMODE_FALLING:
    default:
        return GPIO_MODE_IT_FALLING;
    }
}

uint32_t STM32GPIOController::convertPullToSTM32(GPIOPull gpioPull)
{
    switch(gpioPull)
    {
    case IGPIO_PULL_DOWN:
        return GPIO_PULLDOWN;
    case IGPIO_PULL_UP:
        return GPIO_PULLUP;
    case IGPIO_NO_PULL:
    default:
        return GPIO_NOPULL;
    }
}

uint32_t STM32GPIOController::convertOutModeToSTM32(GPIOOutType gpioOutType)
{
    switch(gpioOutType)
    {
    case IGPIO_OUTTYPE_OPENDRAIN:
        return GPIO_MODE_OUTPUT_OD;
    case IGPIO_OUTTYPE_PUSHPULL:
    default:
        return GPIO_MODE_OUTPUT_PP;
    }
}

GPIOPinState STM32GPIOController::convertPinStateFromSTM2(GPIO_PinState gpioPinState)
{
    switch(gpioPinState)
    {
    case GPIO_PIN_RESET:
        return IGPIO_STATE_LOW;
    case GPIO_PIN_SET:
    default:
        return IGPIO_STATE_HIGH;
    }
}

uint32_t STM32GPIOController::getCallbackIdxFromPin(GPIOPin gpioPin)
{
    STM32Pin stm32Pin = convertPinToSTM32(gpioPin);

    if(STM32_GPIO_PIN_UNDEFINED != stm32Pin.pin)
    {
        return getCallbackIdxFromSTM32Pin(stm32Pin.pin);
    }
    return 0;
}
