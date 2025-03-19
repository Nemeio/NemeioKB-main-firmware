/*
 * STM32CRC32.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Labo
 */

#include "STM32CRC32.hpp"
#include <string.h>

STM32CRC32::STM32CRC32()
{
    memset(&mhCRC, 0, sizeof(CRC_HandleTypeDef));
    mhCRC.Instance = CRC;
    mhCRC.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
    mhCRC.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
    mhCRC.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_BYTE;
    mhCRC.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
    mhCRC.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
}

bool STM32CRC32::compute(const uint8_t* buffer, size_t len, uint32_t& crc32)
{
    crc32 = HAL_CRC_Calculate(&mhCRC, reinterpret_cast<const uint32_t*>(buffer), len);

    crc32 ^= 0xFFFFFFFF;

    return true;
}

void STM32CRC32::configure()
{
    HAL_CRC_Init(&mhCRC);
}


bool STM32CRC32::lock(DelayMs timeout)
{
    return mMutex.lock(timeout);
}

bool STM32CRC32::unlock()
{
    return mMutex.unlock();
}