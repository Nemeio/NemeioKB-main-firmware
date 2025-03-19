/*
 * SysFailEvent.hpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Labo
 */

#ifndef LOG_EVENT_SYSFAILEVENT_HPP_
#define LOG_EVENT_SYSFAILEVENT_HPP_

#include "Event.hpp"
#include "armRegisters.h"

class SysFailEvent : public Event
{
public:
    SysFailEvent(EventType id, const ArmRegisters& registers);
    ~SysFailEvent() override = default;

private:
    friend AbstractSerialAdapter& operator<<(AbstractSerialAdapter& adapter,
                                             const SysFailEvent& event);

    ArmRegisters mRegisters;
};

#endif /* LOG_EVENT_SYSFAILEVENT_HPP_ */
