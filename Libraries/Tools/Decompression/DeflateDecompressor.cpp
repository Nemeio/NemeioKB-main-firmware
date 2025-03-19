/*
 * DeflateDecompressor.cpp
 *
 *  Created on: Nov 4, 2019
 *      Author: Labo
 */

#include "DeflateDecompressor.hpp"
#include "utils.hpp"

DeflateDecompressor::DecompressErrCode DeflateDecompressor::getDecompressedDataLength(
    FileIStream& fileStream, std::size_t& decompressedLength)
{
    DecompressErrCode ret = DecompressErrCode::FS;

    std::size_t filePos = 0;
    if(fileStream.seek(-static_cast<int32_t>(sizeof(uint32_t)), filePos, fs::SeekFlag::END)
       == FileIOStreamErr::SUCCESS)
    {
        uint32_t decompressedLengthLE32 = 0;
        std::size_t readRetLen = 0;
        if(fileStream.read(reinterpret_cast<uint8_t*>(&decompressedLengthLE32),
                           sizeof(uint32_t),
                           readRetLen)
           == FileIOStreamErr::SUCCESS)
        {
            decompressedLength = LE32ToHost(decompressedLengthLE32);
            ret = DecompressErrCode::SUCCESS;
        }
    }

    return ret;
}

DeflateDecompressor::DecompressErrCode DeflateDecompressor::getDecompressedDataLength(
    uint8_t* pData, std::size_t dataLength, std::size_t& decompressedLength)
{
    DecompressErrCode ret = DecompressErrCode::DATA;

    if(dataLength >= sizeof(uint32_t))
    {
        const uint32_t* pDecompressedLengthLE32 = reinterpret_cast<const uint32_t*>(
            pData + dataLength - sizeof(uint32_t));
        decompressedLength = LE32ToHost(*pDecompressedLengthLE32);
        ret = DecompressErrCode::SUCCESS;
    }

    return ret;
}
