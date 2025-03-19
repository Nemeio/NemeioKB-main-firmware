/*
 * IPowerManager.hpp
 *
 *  Created on: 21 avr. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_POWERMANAGER_IPOWERMANAGER_HPP_
#define NEMEIO_POWERMANAGER_IPOWERMANAGER_HPP_

class IPowerManager
{
public:
    IPowerManager() = default;
    virtual ~IPowerManager() = default;

    virtual void powerOff() = 0;
    virtual void userActivityHappened() const = 0;
    virtual void systemActivityHappened() const = 0;
};

#endif /* NEMEIO_POWERMANAGER_IPOWERMANAGER_HPP_ */
