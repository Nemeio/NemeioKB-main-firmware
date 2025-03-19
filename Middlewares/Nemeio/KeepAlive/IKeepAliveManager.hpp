/*
 * IKeepAliveManager.hpp
 *
 *  Created on: Feb 19, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_KEEPALIVE_IKEEPALIVEMANAGER_HPP_
#define NEMEIO_KEEPALIVE_IKEEPALIVEMANAGER_HPP_

class IKeepAliveManager
{
public:
    IKeepAliveManager() = default;
    virtual ~IKeepAliveManager() = default;

    virtual void receivedKeepAlive() = 0;
};

#endif /* NEMEIO_KEEPALIVE_IKEEPALIVEMANAGER_HPP_ */
