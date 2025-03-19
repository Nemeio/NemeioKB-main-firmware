/*
 * SystemTypes.hpp
 *
 *  Created on: Jan 28, 2020
 *      Author: Labo
 */

#ifndef TOOLS_SYSTEM_SYSTEMTYPES_HPP_
#define TOOLS_SYSTEM_SYSTEMTYPES_HPP_

#include "utils.hpp"

struct DelayUs
{
    static constexpr int32_t INFINITE_DELAY = -1;

    constexpr DelayUs(int32_t us)
        : valueUs(us){};
    constexpr DelayUs(const DelayUs& us) = default;

    constexpr bool isInfiniteDelay() const
    {
        // All negative values mean infinite delay.
        // This allows to create infinite delays from INFINITE_DELAY value
        // with any multiple of microseconds (e.g. DelaySec)
        return valueUs < 0;
    };

    constexpr int32_t getMs() const { return valueUs / NB_MICROSECS_IN_MILLISECS; }

    bool operator==(const DelayUs& delay) const
    {
        bool bRet = true;

        if(this->isInfiniteDelay() != delay.isInfiniteDelay())
        {
            bRet = false;
        }
        else if(!this->isInfiniteDelay())
        {
            bRet = this->valueUs == delay.valueUs;
        }

        return bRet;
    }

    bool operator!=(const DelayUs& delay) const { return !(delay == *this); }

    bool operator<=(const DelayUs& delay) const
    {
        bool bRet = false;

        if(delay.isInfiniteDelay())
        {
            bRet = true;
        }
        else if(!this->isInfiniteDelay())
        {
            bRet = this->valueUs <= delay.valueUs;
        }

        return bRet;
    }

    static constexpr int32_t getMaxUs() { return INT32_MAX; };
    static constexpr int32_t getMinUs() { return INT32_MIN; };

    int32_t valueUs;
};

struct DelayMs : public DelayUs
{
    static constexpr int32_t INFINITE_DELAY = -1;

    constexpr DelayMs(int32_t _value)
        : DelayUs(_value * NB_MICROSECS_IN_MILLISECS){};

    static constexpr int32_t getMaxMs() { return DelayUs::getMaxUs() / NB_MICROSECS_IN_MILLISECS; };
    static constexpr int32_t getMinMs() { return DelayUs::getMinUs() / NB_MICROSECS_IN_MILLISECS; };
};

struct DelaySec : public DelayMs
{
    static constexpr int32_t INFINITE_DELAY = -1;

    constexpr DelaySec(int32_t _value)
        : DelayMs(_value * NB_MILLISECS_IN_SECS){};

    static constexpr int32_t getMaxSec() { return DelayMs::getMaxMs() / NB_MILLISECS_IN_SECS; };
    static constexpr int32_t getMinSec() { return DelayMs::getMinMs() / NB_MILLISECS_IN_SECS; };
};

#endif /* TOOLS_SYSTEM_SYSTEMTYPES_HPP_ */
