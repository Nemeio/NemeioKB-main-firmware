/*
 * ISystem.hpp
 *
 *  Created on: Dec 17, 2019
 *      Author: Labo
 */

#ifndef TOOLS_SYSTEM_ISYSTEM_HPP_
#define TOOLS_SYSTEM_ISYSTEM_HPP_

#include <cstdint>
#include "SystemTypes.hpp"

class ISystem
{
public:
    ISystem() = default;
    virtual ~ISystem() = default;

    virtual void reset() = 0;
    virtual uint32_t getTickCount() = 0;
    virtual uint32_t delayToTicks(const DelayMs& delay) = 0;
    virtual void delay(const DelayMs& delay) = 0;
    virtual void disableInterrupt() = 0;
    virtual void enableInterrupt() = 0;
};

#endif /* TOOLS_SYSTEM_ISYSTEM_HPP_ */
