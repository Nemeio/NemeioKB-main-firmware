/*
 * AppCommUtils.cpp
 *
 *  Created on: Feb 5, 2019
 *      Author: Labo
 */

#include "assertTools.h"
#include <AppCommUtils.hpp>

bool AppCommUtils::parseU32(
    const uint8_t* pBuf, size_t len, size_t idx, uint32_t& val, size_t& idxEnd)
{
    return parseUINT(sizeof(uint32_t), pBuf, len, idx, val, idxEnd);
}

bool AppCommUtils::parseU16(
    const uint8_t* pBuf, size_t len, size_t idx, uint16_t& val, size_t& idxEnd)
{
    uint32_t val32 = val;
    bool bRet = parseUINT(sizeof(uint16_t), pBuf, len, idx, val32, idxEnd);
    val = static_cast<uint16_t>(val32);
    return bRet;
}

bool AppCommUtils::parseU8(const uint8_t* pBuf, size_t len, size_t idx, uint8_t& val, size_t& idxEnd)
{
    uint32_t val32 = val;
    bool bRet = parseUINT(sizeof(uint8_t), pBuf, len, idx, val32, idxEnd);
    val = static_cast<uint8_t>(val32);
    return bRet;
}

bool AppCommUtils::parseUINT(
    uint8_t typeSize, const uint8_t* pBuf, size_t len, size_t idx, uint32_t& val, size_t& idxEnd)
{
    bool bRet = false;

    ASSERT(pBuf != NULL);

    val = 0;

    if(len >= (idx + typeSize))
    {
        for(uint8_t i = 0; i < typeSize; ++i)
        {
            val <<= 8;
            val |= pBuf[idx++];
        }

        idxEnd = idx;

        bRet = true;
    }

    return bRet;
}

bool AppCommUtils::writeUINT(
    uint8_t typeSize, uint32_t val, uint8_t* pBuf, size_t len, size_t idx, size_t& idxEnd)
{
    bool bRet = false;

    ASSERT(pBuf != NULL);

    if(len >= (idx + typeSize))
    {
        for(uint8_t i = typeSize; i > 0; --i)
        {
            pBuf[idx++] = (val >> 8 * (i - 1)) & 0xFF;
        }

        idxEnd = idx;

        bRet = true;
    }

    return bRet;
}

bool AppCommUtils::writeU32(uint32_t val, uint8_t* pBuf, size_t len, size_t idx, size_t& idxEnd)
{
    return writeUINT(sizeof(uint32_t), val, pBuf, len, idx, idxEnd);
}

bool AppCommUtils::writeU16(uint16_t val, uint8_t* pBuf, size_t len, size_t idx, size_t& idxEnd)
{
    return writeUINT(sizeof(uint16_t), val, pBuf, len, idx, idxEnd);
}

bool AppCommUtils::writeU8(uint8_t val, uint8_t* pBuf, size_t len, size_t idx, size_t& idxEnd)
{
    return writeUINT(sizeof(uint8_t), val, pBuf, len, idx, idxEnd);
}


AppCommDefs::AppCommErrCode AppCommUtils::convertConfigurationRetToAppCommDefsErrCode(
    configuration::Ret configurationRet)
{
    AppCommDefs::AppCommErrCode appCommErrCode;

    switch(configurationRet)
    {
    case configuration::Ret ::SUCCESS:
        appCommErrCode = AppCommDefs::APPCOMMERR_SUCCESS;
        break;
    case configuration::Ret ::INVALID_DATA:
        appCommErrCode = AppCommDefs::APPCOMMERR_INVALID_CONTENT;
        break;
    case configuration::Ret ::NOT_FOUND:
        appCommErrCode = AppCommDefs::APPCOMMERR_NOT_FOUND;
        break;
    case configuration::Ret ::FS:
        appCommErrCode = AppCommDefs::APPCOMMERR_FS;
        break;
    case configuration::Ret ::NO_SPACE:
        appCommErrCode = AppCommDefs::APPCOMMERR_NO_RESOURCE;
        break;
    case configuration::Ret ::STATE:
        appCommErrCode = AppCommDefs::APPCOMMERR_WRONG_COMM_MODE;
        break;
    default:
        appCommErrCode = AppCommDefs::APPCOMMERR_UNEXPECTED;
        break;
    }

    return appCommErrCode;
}
