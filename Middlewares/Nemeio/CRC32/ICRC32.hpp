/*
 * ICRC32.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Labo
 */

#ifndef CRC32_ICRC32_HPP_
#define CRC32_ICRC32_HPP_

#include <cstddef>
#include <stdint.h>
#include "ILockable.hpp"

class ICRC32 : public ILockable
{
public:
    ICRC32() = default;
    virtual ~ICRC32() = default;

    virtual void configure() = 0;
    virtual bool compute(const uint8_t* buffer, size_t len, uint32_t& crc32) = 0;
};

#endif /* CRC32_ICRC32_HPP_ */
