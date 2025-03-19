/*
 * MemDataReader.hpp
 *
 *  Created on: Aug 22, 2018
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_MEMDATAREADER_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_MEMDATAREADER_HPP_

#include "IDataReader.hpp"
#include <cstddef>

class MemDataReader : public IDataReader
{
public:
    MemDataReader(const uint8_t* pDataBuffer, uint32_t dataBuffSize);
    virtual ~MemDataReader();

    const uint8_t* read(size_t size, size_t& retSize);
    bool getSize(std::size_t& size) final;
    std::size_t getMaxReadSize() final;

private:
    const uint8_t* mpDataBuffer;
    uint32_t mDataBuffSize;
    std::size_t mCurrentOffset;
};

#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_MEMDATAREADER_HPP_ */
