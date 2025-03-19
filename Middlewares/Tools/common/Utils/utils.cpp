/*
 * utils.cpp
 *
 *  Created on: Jul 11, 2019
 *      Author: Labo
 */

#include "utils.hpp"
#include "assertTools.h"

char* strncpyNullTerminated(char* destination, const char* source, size_t num)
{
    ASSERT(destination != nullptr && source != nullptr && num > 0);

    strncpy(destination, source, num - 1);
    destination[num - 1] = '\0';

    return destination;
}

bool charToDigit(char c, uint8_t& i)
{
    constexpr char zeroC = '0';
    constexpr char nineC = '9';

    bool bRet = false;

    if(c >= zeroC && c <= nineC)
    {
        i = c - zeroC;
        bRet = true;
    }

    return bRet;
}

etl::string<2> byteToHexStr(std::byte byte)
{
    etl::string<2> ret;
    snprintf(ret.data(), ret.capacity() + 1, "%.2x", static_cast<int>(byte));

    return ret;
}

void bitRightShiftInByteArray(const uint8_t* buffSrc,
                              const std::size_t lenSrc,
                              const uint8_t rightShift,
                              uint8_t* buffDst,
                              const std::size_t lenDst,
                              bool defaultBitsIsOne)
{
    ASSERT(buffSrc);
    ASSERT(buffDst);
    ASSERT(lenSrc);
    ASSERT(lenDst >= lenSrc + divRoundUp(rightShift, BITS_PER_BYTE));

    const uint8_t byteRightShift = rightShift % BITS_PER_BYTE;
    const uint8_t maskRight = (1 << byteRightShift) - 1;
    const uint8_t maskLeft = ((1 << (BITS_PER_BYTE - byteRightShift)) - 1);

    uint8_t modPrevious = 0;
    uint8_t modNext = 0;
    size_t iSrc = 0;
    size_t iDst = 0;

    // pad first bytes
    memset(buffDst, defaultBitsIsOne ? 0xff : 0x00, rightShift / BITS_PER_BYTE);
    iDst += rightShift / BITS_PER_BYTE;

    // manage bits to shift
    for(iSrc = 0; iSrc < lenSrc; ++iSrc)
    {
        uint8_t byteLoad = buffSrc[iSrc];

        modNext = maskRight & byteLoad;
        byteLoad >>= byteRightShift;
        byteLoad |= modPrevious << (BITS_PER_BYTE - byteRightShift);
        modPrevious = modNext;

        buffDst[iDst++] = byteLoad;
    }

    // pad last bytes
    if(defaultBitsIsOne)
    {
        buffDst[rightShift / BITS_PER_BYTE] |= ~maskLeft;
    }
    if(byteRightShift)
    {
        buffDst[iDst] = modPrevious << (BITS_PER_BYTE - byteRightShift);
        if(defaultBitsIsOne)
        {
            buffDst[iDst] |= maskLeft;
        }
        ++iDst;
    }

    memset(buffDst + iDst, defaultBitsIsOne ? 0xFF : 0x00, lenDst - iDst);
}
