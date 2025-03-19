/*
 * FlashFactory.cpp
 *
 *  Created on: 6 déc. 2021
 *      Author: thomas
 */

#include <FlashFactory.hpp>
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"
#include "FlashTechErrorCode.hpp"

#define MODULE "flash_factory"

FlashFactory::FlashFactory(ISystem& system)
    : mSystem(system)
{
}

FlashPair FlashFactory::makeQspiFlash(uint32_t flashId, QSPISFlashCommunicationInterface& qspi)
{
    FlashPair flash = FlashPair(etl::nullopt, etl::nullopt);

    LOG_INFO(MODULE, "Make QSPI flash from id " << etl::showbase << etl::hex << flashId);

    switch(flashId)
    {
    case FLASH_EXTERNAL_CYPRESS_16MB_MANUFACTURER_INFO:
        mQspiFlash.emplace<S25FLFlash>(FLASH_EXTERNAL_CYPRESS_16MB_INFO, qspi);
        mQspiSecureFlash.emplace(etl::get<S25FLFlash>(mQspiFlash));
        flash = FlashPair(etl::reference_wrapper<IFlash>(etl::get<S25FLFlash>(mQspiFlash)),
                          etl::reference_wrapper<IFlashIO>(mQspiSecureFlash.value()));
        break;
    case FLASH_EXTERNAL_MACRONIX_32MB_MANUFACTURER_INFO:
        mQspiFlash.emplace<MX25LFlash>(FLASH_EXTERNAL_MACRONIX_32MB_INFO, qspi);
        mQspiSecureFlash.emplace(etl::get<MX25LFlash>(mQspiFlash));
        flash = FlashPair(etl::reference_wrapper<IFlash>(etl::get<MX25LFlash>(mQspiFlash)),
                          etl::nullopt); /* Force no secure mode to avoid irreversible changes */
        break;
    case FLASH_EXTERNAL_CYPRESS_32MB_MANUFACTURER_INFO:
        mQspiFlash.emplace<S25FLFlash>(FLASH_EXTERNAL_CYPRESS_32MB_INFO, qspi);
        mQspiSecureFlash.emplace(etl::get<S25FLFlash>(mQspiFlash));
        flash = FlashPair(etl::reference_wrapper<IFlash>(etl::get<S25FLFlash>(mQspiFlash)),
                          etl::reference_wrapper<IFlashIO>(mQspiSecureFlash.value()));
        break;
    default:
        LOG_ERR(MODULE,
                "Unsupported flash error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(
                                                              FlashTechErrorCode::UNSUPPORTED)));
        break;
    }

    return flash;
}

FlashPair FlashFactory::makeSpiFlash(uint32_t flashId, SPISFlashCommunicationInterface& spi)
{
    FlashPair flash = etl::make_pair(etl::nullopt, etl::nullopt);

    LOG_INFO(MODULE, "Make SPI flash from id " << etl::showbase << etl::hex << flashId);

    switch(flashId)
    {
    case FLASH_EXTERNAL_MACRONIX_4MB_MANUFACTURER_INFO:
        mSpiFlash.emplace<MX25LFlash>(FLASH_EXTERNAL_MACRONIX_4MB_INFO,
                                      spi); /* Force 3 bytes addressing for now */
        flash = FlashPair(etl::reference_wrapper<IFlash>(etl::get<MX25LFlash>(mSpiFlash)),
                          etl::nullopt);
        break;
    default:
        LOG_ERR(MODULE,
                "Unsupported flash error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(
                                                              FlashTechErrorCode::UNSUPPORTED)));
        break;
    }

    return flash;
}
