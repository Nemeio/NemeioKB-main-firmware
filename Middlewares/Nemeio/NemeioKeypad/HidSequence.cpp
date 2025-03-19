#include "HidSequence.hpp"
#include "TraceLogger.hpp"

HidSequence::HidSequence(etl::array_view<const HIDKey::Keycode> keyCodes,
                         ComputerHIDMessageHandler& computerHIDMessageHandler)
    : m_computerHIDMessageHandler(computerHIDMessageHandler)
{
    if(keyCodes.size() <= m_keyCodes.capacity())
    {
        m_keyCodes.assign(keyCodes.begin(), keyCodes.end());
    }
}

void HidSequence::send(const Configuration& config)
{
    for(auto& keyCode: m_keyCodes)
    {
        m_computerHIDMessageHandler.selectKeyCode(keyCode, true);
    }
    m_computerHIDMessageHandler.terminateUpdateKeysWithConfig(config);
}

void HidSequence::clear(const Configuration& config)
{
    for(auto& keyCode: m_keyCodes)
    {
        m_computerHIDMessageHandler.selectKeyCode(keyCode, false);
    }
    m_computerHIDMessageHandler.terminateUpdateKeysWithConfig(config);
}