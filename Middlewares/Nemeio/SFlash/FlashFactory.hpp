/*
 * FlashFactory.hpp
 *
 *  Created on: 6 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_FLASHFACTORY_HPP_
#define SFLASH_FLASHFACTORY_HPP_

#include <IFlashIO.hpp>
#include <SecureFlashIO.hpp>
#include <SecureFlashIOWrapper.hpp>
#include "etl/optional.h"
#include "etl/variant.h"
#include "etl/functional.h"

#include "iQSPI.hpp"
#include "iSPI.h"
#include "SingleChipSelector.h"
#include "ISystem.hpp"

#include "QSPISFlashCommunicationInterface.hpp"
#include "SPISFlashCommunicationInterface.hpp"
#include "MX25LFlash.hpp"
#include "S25FLFlash.hpp"
#include "Module.hpp"

using FlashPair = etl::pair<etl::optional<etl::reference_wrapper<IFlash>>,
                            etl::optional<etl::reference_wrapper<IFlashIO>>>;

class FlashFactory : public Module<Identification::ModuleId::FLASH_MEMORY>
{
public:
    explicit FlashFactory(ISystem& system);
    virtual ~FlashFactory() = default;

    /* Flash id bytes : Reserved, Device id LSB, Device id MSB, Manufacturer id */
    static constexpr uint32_t FLASH_EXTERNAL_CYPRESS_16MB_MANUFACTURER_INFO =
        0x00186001; /* Old keyboard */
    static constexpr uint32_t FLASH_EXTERNAL_MACRONIX_32MB_MANUFACTURER_INFO = 0x001920c2;
    static constexpr uint32_t FLASH_EXTERNAL_CYPRESS_32MB_MANUFACTURER_INFO = 0x00196001;
    static constexpr uint32_t FLASH_EXTERNAL_MACRONIX_4MB_MANUFACTURER_INFO =
        0x001620C2; /* ITE flash */


    FlashPair makeQspiFlash(uint32_t flashId, QSPISFlashCommunicationInterface& qspi);
    FlashPair makeSpiFlash(uint32_t flashId, SPISFlashCommunicationInterface& spi);

private:
    static constexpr FlashInfo FLASH_EXTERNAL_CYPRESS_16MB_INFO = {256,
                                                                   256,
                                                                   4 * 1024,
                                                                   MO_TO_BYTES(16)};
    static constexpr FlashInfo FLASH_EXTERNAL_MACRONIX_4MB_INFO = {256,
                                                                   256,
                                                                   4 * 1024,
                                                                   MO_TO_BYTES(4)};
    static constexpr FlashInfo FLASH_EXTERNAL_MACRONIX_32MB_INFO = {256,
                                                                    256,
                                                                    4 * 1024,
                                                                    MO_TO_BYTES(32)};
    static constexpr FlashInfo FLASH_EXTERNAL_CYPRESS_32MB_INFO = {256,
                                                                   256,
                                                                   4 * 1024,
                                                                   MO_TO_BYTES(32)};

    etl::variant<etl::monostate, MX25LFlash> mSpiFlash;
    etl::variant<etl::monostate, MX25LFlash, S25FLFlash> mQspiFlash;
    etl::optional<SecureFlashIOWrapper> mQspiSecureFlash = etl::nullopt;

    ISystem& mSystem;
};

#endif /* SFLASH_FLASHFACTORY_HPP_ */
