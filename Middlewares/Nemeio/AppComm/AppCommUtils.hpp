/*
 * AppCommUtils.hpp
 *
 *  Created on: Feb 5, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_APPCOMMUTILS_HPP_
#define APPCOMM_APPCOMMUTILS_HPP_

#include <stdint.h>
#include <stdlib.h>
#include "AppCommDefs.hpp"
#include "ConfigurationCommon.hpp"

class AppCommUtils
{
public:
    static bool parseU32(const uint8_t* pBuf, size_t len, size_t idx, uint32_t& val, size_t& idxEnd);

    static bool parseU16(const uint8_t* pBuf, size_t len, size_t idx, uint16_t& val, size_t& idxEnd);

    static bool parseU8(const uint8_t* pBuf, size_t len, size_t idx, uint8_t& val, size_t& idxEnd);

    static bool writeU32(uint32_t val, uint8_t* pBuf, size_t len, size_t idx, size_t& idxEnd);

    static bool writeU16(uint16_t val, uint8_t* pBuf, size_t len, size_t idx, size_t& idxEnd);

    static bool writeU8(uint8_t val, uint8_t* pBuf, size_t len, size_t idx, size_t& idxEnd);

    static AppCommDefs::AppCommErrCode convertConfigurationRetToAppCommDefsErrCode(
        configuration::Ret configurationRet);

private:
    static bool parseUINT(uint8_t typeSize,
                          const uint8_t* pBuf,
                          size_t len,
                          size_t idx,
                          uint32_t& val,
                          size_t& idxEnd);

    static bool writeUINT(
        uint8_t typeSize, uint32_t val, uint8_t* pBuf, size_t len, size_t idx, size_t& idxEnd);
};

#endif /* APPCOMM_APPCOMMUTILS_HPP_ */
