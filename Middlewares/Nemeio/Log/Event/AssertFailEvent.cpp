/*
 * AssertFailEvent.cpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Labo
 */

#include "AssertFailEvent.hpp"

AssertFailEvent::AssertFailEvent(const ArmRegisters& registers)
    : SysFailEvent(EventType::ASSERTFAIL, registers)
{
}
