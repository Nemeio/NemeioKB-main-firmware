/*
 * IScreenVersionsReader.hpp
 *
 *  Created on: 29 juil. 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_DISPLAY_ISCREENVERSIONSREADER_HPP_
#define NEMEIO_DISPLAY_ISCREENVERSIONSREADER_HPP_

#include "ScreenDefs.hpp"
#include <cstddef>

class IScreenVersionsReader
{
public:
    IScreenVersionsReader() = default;
    virtual ~IScreenVersionsReader() = default;

    virtual ScreenDefs::ScreenErrCode getVersions(char* firmwareVersionBuffer,
                                                  std::size_t firmwareVersionBufferLen,
                                                  char* lutVersionBuffer,
                                                  std::size_t lutVersionBufferLen) const = 0;
};

#endif /* NEMEIO_DISPLAY_ISCREENVERSIONSREADER_HPP_ */
