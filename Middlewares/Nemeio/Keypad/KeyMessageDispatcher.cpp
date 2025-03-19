/*
 * KeyMessageDispatcher.cpp
 *
 *  Created on: Dec 16, 2020
 *      Author: eskoric
 */

#include "KeyMessageDispatcher.hpp"
#include "AutoLock.h"
#include "assertTools.h"

KeyMessageDispatcher::KeyMessageDispatcher(AbstractRecursiveMutex& mutex)
    : mMutex(mutex)
{
}

void KeyMessageDispatcher::registerHIDKeyMessageHandler(IKeyMessageHandler& messageHandler)
{
    AutoLock autoLock(mMutex);

    mHIDNotifyKeyHandlerStrategy.registerKeyHandler(messageHandler);
}

void KeyMessageDispatcher::registerCustomProtocolKeyMessageHandler(IKeyMessageHandler& messageHandler)
{
    AutoLock autoLock(mMutex);

    mCustomProtocolNotifyKeyHandlerStrategy.registerKeyHandler(messageHandler);
}

void KeyMessageDispatcher::registerPairingKeyMessageHandler(IKeyMessageHandler& messageHandler)
{
    AutoLock autoLock(mMutex);

    mPairingNotifyKeyHandlerStrategy.registerKeyHandler(messageHandler);
}

void KeyMessageDispatcher::pressedKey(const ButtonID& button)
{
    AutoLock autoLock(mMutex);

    doNotifyKeyMessageHandlers(
        [button](IKeyMessageHandler& keyMessageHandler, const Configuration& config)
        { keyMessageHandler.pressedKeyWithConfig(button, config); });
}

void KeyMessageDispatcher::unpressedKey(const ButtonID& button)
{
    AutoLock autoLock(mMutex);

    doNotifyKeyMessageHandlers(
        [button](IKeyMessageHandler& keyMessageHandler, const Configuration& config)
        { keyMessageHandler.unpressedKeyWithConfig(button, config); });
}

void KeyMessageDispatcher::terminateUpdateKeys()
{
    AutoLock autoLock(mMutex);

    doNotifyKeyMessageHandlers([](IKeyMessageHandler& keyMessageHandler, const Configuration& config)
                               { keyMessageHandler.terminateUpdateKeysWithConfig(config); });
}

void KeyMessageDispatcher::configurationChanged(const Configuration* pConfig)
{
    AutoLock autoLock(mMutex);
    ASSERT(pConfig != nullptr);

    resetKeyMessageHandlers();

    mConfig.emplace(*pConfig);

    keypadActionsChanged();
}

void KeyMessageDispatcher::keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode keyProtocolMode)
{
    AutoLock autoLock(mMutex);

    resetKeyMessageHandlers();

    mKeyProtocolMode = keyProtocolMode;

    keypadActionsChanged();
}

void KeyMessageDispatcher::keypadActionsChanged()
{
    updateStrategy(mKeyProtocolMode);
}

void KeyMessageDispatcher::resetKeyMessageHandlers()
{
    doNotifyKeyMessageHandlers(
        [](IKeyMessageHandler& keyMessageHandler, const Configuration& config)
        {
            keyMessageHandler.resetKeysWithConfig(config);
            keyMessageHandler.terminateUpdateKeysWithConfig(config);
        });
}

template<typename NotifyFunctor>
void KeyMessageDispatcher::doNotifyKeyMessageHandlers(NotifyFunctor notify) const
{
    if(!mConfig.has_value())
    {
        return;
    }

    ASSERT(nullptr != mpNotifyKeyHandlerStrategy);
    mpNotifyKeyHandlerStrategy->doNotify(notify, mConfig.value());
}

void KeyMessageDispatcher::updateStrategy(keyprotocoldefs::KeyProtocolMode mode)
{
    if(!mConfig.has_value())
    {
        mpNotifyKeyHandlerStrategy = &mNoneNotifyKeyHandlerStrategy;
        return;
    }

    if((keyprotocoldefs::KeyProtocolMode::CUSTOM == mode && mConfig.value().isHid())
       || (keyprotocoldefs::KeyProtocolMode::HID == mode))
    {
        mpNotifyKeyHandlerStrategy = &mHIDNotifyKeyHandlerStrategy;
    }
    else if(keyprotocoldefs::KeyProtocolMode::CUSTOM == mode)
    {
        mpNotifyKeyHandlerStrategy = &mCustomProtocolNotifyKeyHandlerStrategy;
    }
    else if(keyprotocoldefs::KeyProtocolMode::PAIRING == mode)
    {
        mpNotifyKeyHandlerStrategy = &mPairingNotifyKeyHandlerStrategy;
    }
    else
    {
        mpNotifyKeyHandlerStrategy = &mNoneNotifyKeyHandlerStrategy;
    }
}
