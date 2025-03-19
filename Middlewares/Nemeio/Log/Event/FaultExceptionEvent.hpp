/*
 * FaultExceptionEvent.hpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Labo
 */

#ifndef LOG_EVENT_FAULTEXCEPTIONEVENT_HPP_
#define LOG_EVENT_FAULTEXCEPTIONEVENT_HPP_

#include "SysFailEvent.hpp"

class FaultExceptionEvent : public SysFailEvent
{
public:
    FaultExceptionEvent(const ArmRegisters& registers, uint32_t exceptType);
    ~FaultExceptionEvent() final = default;

private:
    friend AbstractSerialAdapter& operator<<(AbstractSerialAdapter& adapter,
                                             const FaultExceptionEvent& event);
    uint32_t mExceptType;
};

#endif /* LOG_EVENT_FAULTEXCEPTIONEVENT_HPP_ */
