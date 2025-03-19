/*
 * IITBLECommStrategyListener.hpp
 *
 *  Created on: Nov 26, 2019
 *      Author: Labo
 */

#ifndef BLE_IITBLECOMMSTRATEGYLISTENER_HPP_
#define BLE_IITBLECOMMSTRATEGYLISTENER_HPP_

class IITBLECommStrategyListener
{
public:
    IITBLECommStrategyListener() = default;
    virtual ~IITBLECommStrategyListener() = default;

    virtual void bleDataToSendAvailableIsr() = 0;
};

#endif /* BLE_IITBLECOMMSTRATEGYLISTENER_HPP_ */
