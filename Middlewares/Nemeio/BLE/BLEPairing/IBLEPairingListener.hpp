/*
 * IBLEPairingListener.hpp
 *
 *  Created on: Jul 9, 2019
 *      Author: Labo
 */

#ifndef BLE_BLESERIAL_IBLEPAIRINGLISTENER_HPP_
#define BLE_BLESERIAL_IBLEPAIRINGLISTENER_HPP_

#include "BLEDefs.hpp"

class IBLEPairingListener
{
public:
    IBLEPairingListener() = default;
    virtual ~IBLEPairingListener() = default;

    virtual void pairingStart(const BLEDefs::Passkey& passkey) = 0;
    virtual void pairingStop() = 0;
};

#endif /* BLE_BLESERIAL_IBLEPAIRINGLISTENER_HPP_ */
