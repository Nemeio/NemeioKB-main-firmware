/*
 * ICommonFlashInterface.hpp
 *
 *  Created on: 10 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_IFLASHCONFIGURATION_HPP_
#define SFLASH_IFLASHCONFIGURATION_HPP_

#include <IFlashIO.hpp>
#include <cstdint>
#include <cstddef>

class IFlashConfiguration
{
public:
    virtual bool readStatusRegisterCmd(uint8_t* data, size_t size) = 0;
    virtual bool readConfigurationRegisterCmd(uint8_t* data, size_t size) = 0;

    virtual bool writeRegistersCmd(uint8_t* data, size_t size) = 0;

    virtual uint64_t getUniqueId() = 0;
    virtual uint32_t getIdentifier() = 0;

    virtual bool waitMemReady() = 0;
    virtual bool writeEnable() = 0;
};


#endif /* SFLASH_IFLASHCONFIGURATION_HPP_ */
