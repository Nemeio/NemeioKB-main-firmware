/*
 * AbstractConnectivityState.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Labo
 */

#include "AbstractConnectivityState.hpp"
#include "TraceLogger.hpp"

#define MODULE "connectivity"

AbstractConnectivityState::AbstractConnectivityState()
    : mState(ConnectivityStateValue::NONE)
{
}

void AbstractConnectivityState::notifyState(ConnectivityStateValue state)
{
    switch(mState)
    {
    case ConnectivityStateValue::NONE:
        notifyConnectivityChangeNone();
        break;
    case ConnectivityStateValue::BLE:
        notifyConnectivityChangeBle();
        break;
    case ConnectivityStateValue::USB:
        notifyConnectivityChangeUsb();
        break;
    default:
        ASSERT(false);
        break;
    }
}

void AbstractConnectivityState::usbPluggedInComputer()
{
    switch(mState)
    {
    case ConnectivityStateValue::NONE:
    case ConnectivityStateValue::BLE:
        setState(ConnectivityStateValue::USB);
        break;
    case ConnectivityStateValue::USB:
        break;
    default:
        ASSERT(false);
        break;
    }
}

void AbstractConnectivityState::usbPluggedInComputerSuspended()
{
    switch(mState)
    {
    case ConnectivityStateValue::NONE:
    case ConnectivityStateValue::USB:
        setState(ConnectivityStateValue::BLE);
        break;
    case ConnectivityStateValue::BLE:
        break;
    default:
        ASSERT(false);
        break;
    }
}

void AbstractConnectivityState::usbPluggedInPowerSupplyOnly()
{
    switch(mState)
    {
    case ConnectivityStateValue::NONE:
    case ConnectivityStateValue::USB:
        setState(ConnectivityStateValue::BLE);
        break;
    case ConnectivityStateValue::BLE:
        break;
    default:
        ASSERT(false);
        break;
    }
}

void AbstractConnectivityState::usbPluggedOut()
{
    switch(mState)
    {
    case ConnectivityStateValue::NONE:
    case ConnectivityStateValue::USB:
        setState(ConnectivityStateValue::BLE);
        break;
    case ConnectivityStateValue::BLE:
        break;
    default:
        ASSERT(false);
    }
}

void AbstractConnectivityState::setState(ConnectivityStateValue state)
{
    if(mState != state)
    {
        mState = state;
        notifyState(mState);
    }
}

void AbstractConnectivityState::active()
{
    switch(mState)
    {
    case ConnectivityStateValue::NONE:
        setState(ConnectivityStateValue::BLE);
        break;
    case ConnectivityStateValue::USB:
    case ConnectivityStateValue::BLE:
        break;
    default:
        ASSERT(false);
        break;
    }
}

void AbstractConnectivityState::inactive()
{
}

void AbstractConnectivityState::longInactive()
{
}

void AbstractConnectivityState::sleep()
{
    switch(mState)
    {
    case ConnectivityStateValue::BLE:
        setState(ConnectivityStateValue::NONE);
        break;
    case ConnectivityStateValue::USB:
    case ConnectivityStateValue::NONE:
        break;
    default:
        ASSERT(false);
        break;
    }
}

void AbstractConnectivityState::powerOff()
{
    switch(mState)
    {
    case ConnectivityStateValue::NONE:
    case ConnectivityStateValue::USB:
    case ConnectivityStateValue::BLE:
        break;
    default:
        ASSERT(false);
        break;
    }
}

void AbstractConnectivityState::suspend()
{
    switch(mState)
    {
    case ConnectivityStateValue::NONE:
    case ConnectivityStateValue::USB:
    case ConnectivityStateValue::BLE:
        break;
    default:
        ASSERT(false);
        break;
    }
}

void AbstractConnectivityState::notifyConnectivityChangeNone()
{
    LOG_INFO(MODULE, "Set None");
    notifyListeners([](IConnectivityListener& listener) { listener.onConnectivityChangeNone(); });
}

void AbstractConnectivityState::notifyConnectivityChangeBle()
{
    LOG_INFO(MODULE, "Set BLE");
    notifyListeners([](IConnectivityListener& listener) { listener.onConnectivityChangeBle(); });
}

void AbstractConnectivityState::notifyConnectivityChangeUsb()
{
    LOG_INFO(MODULE, "Set USB");
    notifyListeners([](IConnectivityListener& listener) { listener.onConnectivityChangeUsb(); });
}
