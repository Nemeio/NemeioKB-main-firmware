/*
 * ConnectivityStrategySwitcher.hpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

#ifndef CONNECTIVITY_CONNECTIVITYSTRATEGYSWITCHER_HPP_
#define CONNECTIVITY_CONNECTIVITYSTRATEGYSWITCHER_HPP_

#include "IConnectivityListener.hpp"
#include <functional>

template<class Strategy>
class ConnectivityStrategySwitcher : public IConnectivityListener
{
public:
    ConnectivityStrategySwitcher(Strategy& usbStrategy,
                                 Strategy& bleStrategy,
                                 Strategy& noneStrategy);
    virtual ~ConnectivityStrategySwitcher() = default;

    void onConnectivityChangeUsb() final;
    void onConnectivityChangeBle() final;
    void onConnectivityChangeNone() final;

protected:
    Strategy& getCurrentStrategy() const;

private:
    Strategy& mUSBStrategy;
    Strategy& mBLEStrategy;
    Strategy& mNoneStrategy;
    std::reference_wrapper<Strategy> mCurrentStrategy;

    void changeStrategy(Strategy& strategy);
};

template<class Strategy>
ConnectivityStrategySwitcher<Strategy>::ConnectivityStrategySwitcher(Strategy& usbStrategy,
                                                                     Strategy& bleStrategy,
                                                                     Strategy& noneStrategy)
    : mUSBStrategy(usbStrategy)
    , mBLEStrategy(bleStrategy)
    , mNoneStrategy(noneStrategy)
    , mCurrentStrategy(mNoneStrategy)
{
}

template<class Strategy>
void ConnectivityStrategySwitcher<Strategy>::onConnectivityChangeUsb()
{
    changeStrategy(mUSBStrategy);
}

template<class Strategy>
void ConnectivityStrategySwitcher<Strategy>::onConnectivityChangeBle()
{
    changeStrategy(mBLEStrategy);
}

template<class Strategy>
void ConnectivityStrategySwitcher<Strategy>::onConnectivityChangeNone()
{
    changeStrategy(mNoneStrategy);
}

template<class Strategy>
Strategy& ConnectivityStrategySwitcher<Strategy>::getCurrentStrategy() const
{
    return mCurrentStrategy.get();
}

template<class Strategy>
void ConnectivityStrategySwitcher<Strategy>::changeStrategy(Strategy& strategy)
{
    mCurrentStrategy.get().deinit();
    mCurrentStrategy = strategy;
    mCurrentStrategy.get().init();
}

#endif /* CONNECTIVITY_CONNECTIVITYSTRATEGYSWITCHER_HPP_ */
