/*
 * IConnectivityListener.hpp
 *
 *  Created on: Oct 10, 2019
 *      Author: Labo
 */

#ifndef CONNECTIVITY_ICONNECTIVITYLISTENER_HPP_
#define CONNECTIVITY_ICONNECTIVITYLISTENER_HPP_

class IConnectivityListener
{
public:
    IConnectivityListener() = default;
    virtual ~IConnectivityListener() = default;

    virtual void onConnectivityChangeUsb() = 0;
    virtual void onConnectivityChangeBle() = 0;
    virtual void onConnectivityChangeNone() = 0;
};

#endif /* CONNECTIVITY_ICONNECTIVITYLISTENER_HPP_ */
