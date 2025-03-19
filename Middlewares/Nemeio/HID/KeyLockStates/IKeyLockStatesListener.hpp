/*
 * IKeyLockStatesListener.hpp
 *
 *  Created on: Sep 25, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_HID_KEYLOCKSTATES_IKEYLOCKSTATESLISTENER_HPP_
#define NEMEIO_HID_KEYLOCKSTATES_IKEYLOCKSTATESLISTENER_HPP_

#include "HIDDefs.hpp"
#include "Identifiable.hpp"

class IKeyLockStatesListener
{
public:
    IKeyLockStatesListener() = default;
    virtual ~IKeyLockStatesListener() = default;

    virtual void keyLockStatesChanged(const Identifiable& source,
                                      const HIDDefs::KeyLockStates& keyLockStates) = 0;
};

#endif /* NEMEIO_HID_KEYLOCKSTATES_IKEYLOCKSTATESLISTENER_HPP_ */
