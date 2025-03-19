#include "TestEvent.hpp"

TestEvent::TestEvent(etl::string_view description)
    : Event(EventType::TEST)
    , m_description(description)
{
}

AbstractSerialAdapter& operator<<(AbstractSerialAdapter& adapter, const TestEvent& event)
{
    adapter << static_cast<const Event&>(event);
    adapter << static_cast<uint32_t>(event.m_description.size()) << event.m_description;

    return adapter;
}
