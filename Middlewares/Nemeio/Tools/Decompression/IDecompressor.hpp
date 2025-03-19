/*
 * IDecompressor.hpp
 *
 *  Created on: Mar 11, 2019
 *      Author: Labo
 */

#ifndef NEMEIO_TOOLS_DECOMPRESSION_IDECOMPRESSOR_HPP_
#define NEMEIO_TOOLS_DECOMPRESSION_IDECOMPRESSOR_HPP_

#include <string>
#include <functional>

class IDecompressor
{
public:
    enum class DecompressErrCode
    {
        SUCCESS,
        DECOMPRESS,
        FS,
        DATA
    };

    IDecompressor() = default;
    virtual ~IDecompressor() = default;

    virtual DecompressErrCode decompress(const uint8_t* buffSrc,
                                         size_t sizeSrc,
                                         const std::function<bool(uint8_t)>& outputLambda) = 0;

    virtual DecompressErrCode getDecompressedDataLength(uint8_t* pData,
                                                        std::size_t dataLength,
                                                        std::size_t& decompressedLength) = 0;
};

#endif /* NEMEIO_TOOLS_DECOMPRESSION_IDECOMPRESSOR_HPP_ */
