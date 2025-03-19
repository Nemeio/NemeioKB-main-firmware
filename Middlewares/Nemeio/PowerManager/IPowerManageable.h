/*
 * IPowerManageable.h
 *
 *  Created on: Feb 28, 2018
 *      Author: amongatcheucheu
 */

#ifndef NEMEIO_POWERMANAGER_IPOWERMANAGEABLE_H_
#define NEMEIO_POWERMANAGER_IPOWERMANAGEABLE_H_

class IPowerManageable
{
public:
    IPowerManageable() = default;
    virtual ~IPowerManageable() = default;

    virtual void active() = 0;
    virtual void inactive() = 0;
    virtual void longInactive() = 0;
    virtual void sleep() = 0;
    virtual void powerOff() = 0;
    virtual void suspend() = 0;
};

#endif /* NEMEIO_POWERMANAGER_IPOWERMANAGEABLE_H_ */
