/*
 * ISFlashCommunicationInterface.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_ISFLASHCOMMUNICATIONINTERFACE_HPP_
#define NEMEIO_SFLASH_ISFLASHCOMMUNICATIONINTERFACE_HPP_

#include <cstdint>
#include <cstddef>
#include "FlashStructures.hpp"

enum class addressingMode
{
    FLASH_3_BYTES_ADDRESSING,
    FLASH_4_BYTES_ADDRESSING,
};

class ISFlashCommunicationInterface
{
public:
    virtual bool init(size_t size) = 0;
    virtual bool deinit() = 0;

    virtual bool transmit(FlashCommand& command) = 0;
    virtual bool transmit(FlashCommand& command,
                          uint8_t* txData,
                          size_t size,
                          bool bDMA = false) = 0;
    virtual bool receive(FlashCommand& command, uint8_t* rxData, size_t size, bool bDMA = false) = 0;

    virtual bool transmitAndWaitValue(FlashCommand& command, FlashWaitValue value) = 0;
};

#endif /* NEMEIO_SFLASH_ISFLASHCOMMUNICATIONINTERFACE_HPP_ */
