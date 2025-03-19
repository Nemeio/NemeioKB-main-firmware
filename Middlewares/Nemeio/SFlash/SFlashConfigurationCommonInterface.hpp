/*
 * SFlashConfigurationCommonInterface.hpp
 *
 *  Created on: 6 janv. 2022
 *      Author: thomas
 */

#ifndef NEMEIO_SFLASH_SFLASHCONFIGURATIONCOMMONINTERFACE_HPP_
#define NEMEIO_SFLASH_SFLASHCONFIGURATIONCOMMONINTERFACE_HPP_

#include "ISFlashCommunicationInterface.hpp"
#include "IFlashConfiguration.hpp"
#include "CommonFlashInterface.hpp"

class SFlashConfigurationCommonInterface : public IFlashConfiguration
{
public:
    explicit SFlashConfigurationCommonInterface(ISFlashCommunicationInterface& com);
    virtual ~SFlashConfigurationCommonInterface() = default;

    bool readStatusRegisterCmd(uint8_t* data, size_t size) final;
    bool readConfigurationRegisterCmd(uint8_t* data, size_t size) final;

    bool writeRegistersCmd(uint8_t* data, size_t size) final;

    uint64_t getUniqueId() final;
    uint32_t getIdentifier() final;

    bool waitMemReady();
    bool writeEnable();

private:
    ISFlashCommunicationInterface& mCommunicationInterface;

    bool readData(OpCode readRegInstruction, uint8_t* data, size_t size);
};

#endif /* NEMEIO_SFLASH_SFLASHCONFIGURATIONCOMMONINTERFACE_HPP_ */
