/*
 * S25FLFlash.hpp
 *
 *  Created on: 7 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_PRODUCTS_MX25LFLASH_HPP_
#define NEMEIO_SFLASH_PRODUCTS_MX25LFLASH_HPP_

#include <SecureFlashIO.hpp>
#include "Flash.hpp"
#include "QSPISFlashCommunicationInterface.hpp"
#include "SPISFlashCommunicationInterface.hpp"


class MX25LFlash : public Flash, public SecureFlashIO
{
public:
    explicit MX25LFlash(const FlashInfo& flashInfo,
                        QSPISFlashCommunicationInterface& flashCommunication);
    explicit MX25LFlash(const FlashInfo& flashInfo,
                        SPISFlashCommunicationInterface& flashCommunication);
    virtual ~MX25LFlash() = default;

    bool init() final;
    bool deinit() final;

    bool readSecure(void* buffer, uint32_t addr, size_t size) final;
    bool writePageSecure(uint32_t addr, size_t size, uint8_t* data) final;
    bool sectorEraseSecure(uint32_t addr) final;

private:
    enum class MX25LOpCode
    {
        /* Secure mode */
        ENTER_SECURITY_MODE = 0xB1,
        EXIT_SECURITY_MODE = 0xC1,
    };

    static constexpr uint8_t MX25L_QSPI_STATUS_REGISTER_BIT_INDEX = 6;

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

    static constexpr FlashIOLut FLASH_SPI_LUT = {
        .addrMode = addressingMode::FLASH_3_BYTES_ADDRESSING,
        .readCommand = static_cast<uint8_t>(OpCode::READ_HIGHER_SPEED_FAST_READ),
        .writePageCommand = static_cast<uint8_t>(OpCode::PAGE_PROGRAM_PP),
        .eraseCommand = static_cast<uint8_t>(OpCode::SECTOR_ERASE_SE),
        .readCommandDummy = 1,
    };

    ISFlashCommunicationInterface& mFlashComInterface;
    bool mQuadMode = false;

    bool enterQuadMode();

    bool enterSecureMode();
    bool exitSecureMode();
};

#endif /* NEMEIO_SFLASH_PRODUCTS_S25FLFLASH_HPP_ */
