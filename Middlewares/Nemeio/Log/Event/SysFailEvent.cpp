/*
 * SysFailEvent.cpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Labo
 */

#include "SysFailEvent.hpp"

SysFailEvent::SysFailEvent(EventType id, const ArmRegisters& registers)
    : Event(id)
    , mRegisters(registers)
{
}

AbstractSerialAdapter& operator<<(AbstractSerialAdapter& adapter, const SysFailEvent& event)
{
    adapter << static_cast<const Event&>(event);
    adapter << event.mRegisters.R0;
    adapter << event.mRegisters.R1;
    adapter << event.mRegisters.R2;
    adapter << event.mRegisters.R3;
    adapter << event.mRegisters.R4;
    adapter << event.mRegisters.R5;
    adapter << event.mRegisters.R6;
    adapter << event.mRegisters.R7;
    adapter << event.mRegisters.R8;
    adapter << event.mRegisters.R9;
    adapter << event.mRegisters.R10;
    adapter << event.mRegisters.R11;
    adapter << event.mRegisters.R12;
    adapter << event.mRegisters.SP;
    adapter << event.mRegisters.LR;
    adapter << event.mRegisters.PC;
    adapter << event.mRegisters.xPSR;

    return adapter;
}
