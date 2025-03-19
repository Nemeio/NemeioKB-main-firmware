/*
 * FaultExceptionEvent.cpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Labo
 */

#include "FaultExceptionEvent.hpp"

FaultExceptionEvent::FaultExceptionEvent(const ArmRegisters& registers, uint32_t exceptType)
    : SysFailEvent(EventType::FAULTEXCEPTION, registers)
    , mExceptType(exceptType)
{
}

AbstractSerialAdapter& operator<<(AbstractSerialAdapter& adapter, const FaultExceptionEvent& event)
{
    adapter << static_cast<const SysFailEvent&>(event);
    adapter << event.mExceptType;

    return adapter;
}
