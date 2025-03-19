/*
 * MemDataReader.cpp
 *
 *  Created on: Aug 22, 2018
 *      Author: Labo
 */

#include <MemDataReader.hpp>
#include <cstddef>

MemDataReader::MemDataReader(const uint8_t* pBuffer, uint32_t buffSize)
    : mpDataBuffer(pBuffer)
    , mDataBuffSize(buffSize)
    , mCurrentOffset(0)
{
}

MemDataReader::~MemDataReader()
{
}

const uint8_t* MemDataReader::read(size_t size, size_t& retSize)
{
    if(mDataBuffSize - mCurrentOffset < size)
    {
        retSize = mDataBuffSize - mCurrentOffset;
    }
    else
    {
        retSize = size;
    }

    const uint8_t* ret = mpDataBuffer + mCurrentOffset;

    mCurrentOffset += retSize;

    return ret;
}

bool MemDataReader::getSize(std::size_t& size)
{
    size = mDataBuffSize;

    return true;
}

std::size_t MemDataReader::getMaxReadSize()
{
    return mDataBuffSize;
}
