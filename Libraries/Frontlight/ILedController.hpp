/*
 * ILedController.hpp
 *
 *  Created on: Feb 15, 2018
 *      Author: amongatcheucheu
 */

#ifndef FRONTLIGHT_ILEDCONTROLLER_HPP_
#define FRONTLIGHT_ILEDCONTROLLER_HPP_

#include "stdint.h"

class ILedController
{
public:
    ILedController() = default;
    virtual ~ILedController() = default;
    virtual void init() = 0;
    virtual bool setLightIntensity(uint8_t intensityPercent) = 0;
    virtual void powerOff() = 0;
};

#endif /* FRONTLIGHT_ILEDCONTROLLER_HPP_ */
