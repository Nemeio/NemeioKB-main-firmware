/*
 * StreamFileDataReader.hpp
 *
 *  Created on: Oct 21, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMFILEDATAREADER_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMFILEDATAREADER_HPP_

#include "StreamDataReader.hpp"
#include "Decompression/IFileDecompressor.hpp"

#include "FileIStream.hpp"

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
class StreamFileDataReader
    : public StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>
{
public:
    StreamFileDataReader(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
                         std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
                         std::size_t streamTriggerLevel,
                         StreamDataReadTask& streamDataReadTask,
                         fs::IFileSystem& fileSystem,
                         std::string_view path,
                         std::array<uint8_t, MAX_FILE_SIZE>& readFileBuffer,
                         std::array<uint8_t, READ_BUFF_SIZE>& decompressChunkBuffer);
    virtual ~StreamFileDataReader() = default;

    bool getSize(std::size_t& size);

private:
    std::string_view mPath;
    FileIStream mFileStream;
    std::array<uint8_t, MAX_FILE_SIZE>& mReadFileBuffer;
    std::array<uint8_t, READ_BUFF_SIZE>& mDecompressChunkBuffer;

    bool doRunRead();
};

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
StreamFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>::
    StreamFileDataReader(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
                         std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
                         std::size_t streamTriggerLevel,
                         StreamDataReadTask& streamDataReadTask,
                         fs::IFileSystem& fileSystem,
                         std::string_view path,
                         std::array<uint8_t, MAX_FILE_SIZE>& readFileBuffer,
                         std::array<uint8_t, READ_BUFF_SIZE>& decompressChunkBuffer)
    : StreamDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT>(readBuffer,
                                                                         streamBufferStorage,
                                                                         streamTriggerLevel,
                                                                         streamDataReadTask)
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
inline bool
StreamFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>::getSize(
    std::size_t& decompressedSize)
{
    return mFileStream.size(decompressedSize) == FileIOStreamErr::SUCCESS;
}

template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         std::size_t MAX_FILE_SIZE>
bool StreamFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>::doRunRead()
{
    bool bReadRet = true;
    std::size_t fileSize = 0;

    this->startStream();
    if(mFileStream.size(fileSize) == FileIOStreamErr::SUCCESS)
    {
        std::size_t chunkSize = this->mStreamTriggerLevel;
        ASSERT(chunkSize <= mReadFileBuffer.size());
        size_t dataToWriteIdx = 0;

        do
        {
            if(mFileStream.read(mReadFileBuffer.data(), chunkSize, dataToWriteIdx)
                   == FileIOStreamErr::SUCCESS
               && dataToWriteIdx > 0)
            {
                std::size_t streamSendLen = dataToWriteIdx;
                std::size_t sentBytes = this->sendToStream(mReadFileBuffer.data(),
                                                           streamSendLen,
                                                           STREAM_TIMEOUT);
                bReadRet = (sentBytes == streamSendLen);
            }
            else
            {
                bReadRet = false;
            }
        } while(bReadRet);
    }
    this->endStream();

    return bReadRet;
}

#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMCOMPRESSEDFILEDATAREADER_HPP_ */
