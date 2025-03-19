/*
 * ITBLECommStrategy.hpp
 *
 *  Created on: Nov 22, 2019
 *      Author: Labo
 */

#ifndef BLE_ITBLECOMMSTRATEGY_HPP_
#define BLE_ITBLECOMMSTRATEGY_HPP_

#include <IITBLECommStrategyListener.hpp>
#include "IGPIOController.hpp"
#include "IBLECommStrategy.hpp"
#include "Signal.hpp"


class ITBLECommStrategy : public IBLECommStrategy
{
public:
    ITBLECommStrategy(IGPIOController& gpioController,
                      GPIOPin& hrdyPin,
                      GPIOPin& dtsPin,
                      IITBLECommStrategyListener& listener);
    virtual ~ITBLECommStrategy() = default;

    void init();
    void deinit();
    void resetHostReadyState();
    bool waitForHostReady(DelayMs delay);
    DelayMs getPollDelay();
    bool isDataToSend();

    void hostReadyCallback();
    void dataToSendCallback();

private:
    Signal mHostReadySignal;
    IGPIOController& mGPIOController;
    GPIOPin& mHRDYPin;
    GPIOPin& mDTSPin;
    IITBLECommStrategyListener& mListener;
};

#endif /* BLE_ITBLECOMMSTRATEGY_HPP_ */
