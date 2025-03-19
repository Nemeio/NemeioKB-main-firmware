/*
 * DataReader.hpp
 *
 *  Created on: Aug 22, 2018
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_IDATAREADER_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_IDATAREADER_HPP_

#include "ISizeReader.hpp"
#include <stdint.h>

class IDataReader : public ISizeReader
{
public:
    IDataReader() = default;
    virtual ~IDataReader() = default;

    virtual const uint8_t* read(size_t size, size_t& retSize) = 0;
};

#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_IDATAREADER_HPP_ */
