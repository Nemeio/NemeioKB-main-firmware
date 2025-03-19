/*
 * FlashInfo.hpp
 *
 *  Created on: 2 déc. 2021
 *      Author: thomas
 */

#ifndef SFLASH_FLASHINFO_HPP_
#define SFLASH_FLASHINFO_HPP_

#include <cstdint>
#include <cstddef>

struct FlashInfo
{
    size_t mReadSize;
    size_t mPageSize;
    size_t mEraseSize;
    size_t mTotalSize;
};

#endif /* SFLASH_FLASHINFO_HPP_ */
