/*
 * NRFFirmwarePackage.hpp
 *
 *  Created on: 2 mars 2021
 *      Author: thomas
 */

#ifndef FIRMWAREUPDATE_NRFFIRMWAREPACKAGE_HPP_
#define FIRMWAREUPDATE_NRFFIRMWAREPACKAGE_HPP_

#include <cstdint>
#include "INRFFirmwarePackageListener.hpp"

class NRFFirmwarePackage
{
public:
    explicit NRFFirmwarePackage(INRFFirmwarePackageListener& listener);
    virtual ~NRFFirmwarePackage() = default;

    void init(std::size_t len);
    bool receiveData(array_view<const uint8_t> data);

private:
    static constexpr uint32_t MAGIC_NUMBER_FIRMWARE_INIT = 0xB001AB1E;
    static constexpr uint32_t MAGIC_NUMBER_FIRMWARE_DATA = 0x0B1EB1E0;

    struct __attribute__((__packed__)) nrf_firmware_header
    {
        uint32_t magicNumber;
        uint32_t size;
        uint32_t nextFirmwareOffset;
    };

    enum class firmwareState
    {
        HEADER,
        FIRMWARE,
        END,
    };

    INRFFirmwarePackageListener& mListener;
    nrf_firmware_header mCurrentHeader;
    std::size_t mCurrentOffset;
    std::size_t mHeaderOffset;
    std::size_t mFirmwareOffset;
    std::size_t mTotalSize;
    std::size_t mNextHeader;
    firmwareState mCurrentState;

    [[nodiscard]] NRFFirmwarePackageType getFirmwareTypeFromMagicNumber(uint32_t magic) const;
};

#endif /* FIRMWAREUPDATE_NRFFIRMWAREPACKAGE_HPP_ */
