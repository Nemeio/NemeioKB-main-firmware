/*
 * STM32System.hpp
 *
 *  Created on: 12 févr. 2021
 *      Author: thomas
 */

#ifndef SYSTEM_STM32SYSTEM_HPP_
#define SYSTEM_STM32SYSTEM_HPP_

#include "ISystem.hpp"

class STM32System : public ISystem
{
public:
    STM32System() = default;
    virtual ~STM32System() override = default;

    void reset() override;
    uint32_t getTickCount() override;
    uint32_t delayToTicks(const DelayMs& delay) override;
    void delay(const DelayMs& delay) override;
    void disableInterrupt() override;
    void enableInterrupt() override;
};

#endif /* SYSTEM_STM32SYSTEM_HPP_ */
