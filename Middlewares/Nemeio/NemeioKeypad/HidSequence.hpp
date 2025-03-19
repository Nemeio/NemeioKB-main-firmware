#pragma once

#include "HIDKey.hpp"
#include "ComputerHIDMessageHandler.h"
#include "etl/array_view.h"
#include "etl/vector.h"

class HidSequence
{
public:
    HidSequence(etl::array_view<const HIDKey::Keycode> keyCodes,
                ComputerHIDMessageHandler& computerHIDMessageHandler);

    void send(const Configuration& config);
    void clear(const Configuration& config);

private:
    static constexpr size_t MAX_KEYS = 3;
    etl::vector<HIDKey::Keycode, MAX_KEYS> m_keyCodes;
    ComputerHIDMessageHandler& m_computerHIDMessageHandler;
};