#pragma once

#include "Event.hpp"
#include "etl/string_view.h"

class TestEvent : public Event
{
public:
    explicit TestEvent(etl::string_view description);
    ~TestEvent() final = default;

private:
    friend AbstractSerialAdapter& operator<<(AbstractSerialAdapter& adapter, const TestEvent& event);

    const etl::string_view m_description;
};
