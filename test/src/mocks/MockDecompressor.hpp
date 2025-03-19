#ifndef MOCKDECOMPRESSOR_HPP_
#define MOCKDECOMPRESSOR_HPP_

#include "gmock/gmock.h"
#include "IDecompressor.hpp"

class MockDecompressor : public IDecompressor
{
public:
    MOCK_METHOD(DecompressErrCode,
                decompress,
                (const uint8_t* buffSrc,
                 size_t sizeSrc,
                 const std::function<bool(uint8_t)>& outputLambda),
                (override));
    MOCK_METHOD(DecompressErrCode,
                getDecompressedDataLength,
                (uint8_t * pData, std::size_t dataLength, std::size_t& decompressedLength),
                (override));
};

#endif /* MOCKDECOMPRESSOR_HPP_ */
