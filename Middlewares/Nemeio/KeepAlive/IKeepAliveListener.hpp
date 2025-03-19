/*
 * IKeepAliveListener.hpp
 *
 *  Created on: Sep 20, 2018
 *      Author: Labo
 */

#ifndef NEMEIO_KEEPALIVE_IKEEPALIVELISTENER_HPP_
#define NEMEIO_KEEPALIVE_IKEEPALIVELISTENER_HPP_

class IKeepAliveListener
{
public:
    virtual ~IKeepAliveListener() = default;

    virtual void keepAlive() = 0;
    virtual void keepAliveTimeout() = 0;
};

#endif /* NEMEIO_KEEPALIVE_IKEEPALIVELISTENER_HPP_ */
