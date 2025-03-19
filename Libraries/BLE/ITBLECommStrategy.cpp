/*
 * ITBLECommStrategy.cpp
 *
 *  Created on: Nov 22, 2019
 *      Author: Labo
 */

#include "ITBLECommStrategy.hpp"
#include "FreeRTOSUtils.hpp"

ITBLECommStrategy::ITBLECommStrategy(IGPIOController& gpioController,
                                     GPIOPin& hrdyPin,
                                     GPIOPin& dtsPin,
                                     IITBLECommStrategyListener& listener)
    : mGPIOController(gpioController)
    , mHRDYPin(hrdyPin)
    , mDTSPin(dtsPin)
    , mListener(listener)
{
}

void ITBLECommStrategy::init()
{
    GPIOInConfig gpioInConfig;

    std::function<void()> hostReadyItCallback = std::bind(&ITBLECommStrategy::hostReadyCallback,
                                                          this);

    gpioInConfig.mode = IGPIO_INMODE_EXTINT;
    gpioInConfig.pull = IGPIO_PULL_DOWN;
    gpioInConfig.itMode = IGPIO_ITMODE_RISING;
    gpioInConfig.itCallback = hostReadyItCallback;

    mGPIOController.configureInput(mHRDYPin, gpioInConfig);

    std::function<void()> dataToSendItCallback = std::bind(&ITBLECommStrategy::dataToSendCallback,
                                                           this);

    gpioInConfig.mode = IGPIO_INMODE_EXTINT;
    gpioInConfig.pull = IGPIO_PULL_DOWN;
    gpioInConfig.itMode = IGPIO_ITMODE_RISING;
    gpioInConfig.itCallback = dataToSendItCallback;

    mGPIOController.configureInput(mDTSPin, gpioInConfig);
}

void ITBLECommStrategy::deinit()
{
    mGPIOController.deinit(mHRDYPin);
    mGPIOController.deinit(mDTSPin);
}

void ITBLECommStrategy::resetHostReadyState()
{
    mHostReadySignal.clear();
}

bool ITBLECommStrategy::waitForHostReady(DelayMs delay)
{
    return mHostReadySignal.wait(delay);
}

DelayMs ITBLECommStrategy::getPollDelay()
{
    return DelayMs(DelayMs::INFINITE_DELAY);
}

bool ITBLECommStrategy::isDataToSend()
{
    GPIOPinState state = mGPIOController.read(mDTSPin);
    return IGPIO_STATE_HIGH == state;
}


void ITBLECommStrategy::hostReadyCallback()
{
    mHostReadySignal.setFromISR();
}

void ITBLECommStrategy::dataToSendCallback()
{
    mListener.bleDataToSendAvailableIsr();
}
