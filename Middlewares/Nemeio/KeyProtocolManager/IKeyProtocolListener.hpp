/*
 * KeyProtocolListener.hpp
 *
 *  Created on: Nov 19, 2018
 *      Author: Labo
 */

#ifndef KEYPROTOCOLMANAGER_IKEYPROTOCOLLISTENER_HPP_
#define KEYPROTOCOLMANAGER_IKEYPROTOCOLLISTENER_HPP_

#include "KeyProtocolDefs.hpp"

class IKeyProtocolListener
{
public:
    virtual ~IKeyProtocolListener() = default;

    virtual void keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode keyProtocolMode) = 0;
};

#endif /* KEYPROTOCOLMANAGER_IKEYPROTOCOLLISTENER_HPP_ */
