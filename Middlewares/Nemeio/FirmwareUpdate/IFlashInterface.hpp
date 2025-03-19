/*
 * IFlashInterface.hpp
 *
 *  Created on: Feb 10, 2020
 *      Author: Labo
 */

#ifndef FIRMWAREUPDATE_IFLASHINTERFACE_HPP_
#define FIRMWAREUPDATE_IFLASHINTERFACE_HPP_

#include <cstddef>

class IFlashInterface
{
public:
    IFlashInterface() = default;
    virtual ~IFlashInterface() = default;

    virtual bool write(void* pDestination, const void* pSource, uint32_t uLength) = 0;
    virtual bool eraseSize(void* pStart, uint32_t uLength) = 0;

    virtual void* getEnd() const = 0;
    virtual size_t getPageSize() const = 0;
    virtual bool isFlashZone(void* pStart, uint32_t uLength) const = 0;
    virtual bool isFlashErasableZone(void* pStart, uint32_t uLength) const = 0;
};

#endif /* FIRMWAREUPDATE_IFLASHINTERFACE_HPP_ */
