/*
 * IGPIOController.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Labo
 */

#ifndef GPIO_IGPIOCONTROLLER_H_
#define GPIO_IGPIOCONTROLLER_H_

#include <stdint.h>
#include <functional>
#include "GPIOPins.h"

typedef enum GPIOInMode_t
{
    IGPIO_INMODE_IN = 0,
    IGPIO_INMODE_EXTINT
} GPIOInMode;

typedef enum GPIOPull_t
{
    IGPIO_PULL_UP = 0,
    IGPIO_PULL_DOWN,
    IGPIO_NO_PULL
} GPIOPull;

typedef enum GPIOOutType_t
{
    IGPIO_OUTTYPE_PUSHPULL = 0,
    IGPIO_OUTTYPE_OPENDRAIN
} GPIOOutType;

typedef enum GPIOInterruptMode_t
{
    IGPIO_ITMODE_UNDEFINED = 0,
    IGPIO_ITMODE_RISING,
    IGPIO_ITMODE_FALLING,
    IGPIO_ITMODE_RISING_FALLING
} GPIOInterruptMode;

typedef enum GPIOPinState_t
{
    IGPIO_STATE_LOW = 0,
    IGPIO_STATE_HIGH
} GPIOPinState;

typedef std::function<void()> GPIOIntrCallback;

typedef struct GPIOOutConfig_t
{
    GPIOOutType outType;
    GPIOPull pull;
} GPIOOutConfig;

typedef struct GPIOInConfig_t
{
    GPIOInMode mode;
    GPIOPull pull;
    GPIOInterruptMode itMode;
    GPIOIntrCallback itCallback;
} GPIOInConfig;


class IGPIOController
{
public:
    IGPIOController() = default;
    virtual ~IGPIOController() = default;

    virtual void configureOutput(GPIOPin gpioPin, GPIOOutConfig gpioOutConfig) = 0;
    virtual void configureInput(GPIOPin gpioPin, GPIOInConfig gpioInConfig) = 0;

    virtual GPIOPinState read(GPIOPin gpioPin) = 0;
    virtual void set(GPIOPin gpioPin) = 0;
    virtual void clear(GPIOPin gpioPin) = 0;
    virtual void deinit(GPIOPin gpioPin) = 0;
};

#endif /* GPIO_IGPIOCONTROLLER_H_ */
