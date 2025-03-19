/*
 * ILEDStrategy.hpp
 *
 *  Created on: 6 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_LEDS_ILEDSTRATEGY_HPP_
#define NEMEIO_LEDS_ILEDSTRATEGY_HPP_

class ILEDStrategy
{
public:
    virtual ~ILEDStrategy() = default;

    virtual void startLedStrategy() = 0;
    virtual void stopLedStrategy() = 0;
};

#endif /* NEMEIO_LEDS_ILEDSTRATEGY_HPP_ */
