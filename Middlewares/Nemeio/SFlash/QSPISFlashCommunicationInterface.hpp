/*
 * QSPISFlashCommunicationInterface.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_QSPISFLASHCOMMUNICATIONINTERFACE_HPP_
#define NEMEIO_SFLASH_QSPISFLASHCOMMUNICATIONINTERFACE_HPP_

#include "ISFlashCommunicationInterface.hpp"
#include "iQSPI.hpp"

class QSPISFlashCommunicationInterface : public ISFlashCommunicationInterface
{
public:
    explicit QSPISFlashCommunicationInterface(iQSPI& qspi);
    virtual ~QSPISFlashCommunicationInterface() = default;

    static constexpr size_t DEFAULT_FLASH_SIZE = 256;

    bool init(size_t size) final;
    bool deinit() final;

    bool transmit(FlashCommand& command) final;
    bool transmit(FlashCommand& command, uint8_t* txData, size_t size, bool bDMA = false) final;
    bool receive(FlashCommand& command, uint8_t* rxData, size_t size, bool bDMA = false) final;

    bool transmitAndWaitValue(FlashCommand& command, FlashWaitValue value) final;

private:
    iQSPI& mQspi;

    uint8_t addrModeToAddrNbBytes(addressingMode addrMode);
};

#endif /* NEMEIO_SFLASH_QSPISFLASHCOMMUNICATIONINTERFACE_HPP_ */
