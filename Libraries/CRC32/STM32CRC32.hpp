/*
 * STM32CRC32.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Labo
 */

#ifndef CRC32_STM32CRC32_HPP_
#define CRC32_STM32CRC32_HPP_

#include "ICRC32.hpp"
#include "stm32l4xx_hal.h"
#include "RecursiveMutex.hpp"

class STM32CRC32 : public ICRC32
{
public:
    STM32CRC32();
    ~STM32CRC32() final = default;

    void configure() final;
    bool compute(const uint8_t* buffer, size_t len, uint32_t& crc32) final;

    bool lock(DelayMs timeout) final;
    bool unlock() final;

private:
    CRC_HandleTypeDef mhCRC;
    RecursiveMutex mMutex;
};

#endif /* CRC32_STM32CRC32_HPP_ */
