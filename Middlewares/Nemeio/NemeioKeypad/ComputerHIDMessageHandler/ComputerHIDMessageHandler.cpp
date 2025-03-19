/*
 * ComputerHIDMessageHandler.cpp
 *
 *  Created on: 12 mai 2017
 *      Author: cgarnier
 */

#include <vector>
#include <algorithm>

#include "ComputerHIDMessageHandler.h"
#include "keyIDConfig.h"

void ComputerHIDMessageHandler::registerHID(AbstractHID& abstractHID)
{
    m_abstractHIDs.push_back(&abstractHID);
}

void ComputerHIDMessageHandler::unregisterHID(AbstractHID& abstractHID)
{
    auto ite = std::find(m_abstractHIDs.begin(), m_abstractHIDs.end(), &abstractHID);
    if(m_abstractHIDs.end() != ite)
    {
        m_abstractHIDs.erase(ite);
    }
}

void ComputerHIDMessageHandler::selectTouchForSenders(const ButtonID& button,
                                                      bool bPressed,
                                                      const Configuration& config)
{
    for(auto& hid: m_abstractHIDs)
    {
        uint8_t keyIdx = 0;
        if(button.toIndex(keyIdx) && keyIdx <= ButtonID::KEYBOARD_BUTTON_MAX_INDEX)
        {
            hid->selectTouch(keyIdx,
                             bPressed,
                             config.getOperatingSystem() == configuration::OperatingSystem::MAC);
        }
    }
}

void ComputerHIDMessageHandler::pressedKeyWithConfig(const ButtonID& button,
                                                     const Configuration& config)
{
    selectTouchForSenders(button, true, config);
}

void ComputerHIDMessageHandler::unpressedKeyWithConfig(const ButtonID& button,
                                                       const Configuration& config)
{
    selectTouchForSenders(button, false, config);
}

void ComputerHIDMessageHandler::terminateUpdateKeysWithConfig(const Configuration& config)
{
    for(auto& hid: m_abstractHIDs)
    {
        hid->sendReports();
    }
}

void ComputerHIDMessageHandler::resetKeysWithConfig(const Configuration& config)
{
    for(auto& hid: m_abstractHIDs)
    {
        hid->clearReports();
    }
}

void ComputerHIDMessageHandler::selectKeyCode(HIDKey::Keycode keyCode, bool pressed)
{
    for(auto& hid: m_abstractHIDs)
    {
        hid->selectKeyCode(keyCode, pressed);
    }
}