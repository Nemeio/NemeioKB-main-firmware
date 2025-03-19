/*
 * ComputerHIDMessageHandler.h
 *
 *  Created on: 12 mai 2017
 *      Author: cgarnier
 */

#ifndef __COMPUTERHIDMESSAGEHANDLER_H_
#define __COMPUTERHIDMESSAGEHANDLER_H_

#include "IKeyMessageHandler.hpp"
#include "AbstractHID.h"
#include "HIDKey.hpp"
#include "etl/array.h"
#include "ISystem.hpp"

class ComputerHIDMessageHandler : public IKeyMessageHandler
{
public:
    ComputerHIDMessageHandler() = default;
    virtual ~ComputerHIDMessageHandler() = default;

    void registerHID(AbstractHID& abstractHID);
    void unregisterHID(AbstractHID& abstractHID);

    void sendHIDSequence(const etl::array_view<HIDKey::Keycode>& keycodes,
                         const Configuration& config);
    void selectKeyCode(HIDKey::Keycode keyCode, bool pressed);

    void pressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void unpressedKeyWithConfig(const ButtonID& button, const Configuration& config) final;
    void terminateUpdateKeysWithConfig(const Configuration& config) final;
    void resetKeysWithConfig(const Configuration& config) final;

private:
    static constexpr uint32_t HID_SEQUENCE_PRESS_DURATION_MS = 150;
    std::vector<AbstractHID*> m_abstractHIDs;

    void selectTouchForSenders(const ButtonID& button, bool bPressed, const Configuration& config);
};

#endif /* _COMPUTERHIDMESSAGEHANDLER_H_ */
