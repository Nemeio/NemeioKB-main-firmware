/*
 * KeyProtocolManager.cpp
 *
 *  Created on: Sep 19, 2018
 *      Author: Labo
 */

#include "KeyProtocolManager.hpp"
#include "AutoLock.h"
#include "assertTools.h"

KeyProtocolManager::KeyProtocolManager(AbstractRecursiveMutex& mutex)
    : mMutex(mutex)
{
}

void KeyProtocolManager::doSetCustomMode()
{
    keyprotocoldefs::KeyProtocolMode newMode = keyprotocoldefs::KeyProtocolMode::CUSTOM;
    doSetNewMode(newMode);
}

void KeyProtocolManager::doSetHIDMode()
{
    keyprotocoldefs::KeyProtocolMode newMode = keyprotocoldefs::KeyProtocolMode::HID;
    doSetNewMode(newMode);
}

void KeyProtocolManager::doSetPairingMode()
{
    keyprotocoldefs::KeyProtocolMode newMode = keyprotocoldefs::KeyProtocolMode::PAIRING;
    doSetNewMode(newMode);
}

void KeyProtocolManager::doSetNewMode(keyprotocoldefs::KeyProtocolMode mode)
{
    notifyKeyProtocolListeners(mode);
}

void KeyProtocolManager::registerKeyProtocolListener(IKeyProtocolListener& keyProtocolListener)
{
    AutoLock autoLock(mMutex);

    mKeyProtocolListeners.push_back(&keyProtocolListener);
}

void KeyProtocolManager::init()
{
    AutoLock autoLock(mMutex);

    if(KeyProtocolState::INIT == mProtocolState)
    {
        // Enter HID not connected state
        mProtocolState = KeyProtocolState::NOCONN_HID;
        doSetHIDMode();
    }
}

void KeyProtocolManager::keepAlive()
{
    AutoLock autoLock(mMutex);

    switch(mProtocolState)
    {
    case KeyProtocolState::NOCONN_HID:
        // Enter connected custom state
        mProtocolState = KeyProtocolState::CONN_CUSTOM;
        doSetCustomMode();
        break;
    case KeyProtocolState::INIT:
    case KeyProtocolState::CONN_HID:
    case KeyProtocolState::CONN_CUSTOM:
    case KeyProtocolState::PAIRING:
    default:
        // Do nothing
        break;
    }
}

void KeyProtocolManager::keepAliveTimeout()
{
    AutoLock autoLock(mMutex);

    switch(mProtocolState)
    {
    case KeyProtocolState::CONN_CUSTOM:
        // Enter HID not connected state
        mProtocolState = KeyProtocolState::NOCONN_HID;
        doSetHIDMode();
        break;
    case KeyProtocolState::CONN_HID:
        // Enter HID not connected state
        mProtocolState = KeyProtocolState::NOCONN_HID;
        break;
    case KeyProtocolState::INIT:
    case KeyProtocolState::NOCONN_HID:
    case KeyProtocolState::PAIRING:
    default:
        // Do nothing
        break;
    }
}

bool KeyProtocolManager::setProtocolMode(keyprotocoldefs::KeyProtocolMode protocolMode)
{
    AutoLock autoLock(mMutex);

    if(protocolMode != keyprotocoldefs::KeyProtocolMode::CUSTOM
       && protocolMode != keyprotocoldefs::KeyProtocolMode::HID)
    {
        return false;
    }

    switch(mProtocolState)
    {
    case KeyProtocolState::CONN_CUSTOM:
        if(keyprotocoldefs::KeyProtocolMode::HID == protocolMode)
        {
            // Enter HID connected state
            mProtocolState = KeyProtocolState::CONN_HID;
            doSetHIDMode();
        }
        break;
    case KeyProtocolState::CONN_HID:
        if(keyprotocoldefs::KeyProtocolMode::CUSTOM == protocolMode)
        {
            // Enter custom connected state
            mProtocolState = KeyProtocolState::CONN_CUSTOM;
            doSetCustomMode();
        }
        break;
    case KeyProtocolState::INIT:
    case KeyProtocolState::NOCONN_HID:
    case KeyProtocolState::PAIRING:
    default:
        // Do nothing
        break;
    }

    return true;
}

void KeyProtocolManager::notifyKeyProtocolListeners(keyprotocoldefs::KeyProtocolMode mode)
{
    for(auto& pListener: mKeyProtocolListeners)
    {
        if(pListener != nullptr)
        {
            pListener->keyProtocolModeChanged(mode);
        }
    }
}

void KeyProtocolManager::pairingStart(const BLEDefs::Passkey& passkey)
{
    AutoLock autoLock(mMutex);

    switch(mProtocolState)
    {
    case KeyProtocolState::CONN_CUSTOM:
    case KeyProtocolState::CONN_HID:
    case KeyProtocolState::INIT:
    case KeyProtocolState::NOCONN_HID:
        mProtocolState = KeyProtocolState::PAIRING;
        doSetPairingMode();
        break;
    case KeyProtocolState::PAIRING:
    default:
        // Do nothing
        break;
    }
}

void KeyProtocolManager::pairingStop()
{
    AutoLock autoLock(mMutex);

    switch(mProtocolState)
    {
    case KeyProtocolState::PAIRING:
        mProtocolState = KeyProtocolState::NOCONN_HID;
        doSetHIDMode();
        break;
    case KeyProtocolState::CONN_CUSTOM:
    case KeyProtocolState::CONN_HID:
    case KeyProtocolState::INIT:
    case KeyProtocolState::NOCONN_HID:
    default:
        // Do nothing
        break;
    }
}
