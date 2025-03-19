/*
 * GunzipDecompressor.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: Labo
 */

#include <Decompression/TinyDeflateDecompressor.hpp>
#include <LittleFileSystem.hpp>
#include "gunzip.hh"
#include "assertTools.h"
#include "AutoLock.h"
#include "timing_macros.h"

bool TinyDeflateDecompressor::writeFile(fs::IFileSystem& fileSystem,
                                        fscommon::FileHandle& fileDest,
                                        const uint8_t* buff,
                                        size_t len)
{
    ssize_t retLen = fileSystem.fileWrite(fileDest, buff, len);
    return (retLen >= 0 && static_cast<size_t>(retLen) == len);
}

TinyDeflateDecompressor::DecompressErrCode TinyDeflateDecompressor::decompress(
    const uint8_t* buffSrc, size_t sizeSrc, const std::function<bool(uint8_t)>& outputLambda)
{
    DecompressErrCode ret = DecompressErrCode::SUCCESS;

    ret = doInflate(buffSrc, sizeSrc, outputLambda);

    return ret;
}

TinyDeflateDecompressor::DecompressErrCode TinyDeflateDecompressor::doInflate(
    const uint8_t* buffSrc, size_t sizeSrc, const std::function<bool(uint8_t)>& outputLambda)
{
    AutoLock autolock(mInflateMutex);

    DecompressErrCode ret = DecompressErrCode::SUCCESS;

    auto deflRet = Deflate(buffSrc + 0,               // BidirectionalIterator begin
                           static_cast<int>(sizeSrc), // SizeType length
                           outputLambda               // OutputFunctor output
    );

    if(deflRet != 0)
    {
        ret = DecompressErrCode::DECOMPRESS;
    }

    return ret;
}

TinyDeflateDecompressor::DecompressErrCode TinyDeflateDecompressor::doInflate(
    const uint8_t* buffSrc, size_t sizeSrc, fs::IFileSystem& fileSystem, fs::FileHandle& fileDest)
{
    AutoLock autolock(mInflateMutex);

    DecompressErrCode ret = DecompressErrCode::SUCCESS;
    size_t dataToWriteIdx = 0;

    auto outputLambda = [&](uint8_t byte)
    {
        bool bRet = true;

        if(dataToWriteIdx < CHUNK_SIZE)
        {
            mInflateChunk[dataToWriteIdx++] = byte;

            if(dataToWriteIdx == CHUNK_SIZE)
            {
                if(writeFile(fileSystem, fileDest, mInflateChunk, dataToWriteIdx))
                {
                    bRet = false;
                }
                else
                {
                    ret = DecompressErrCode::FS;
                }
                dataToWriteIdx = 0;
            }
            else
            {
                bRet = false;
            }
        }

        return bRet;
    };

    auto deflRet = Deflate(buffSrc + 0,               // BidirectionalIterator begin
                           static_cast<int>(sizeSrc), // SizeType length
                           outputLambda               // OutputFunctor output
    );

    if(deflRet == 0)
    {
        if(dataToWriteIdx > 0)
        {
            writeFile(fileSystem, fileDest, mInflateChunk, dataToWriteIdx);
        }
    }
    else if(DecompressErrCode::SUCCESS == ret)
    {
        ret = DecompressErrCode::DECOMPRESS;
    }

    return ret;
}

TinyDeflateDecompressor::DecompressErrCode TinyDeflateDecompressor::decompress(
    const std::function<int16_t()>& inputLambda, const std::function<bool(uint8_t)>& outputLambda)
{
    AutoLock autolock(mInflateMutex);

    DecompressErrCode ret = DecompressErrCode::SUCCESS;

    auto deflRet = Deflate(inputLambda, // InputFunctor input
                           outputLambda // OutputFunctor output
    );

    if(deflRet != 0)
    {
        ret = DecompressErrCode::DECOMPRESS;
    }

    return ret;
}
