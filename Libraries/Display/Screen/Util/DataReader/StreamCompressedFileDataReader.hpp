/*
 * StreamCompressedFileDataReader.hpp
 *
 *  Created on: Oct 21, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMCOMPRESSEDFILEDATAREADER_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMCOMPRESSEDFILEDATAREADER_HPP_

#include "StreamDataReader.hpp"
#include "Decompression/IFileDecompressor.hpp"

#include "FileIStream.hpp"

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
class StreamCompressedFileDataReader
    : public StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>
{
public:
    StreamCompressedFileDataReader(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
                                   std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
                                   std::size_t streamTriggerLevel,
                                   StreamDataReadTask& streamDataReadTask,
                                   IFileDecompressor& decompressor,
                                   fs::IFileSystem& fileSystem,
                                   std::string_view path,
                                   std::array<uint8_t, MAX_FILE_SIZE>& readFileBuffer,
                                   std::array<uint8_t, READ_BUFF_SIZE>& decompressChunkBuffer);
    virtual ~StreamCompressedFileDataReader() = default;

    bool getSize(std::size_t& size);

private:
    IFileDecompressor& mDecompressor;
    std::string_view mPath;
    FileIStream mFileStream;
    std::array<uint8_t, MAX_FILE_SIZE>& mReadFileBuffer;
    std::array<uint8_t, READ_BUFF_SIZE>& mDecompressChunkBuffer;

    bool doRunRead();

    bool decompressUnidirectionalInput(std::size_t fileSize,
                                       const std::function<bool(uint8_t)>& outputLambda);
    bool decompressRandomAccessInput(std::size_t fileSize,
                                     const std::function<bool(uint8_t)>& outputLambda);

    int16_t inputFuncUnidirectional(std::size_t& dataToReadIdx, std::size_t& remainingReadSize);
    bool ouputFunc(uint8_t byte, std::size_t chunkSize, std::size_t& dataToWriteIdx);
};

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
StreamCompressedFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>::
    StreamCompressedFileDataReader(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
                                   std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
                                   std::size_t streamTriggerLevel,
                                   StreamDataReadTask& streamDataReadTask,
                                   IFileDecompressor& decompressor,
                                   fs::IFileSystem& fileSystem,
                                   std::string_view path,
                                   std::array<uint8_t, MAX_FILE_SIZE>& readFileBuffer,
                                   std::array<uint8_t, READ_BUFF_SIZE>& decompressChunkBuffer)
    : StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>(readBuffer,
                                                                         streamBufferStorage,
                                                                         streamTriggerLevel,
                                                                         streamDataReadTask)
    , mDecompressor(decompressor)
    , mFileStream(fileSystem)
    , mReadFileBuffer(readFileBuffer)
    , mDecompressChunkBuffer(decompressChunkBuffer)
{
    mFileStream.open(path, fs::OpenFlag::RDONLY);
}

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
inline bool StreamCompressedFileDataReader<READ_BUFF_SIZE,
                                           STREAM_BUFF_SIZE,
                                           STREAM_TIMEOUT,
                                           MAX_FILE_SIZE>::getSize(std::size_t& decompressedSize)
{
    bool bRet = false;

    bRet = mDecompressor.getDecompressedDataLength(mFileStream, decompressedSize)
               == IDecompressor::DecompressErrCode::SUCCESS
           && mFileStream.reset() == FileIOStreamErr::SUCCESS;
    return bRet;
}

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
bool StreamCompressedFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>::
    doRunRead()
{
    bool bReadRet = false;
    std::size_t fileSize = 0;

    this->startStream();
    if(mFileStream.size(fileSize) == FileIOStreamErr::SUCCESS)
    {
        std::size_t chunkSize = this->mStreamTriggerLevel;

        size_t dataToWriteIdx = 0;

        auto outputLambda = [&dataToWriteIdx, chunkSize, this](uint8_t byte)
        { return ouputFunc(byte, chunkSize, dataToWriteIdx); };

        if(fileSize <= MAX_FILE_SIZE)
        {
            bReadRet = decompressRandomAccessInput(fileSize, outputLambda);
        }
        else
        {
            bReadRet = decompressUnidirectionalInput(fileSize, outputLambda);
        }

        if(bReadRet && dataToWriteIdx > 0)
        {
            std::size_t streamSendLen = dataToWriteIdx;
            std::size_t sentBytes = this->sendToStream(mDecompressChunkBuffer.data(),
                                                       streamSendLen,
                                                       STREAM_TIMEOUT);
            bReadRet = (sentBytes == streamSendLen);
        }
    }
    this->endStream();

    return bReadRet;
}

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
int16_t
StreamCompressedFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>::
    inputFuncUnidirectional(std::size_t& dataToReadIdx, std::size_t& remainingReadSize)
{
    int16_t signedByte = -1;

    bool bReadRet = true;

    if(0 == dataToReadIdx)
    {
        if(remainingReadSize == 0)
        {
            bReadRet = false;
        }
        else
        {
            std::size_t retReadLen = 0;
            std::size_t readSize = std::min(remainingReadSize, mReadFileBuffer.size());
            bReadRet = (mFileStream.read(mReadFileBuffer.data(), readSize, retReadLen)
                            == FileIOStreamErr::SUCCESS
                        && retReadLen == readSize);
            if(bReadRet)
            {
                remainingReadSize -= readSize;
            }
        }
    }

    if(bReadRet)
    {
        signedByte = mReadFileBuffer[dataToReadIdx];
        ++dataToReadIdx;

        if(mReadFileBuffer.size() == dataToReadIdx)
        {
            dataToReadIdx = 0;
        }
    }

    return signedByte;
}

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
inline bool
StreamCompressedFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>::
    decompressUnidirectionalInput(std::size_t fileSize,
                                  const std::function<bool(uint8_t)>& outputLambda)
{
    std::size_t dataToReadIdx = 0;
    std::size_t remainingReadSize = fileSize;

    auto inputLambda = [&dataToReadIdx, &remainingReadSize, this]()
    { return inputFuncUnidirectional(dataToReadIdx, remainingReadSize); };

    return IDecompressor::DecompressErrCode::SUCCESS
           == mDecompressor.decompress(inputLambda, outputLambda);
}

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
inline bool
StreamCompressedFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>::
    decompressRandomAccessInput(std::size_t fileSize,
                                const std::function<bool(uint8_t)>& outputLambda)
{
    std::size_t retReadLen = 0;
    return (FileIOStreamErr::SUCCESS
            == mFileStream.read(mReadFileBuffer.data(), fileSize, retReadLen))
           && retReadLen == fileSize
           && IDecompressor::DecompressErrCode::SUCCESS
                  == mDecompressor.decompress(mReadFileBuffer.data(), retReadLen, outputLambda);
}

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
bool StreamCompressedFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>::
    ouputFunc(uint8_t byte, std::size_t chunkSize, std::size_t& dataToWriteIdx)
{
    bool bSuccess = false;
    if(dataToWriteIdx < chunkSize)
    {
        bSuccess = true;
        mDecompressChunkBuffer[dataToWriteIdx++] = byte;

        if(dataToWriteIdx == chunkSize)
        {
            std::size_t streamSendLen = dataToWriteIdx;
            std::size_t sentBytes = this->sendToStream(mDecompressChunkBuffer.data(),
                                                       streamSendLen,
                                                       STREAM_TIMEOUT);
            bSuccess = (sentBytes == streamSendLen);
            dataToWriteIdx = 0;
        }
    }

    return !bSuccess;
}

#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMCOMPRESSEDFILEDATAREADER_HPP_ */
