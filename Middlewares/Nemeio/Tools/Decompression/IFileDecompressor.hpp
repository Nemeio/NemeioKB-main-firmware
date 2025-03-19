/*
 * IFileDecompressor.hpp
 *
 *  Created on: Mar 11, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_TOOLS_DECOMPRESSION_IFILEDECOMPRESSOR_HPP_
#define NEMEIO_TOOLS_DECOMPRESSION_IFILEDECOMPRESSOR_HPP_

#include <string>
#include <functional>
#include "FileIStream.hpp"
#include "IDecompressor.hpp"

class IFileDecompressor : public IDecompressor
{
public:
    IFileDecompressor() = default;
    virtual ~IFileDecompressor() = default;

    virtual DecompressErrCode decompress(const uint8_t* buffSrc,
                                         size_t sizeSrc,
                                         const std::function<bool(uint8_t)>& outputLambda) = 0;
    virtual DecompressErrCode decompress(const std::function<int16_t()>& inputLambda,
                                         const std::function<bool(uint8_t)>& outputLambda) = 0;

    virtual DecompressErrCode getDecompressedDataLength(FileIStream& fileStream,
                                                        std::size_t& decompressedLength) = 0;
    virtual DecompressErrCode getDecompressedDataLength(uint8_t* pData,
                                                        std::size_t dataLength,
                                                        std::size_t& decompressedLength) = 0;
};

#endif /* NEMEIO_TOOLS_DECOMPRESSION_IFILEDECOMPRESSOR_HPP_ */
