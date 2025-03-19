/*
 * ISizeReader.hpp
 *
 *  Created on: 27 juil. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_UTIL_DATAREADER_ISIZEREADER_HPP_
#define NEMEIO_DISPLAY_UTIL_DATAREADER_ISIZEREADER_HPP_

#include <cstddef>

class ISizeReader
{
public:
    ISizeReader() = default;
    virtual ~ISizeReader() = default;

    virtual bool getSize(std::size_t& size) = 0;
    virtual size_t getMaxReadSize() = 0;
};

#endif /* NEMEIO_DISPLAY_UTIL_DATAREADER_ISIZEREADER_HPP_ */
