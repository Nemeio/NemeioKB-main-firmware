/*
 * FlashStructures.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_QSPI_FLASHSTRUCTURES_HPP_
#define NEMEIO_QSPI_FLASHSTRUCTURES_HPP_

enum class SFlashCommunicationInterfaceAddrMode
{
    ADDR_MODE_NONE,
    ADDR_MODE_3_BYTES,
    ADDR_MODE_4_BYTES,
};

enum class SFlashCommunicationInterfaceMatchOperator
{
    MATCH_OPERATOR_AND,
};

struct FlashCommand
{
    uint8_t command;
    SFlashCommunicationInterfaceAddrMode addrMode;
    uint32_t address;
    uint8_t dummy;

    /* QSPI only : TODO Improve using dedicated structure */
    uint8_t dataNbLines;
};

struct FlashWaitValue
{
    uint32_t match;
    uint32_t mask;
    SFlashCommunicationInterfaceMatchOperator matchOperator;
    uint32_t size;
    uint32_t periodMs;
    uint8_t waitCommand;

    /* QSPI only : TODO Improve using dedicated structure */
    uint8_t waitCommandDataNbLines;
};

#endif /* NEMEIO_QSPI_FLASHSTRUCTURES_HPP_ */
