/*
 * AssertFailEvent.hpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Labo
 */

#ifndef LOG_EVENT_ASSERTFAILEVENT_HPP_
#define LOG_EVENT_ASSERTFAILEVENT_HPP_

#include "SysFailEvent.hpp"

class AssertFailEvent : public SysFailEvent
{
public:
    explicit AssertFailEvent(const ArmRegisters& registers);
    ~AssertFailEvent() final = default;
};

#endif /* LOG_EVENT_ASSERTFAILEVENT_HPP_ */
