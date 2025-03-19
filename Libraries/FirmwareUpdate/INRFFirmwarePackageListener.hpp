/*
 * INRFFirmwarePackageListener.hpp
 *
 *  Created on: 2 mars 2021
 *      Author: thomas
 */

#ifndef FIRMWAREUPDATE_INRFFIRMWAREPACKAGELISTENER_HPP_
#define FIRMWAREUPDATE_INRFFIRMWAREPACKAGELISTENER_HPP_

#include <array_view.hpp>
#include <cstdint>

enum class NRFFirmwarePackageType
{
    FIRMWARE_UNKNOWN,
    FIRMWARE_INIT,
    FIRMWARE_DATA,
};

class INRFFirmwarePackageListener
{
public:
    virtual ~INRFFirmwarePackageListener() = default;

    virtual bool onManifest(NRFFirmwarePackageType type, std::size_t len) = 0;
    virtual bool onFirmware(array_view<const uint8_t> data) = 0;
    virtual bool onFirmwareEnd() = 0;
};


#endif /* FIRMWAREUPDATE_INRFFIRMWAREPACKAGELISTENER_HPP_ */
