/*
 * IBatteryStateListener.hpp
 *
 *  Created on: Jul 16, 2018
 *      Author: Labo
 */

#ifndef POWERMANAGER_IBATTERYSTATELISTENER_HPP_
#define POWERMANAGER_IBATTERYSTATELISTENER_HPP_

class IBatteryStateListener
{
public:
    IBatteryStateListener() = default;
    virtual ~IBatteryStateListener() = default;

    virtual void lowBatteryState() = 0;
    virtual void normalBatteryState() = 0;
    virtual void charging() = 0;
};

#endif /* POWERMANAGER_IBATTERYSTATELISTENER_HPP_ */
