/*
 * CustomKeyProtocolMessageHandler.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: Labo
 */

#include "CustomKeyProtocolMessageHandler.hpp"

#include <algorithm>

void CustomKeyProtocolMessageHandler::selectTouchForSenders(const ButtonID& button,
                                                            bool bPressed,
                                                            const Configuration& config)
{
    for(auto ite = mCustomKeyProtocols.begin(); ite != mCustomKeyProtocols.end(); ++ite)
    {
        uint8_t keyIdx = 0;
        if(button.toIndex(keyIdx) && keyIdx <= ButtonID::KEYBOARD_BUTTON_MAX_INDEX)
        {
            (*ite)->selectTouch(keyIdx, bPressed);
        }
    }
}

void CustomKeyProtocolMessageHandler::registerCustomKeyProtocol(CustomKeyProtocol& customKeyProtocol)
{
    mCustomKeyProtocols.push_back(&customKeyProtocol);
}

void CustomKeyProtocolMessageHandler::unregisterCustomKeyProtocol(
    CustomKeyProtocol& customKeyProtocol)
{
    auto ite = std::find(mCustomKeyProtocols.begin(), mCustomKeyProtocols.end(), &customKeyProtocol);
    if(mCustomKeyProtocols.end() != ite)
    {
        mCustomKeyProtocols.erase(ite);
    }
}

void CustomKeyProtocolMessageHandler::pressedKeyWithConfig(const ButtonID& button,
                                                           const Configuration& config)
{
    selectTouchForSenders(button, true, config);
}

void CustomKeyProtocolMessageHandler::unpressedKeyWithConfig(const ButtonID& button,
                                                             const Configuration& config)
{
    selectTouchForSenders(button, false, config);
}

void CustomKeyProtocolMessageHandler::terminateUpdateKeysWithConfig(const Configuration& config)
{
    for(auto ite = mCustomKeyProtocols.begin(); ite != mCustomKeyProtocols.end(); ++ite)
    {
        (*ite)->sendKeyReport(config);
    }
}

void CustomKeyProtocolMessageHandler::resetKeysWithConfig(const Configuration& /*config*/)
{
    for(auto ite = mCustomKeyProtocols.begin(); ite != mCustomKeyProtocols.end(); ++ite)
    {
        (*ite)->resetKeys();
    }
}
