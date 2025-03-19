/*
 * SFlashIOCommonInterface.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_SFLASHIOCOMMONINTERFACE_HPP_
#define NEMEIO_SFLASH_SFLASHIOCOMMONINTERFACE_HPP_

#include "ISFlashCommunicationInterface.hpp"
#include "CommonFlashInterface.hpp"
#include "IFlashIO.hpp"
#include "IFlashConfiguration.hpp"

struct FlashIOLut
{
    addressingMode addrMode;

    uint8_t readCommand;
    uint8_t writePageCommand;
    uint8_t eraseCommand;

    uint8_t readCommandDummy;

    /* QSPI only : TODO Improve using dedicated structure */
    uint8_t readCommandDataNbLines;
    uint8_t writePageCommandDataNbLines;
};

class SFlashIOCommonInterface : public IFlashIO
{
public:
    explicit SFlashIOCommonInterface(IFlashConfiguration& flashConfig,
                                     ISFlashCommunicationInterface& communicationInterface,
                                     const FlashIOLut& command);
    virtual ~SFlashIOCommonInterface() = default;

    bool read(void* buffer, uint32_t addr, size_t size);
    bool writePage(uint32_t addr, size_t size, uint8_t* data);
    bool sectorErase(uint32_t addr);

    const FlashInfo getFlashInfo();

private:
    IFlashConfiguration& mFlashConfiguration;
    ISFlashCommunicationInterface& mCommunicationInterface;
    const FlashIOLut mFlashIOInfos;

    static inline SFlashCommunicationInterfaceAddrMode lutToAddrMode(addressingMode addrMode);
};

#endif /* NEMEIO_SFLASH_SFLASHIOCOMMONINTERFACE_HPP_ */
