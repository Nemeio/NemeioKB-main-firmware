/*
 * SPISFlashCommunicationInterface.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_SPISFLASHCOMMUNICATIONINTERFACE_HPP_
#define NEMEIO_SFLASH_SPISFLASHCOMMUNICATIONINTERFACE_HPP_

#include "ISFlashCommunicationInterface.hpp"
#include "iSPI.h"
#include "SingleChipSelector.h"
#include "IFlashConfiguration.hpp"
#include "ISystem.hpp"

class SPISFlashCommunicationInterface : public ISFlashCommunicationInterface
{
public:
    SPISFlashCommunicationInterface(iSPI& spi, SingleChipSelector& chipSelector, ISystem& system);
    virtual ~SPISFlashCommunicationInterface() = default;

    bool init(size_t size) final;
    bool deinit() final;

    bool transmit(FlashCommand& command) final;
    bool transmit(FlashCommand& command, uint8_t* txData, size_t size, bool bDMA = false) final;
    bool receive(FlashCommand& command, uint8_t* rxData, size_t size, bool bDMA = false) final;

    bool transmitAndWaitValue(FlashCommand& command, FlashWaitValue value) final;

private:
    static constexpr uint8_t WRITE_IN_PROGRESS_MASK = 1 << 0;
    static constexpr uint8_t WRITE_ENABLE_LATCH_MASK = 1 << 1;
    static constexpr uint16_t STATUS_REGISTER_POLLING_MAX_ATTEMPT = 100;
    static constexpr int32_t STATUS_REGISTER_POLLING_DELAY_MS = 1;

    ISystem& mSystem;
    iSPI& mSpi;
    SingleChipSelector& mChipSelector;

    uint8_t addrModeToSize(SFlashCommunicationInterfaceAddrMode addrMode);
    ISPIErrCode sendAddress(uint32_t addr, uint8_t addrSize, bool bEndTransfer);
    void endTransfer();
};

#endif /* NEMEIO_SFLASH_SPISFLASHCOMMUNICATIONINTERFACE_HPP_ */
