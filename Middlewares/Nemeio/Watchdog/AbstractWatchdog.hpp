/*
 * AbstractWatchdog.hpp
 *
 *  Created on: Aug 20, 2019
 *      Author: Labo
 */

#ifndef WATCHDOG_ABSTRACTWATCHDOG_HPP_
#define WATCHDOG_ABSTRACTWATCHDOG_HPP_

#include <cstdint>

class AbstractWatchdog
{
public:
    AbstractWatchdog(uint32_t timeoutSeconds);
    virtual ~AbstractWatchdog() = default;

    virtual bool init() = 0;
    virtual bool refresh() = 0;
    bool setTimeoutSeconds(uint32_t timeoutSeconds);
    uint32_t getTimeoutSeconds() const;

protected:
    uint32_t mTimeoutSeconds;

    virtual bool start() = 0;
};

inline AbstractWatchdog::AbstractWatchdog(uint32_t timeoutSeconds)
    : mTimeoutSeconds(timeoutSeconds)
{
}

inline bool AbstractWatchdog::setTimeoutSeconds(uint32_t timeoutSeconds)
{
    mTimeoutSeconds = timeoutSeconds;
    return start();
}

inline uint32_t AbstractWatchdog::getTimeoutSeconds() const
{
    return mTimeoutSeconds;
}

#endif /* WATCHDOG_ABSTRACTWATCHDOG_HPP_ */
