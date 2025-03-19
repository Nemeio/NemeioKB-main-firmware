/*
 * AbstractConnectivityState.hpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

#ifndef CONNECTIVITY_ABSTRACTCONNECTIVITYSTATE_HPP_
#define CONNECTIVITY_ABSTRACTCONNECTIVITYSTATE_HPP_

#include "IUsbActivable.h"
#include "IPowerManageable.h"
#include <algorithm>
#include "assertTools.h"
#include "IConnectivityListener.hpp"
#include <functional>

class AbstractConnectivityState : public IUsbActivable, public IPowerManageable
{
public:
    enum class ConnectivityStateValue
    {
        NONE,
        USB,
        BLE
    };

    AbstractConnectivityState();
    virtual ~AbstractConnectivityState() = default;

    void usbPluggedInComputer();
    void usbPluggedInComputerSuspended();
    void usbPluggedInPowerSupplyOnly();
    void usbPluggedOut();

    void active();
    void inactive();
    void longInactive();
    void sleep();
    void powerOff();
    void suspend();

private:
    ConnectivityStateValue mState;

    void setState(ConnectivityStateValue state);
    void notifyState(ConnectivityStateValue state);
    void notifyConnectivityChangeNone();
    void notifyConnectivityChangeBle();
    void notifyConnectivityChangeUsb();
    virtual void notifyListeners(std::function<void(IConnectivityListener&)> notify) = 0;
};

#endif /* CONNECTIVITY_ABSTRACTCONNECTIVITYSTATE_HPP_ */
