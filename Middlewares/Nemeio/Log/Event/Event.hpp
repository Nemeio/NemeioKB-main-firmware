/*
 * Event.hpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Labo
 */

#ifndef LOG_EVENT_EVENT_HPP_
#define LOG_EVENT_EVENT_HPP_

#include <cstdint>
#include "AbstractSerialAdapter.hpp"

using underlying_EventType = uint8_t;

enum class EventType : underlying_EventType
{
    FAULTEXCEPTION = 0,
    ASSERTFAIL,
    TEST
};

class Event
{
public:
    explicit Event(EventType id);
    virtual ~Event() = default;

private:
    friend AbstractSerialAdapter& operator<<(AbstractSerialAdapter& adapter, const Event& event);
    const EventType mId;
};

#endif /* LOG_EVENT_EVENT_HPP_ */
