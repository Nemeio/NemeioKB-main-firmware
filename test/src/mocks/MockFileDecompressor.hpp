#ifndef MOCKFILEDECOMPRESSOR_HPP_
#define MOCKFILEDECOMPRESSOR_HPP_

#include "gmock/gmock.h"
#include "IFileDecompressor.hpp"

class MockFileDecompressor : public IFileDecompressor
{
public:
    MOCK_METHOD(DecompressErrCode,
                decompress,
                (const uint8_t* buffSrc,
                 size_t sizeSrc,
                 fs::IFileSystem& fileSystem,
                 const std::string& path),
                (override));
    MOCK_METHOD(DecompressErrCode,
                decompress,
                (const uint8_t* buffSrc,
                 size_t sizeSrc,
                 const std::function<bool(uint8_t)>& outputLambda),
                (override));
    MOCK_METHOD(DecompressErrCode,
                decompress,
                (const std::function<int16_t()>& inputLambda,
                 const std::function<bool(uint8_t)>& outputLambda),
                (override));

    MOCK_METHOD(DecompressErrCode,
                getDecompressedDataLength,
                (FileIStream & fileStream, std::size_t& decompressedLength),
                (override));
    MOCK_METHOD(DecompressErrCode,
                getDecompressedDataLength,
                (uint8_t * pData, std::size_t dataLength, std::size_t& decompressedLength),
                (override));
};

#endif /* MOCKDECOMPRESSOR_HPP_ */
