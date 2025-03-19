/*
 * Event.cpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Labo
 */

#include "Event.hpp"

Event::Event(EventType id)
    : mId(id)
{
}

AbstractSerialAdapter& operator<<(AbstractSerialAdapter& adapter, const Event& event)
{
    return adapter << static_cast<underlying_EventType>(event.mId);
}
