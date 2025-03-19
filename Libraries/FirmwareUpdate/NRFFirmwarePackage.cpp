/*
 * NRFFirmwarePackage.cpp
 *
 *  Created on: 2 mars 2021
 *      Author: thomas
 */

#include "NRFFirmwarePackage.hpp"
#include <cstdint>
#include <cstring>


NRFFirmwarePackage::NRFFirmwarePackage(INRFFirmwarePackageListener& listener)
    : mListener(listener)
{
    init(0);
}

void NRFFirmwarePackage::init(std::size_t len)
{
    memset(&mCurrentHeader, 0, sizeof(mCurrentHeader));
    mTotalSize = len;
    mCurrentOffset = 0;
    mCurrentState = firmwareState::HEADER;
    mHeaderOffset = 0;
    mFirmwareOffset = 0;
    mNextHeader = 0;
}


bool NRFFirmwarePackage::receiveData(array_view<const uint8_t> data)
{
    std::size_t currentDataOffset = 0;
    std::size_t copySize;
    bool ret = true;
    do
    {
        switch(mCurrentState)
        {
        case firmwareState::HEADER:
            copySize = std::min(static_cast<std::size_t>(sizeof(mCurrentHeader)
                                                         - mCurrentHeader.nextFirmwareOffset),
                                data.size() - currentDataOffset);
            memcpy(reinterpret_cast<uint8_t*>(&mCurrentHeader) + mHeaderOffset,
                   data.data() + currentDataOffset,
                   copySize);

            mHeaderOffset += copySize;
            currentDataOffset += copySize;
            if(mHeaderOffset >= sizeof(mCurrentHeader))
            {
                mNextHeader = mCurrentHeader.nextFirmwareOffset;
                mCurrentState = firmwareState::FIRMWARE;
                mHeaderOffset = 0;
                ret = mListener.onManifest(getFirmwareTypeFromMagicNumber(
                                               mCurrentHeader.magicNumber),
                                           static_cast<std::size_t>(mCurrentHeader.size));
            }
            break;
        case firmwareState::FIRMWARE:
            copySize = std::min(static_cast<std::size_t>(mCurrentHeader.size - mFirmwareOffset),
                                data.size() - currentDataOffset);

            ret = mListener.onFirmware(
                array_view<const uint8_t>(data.data() + currentDataOffset, copySize));

            mFirmwareOffset += copySize;
            currentDataOffset += copySize;
            if(mFirmwareOffset >= mCurrentHeader.size)
            {
                mCurrentState = firmwareState::END;
                mFirmwareOffset = 0;
                ret = mListener.onFirmwareEnd();
            }
            break;
        case firmwareState::END:
            if(mCurrentHeader.nextFirmwareOffset != 0)
            {
                memset(&mCurrentHeader, 0, sizeof(mCurrentHeader));
                mCurrentState = firmwareState::HEADER;
            }
            else if(currentDataOffset != data.size())
            {
                ret = false;
            }
            break;
        default:
            /* Force assert */
            ASSERT(false);
            break;
        }
    } while(ret && currentDataOffset != data.size());
    mCurrentOffset += currentDataOffset;

    return ret;
}

NRFFirmwarePackageType NRFFirmwarePackage::getFirmwareTypeFromMagicNumber(uint32_t magic) const
{
    NRFFirmwarePackageType type = NRFFirmwarePackageType::FIRMWARE_UNKNOWN;
    switch(magic)
    {
    case MAGIC_NUMBER_FIRMWARE_INIT:
        type = NRFFirmwarePackageType::FIRMWARE_INIT;
        break;
    case MAGIC_NUMBER_FIRMWARE_DATA:
        type = NRFFirmwarePackageType::FIRMWARE_DATA;
        break;
    default:
        break;
    }

    return type;
}
