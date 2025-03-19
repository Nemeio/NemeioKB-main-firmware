/*
 * DataSender.hpp
 *
 *  Created on: Sep 14, 2018
 *      Author: Labo
 */

#ifndef NEMEIO_CUSTOMKEYPROTOCOL_IKEYDATASENDER_HPP_
#define NEMEIO_CUSTOMKEYPROTOCOL_IKEYDATASENDER_HPP_

#include <stdint.h>
#include <stddef.h>

class IKeyDataSender
{
public:
    IKeyDataSender() = default;
    virtual ~IKeyDataSender() = default;

    virtual void sendKeyData(uint8_t* pData, size_t len) = 0;
};

#endif /* NEMEIO_CUSTOMKEYPROTOCOL_IKEYDATASENDER_HPP_ */
