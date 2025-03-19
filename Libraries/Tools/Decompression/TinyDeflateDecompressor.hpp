/*
 * GunzipDecompressor.hpp
 *
 *  Created on: Mar 11, 2019
 *      Author: Labo
 */

#ifndef TOOLS_DECOMPRESSION_TINYDEFLATEDECOMPRESSOR_HPP_
#define TOOLS_DECOMPRESSION_TINYDEFLATEDECOMPRESSOR_HPP_

#include <fs.hpp>
#include "DeflateDecompressor.hpp"
#include "Mutex.hpp"

class TinyDeflateDecompressor : public DeflateDecompressor
{
public:
    TinyDeflateDecompressor() = default;
    ~TinyDeflateDecompressor() = default;

    DecompressErrCode decompress(const uint8_t* buffSrc,
                                 size_t sizeSrc,
                                 const std::function<bool(uint8_t)>& outputLambda);
    DecompressErrCode decompress(const std::function<int16_t()>& inputLambda,
                                 const std::function<bool(uint8_t)>& outputLambda);

private:
    static constexpr size_t CHUNK_SIZE = 1024;

    Mutex mInflateMutex;
    uint8_t mInflateChunk[CHUNK_SIZE];

    bool writeFile(fs::IFileSystem& fileSystem,
                   fs::FileHandle& fileDest,
                   const uint8_t* buff,
                   size_t len);
    DecompressErrCode doInflate(const uint8_t* buffSrc,
                                size_t sizeSrc,
                                fs::IFileSystem& fileSystem,
                                fs::FileHandle& fileDest);
    DecompressErrCode doInflate(const uint8_t* buffSrc,
                                size_t sizeSrc,
                                const std::function<bool(uint8_t)>& outputLambda);
};

#endif /* TOOLS_DECOMPRESSION_TINYDEFLATEDECOMPRESSOR_HPP_ */
