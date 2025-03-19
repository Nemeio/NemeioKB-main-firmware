/*
 * DeflateDecompressor.hpp
 *
 *  Created on: Nov 4, 2019
 *      Author: Labo
 */

#ifndef TOOLS_DECOMPRESSION_DEFLATEDECOMPRESSOR_HPP_
#define TOOLS_DECOMPRESSION_DEFLATEDECOMPRESSOR_HPP_

#include "IFileDecompressor.hpp"

class DeflateDecompressor : public IFileDecompressor
{
public:
    DeflateDecompressor() = default;
    virtual ~DeflateDecompressor() = default;

    DecompressErrCode getDecompressedDataLength(FileIStream& fileStream,
                                                std::size_t& decompressedLength);
    DecompressErrCode getDecompressedDataLength(uint8_t* pData,
                                                std::size_t dataLength,
                                                std::size_t& decompressedLength);
};

#endif /* TOOLS_DECOMPRESSION_DEFLATEDECOMPRESSOR_HPP_ */
