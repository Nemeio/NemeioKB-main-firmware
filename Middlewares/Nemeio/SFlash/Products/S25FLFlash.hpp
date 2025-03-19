/*
 * S25FLFlash.hpp
 *
 *  Created on: 7 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_PRODUCTS_S25FLFLASH_HPP_
#define NEMEIO_SFLASH_PRODUCTS_S25FLFLASH_HPP_

#include <SecureFlashIO.hpp>
#include "Flash.hpp"
#include "QSPISFlashCommunicationInterface.hpp"
#include "SPISFlashCommunicationInterface.hpp"


class S25FLFlash : public Flash, public SecureFlashIO
{
public:
    explicit S25FLFlash(const FlashInfo& flashInfo,
                        QSPISFlashCommunicationInterface& flashCommunication);
    virtual ~S25FLFlash() = default;

    bool init() final;
    bool deinit() final;

private:
    enum class S25FLOpCode
    {
        /* Secure commands */
        SECURITY_REGION_SECTOR_ERASE = 0x44,
        SECURITY_REGION_PROG = 0x42,
        SECURITY_REGION_READ = 0x48,
    };

    static constexpr uint8_t S25FL_QSPI_STATUS_REGISTER_BIT_INDEX = 1;

    static constexpr FlashInfo FLASH_SECURE_INFO = {1, 256, 4 * 1024, 4 * 1024};
    static constexpr FlashIOLut FLASH_QSPI_LUT_3BYTES_ADDRESSING = {
        .addrMode = addressingMode::FLASH_3_BYTES_ADDRESSING,
        .readCommand = static_cast<uint8_t>(OpCode::QUAD_O_READ_QOR),
        .writePageCommand = static_cast<uint8_t>(OpCode::QUAD_PAGE_PROG_QPP),
        .eraseCommand = static_cast<uint8_t>(OpCode::SECTOR_ERASE_SE),
        .readCommandDummy = 8,

        .readCommandDataNbLines = 4,
        .writePageCommandDataNbLines = 4,
    };

    static constexpr FlashIOLut FLASH_QSPI_LUT_4BYTES_ADDRESSING = {
        .addrMode = addressingMode::FLASH_4_BYTES_ADDRESSING,
        .readCommand = static_cast<uint8_t>(OpCode::QUAD_O_READ_QOR_4B),
        .writePageCommand = static_cast<uint8_t>(OpCode::QUAD_PAGE_PROG_QPP_4B),
        .eraseCommand = static_cast<uint8_t>(OpCode::SECTOR_ERASE_SE_4B),
        .readCommandDummy = 8,

        .readCommandDataNbLines = 4,
        .writePageCommandDataNbLines = 4,
    };


    static constexpr FlashIOLut FLASH_QSPI_SECURE_LUT = {
        .addrMode = addressingMode::FLASH_3_BYTES_ADDRESSING,
        .readCommand = static_cast<uint8_t>(S25FLOpCode::SECURITY_REGION_READ),
        .writePageCommand = static_cast<uint8_t>(S25FLOpCode::SECURITY_REGION_PROG),
        .eraseCommand = static_cast<uint8_t>(S25FLOpCode::SECURITY_REGION_READ),
        .readCommandDummy = 8,

        .readCommandDataNbLines = 1,
        .writePageCommandDataNbLines = 1,
    };

    bool enterQuadMode();
};

#endif /* NEMEIO_SFLASH_PRODUCTS_S25FLFLASH_HPP_ */
