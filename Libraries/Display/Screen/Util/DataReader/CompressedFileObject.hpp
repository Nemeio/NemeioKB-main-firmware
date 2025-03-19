/*
 * CompressedFileObject.hpp
 *
 *  Created on: Jan 27, 2020
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_COMPRESSEDCompressedFileObject_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_COMPRESSEDCompressedFileObject_HPP_

#include "StreamObject.hpp"
#include "StreamCompressedFileDataReader.hpp"

template<std::size_t MAX_FILE_SIZE>
class CompressedFileObject : public StreamObject<CompressedFileObject<MAX_FILE_SIZE>>
{
public:
    CompressedFileObject(StreamDataReadTask& streamDataReadTask,
                         IFileDecompressor& decompressor,
                         fs::IFileSystem& fileSystem,
                         std::string_view fileName,
                         std::array<uint8_t, MAX_FILE_SIZE>& readFileBuffer);
    virtual ~CompressedFileObject() = default;

    template<std::size_t READ_BUFF_SIZE,
             std::size_t STREAM_BUFF_SIZE,
             int32_t STREAM_TIMEOUT,
             typename InitCallback,
             typename ReadCallback>
    bool doRead(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
                std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
                std::size_t streamTriggerLevel,
                std::array<uint8_t, READ_BUFF_SIZE>& sourceReadChunkBuffer,
                InitCallback initCb,
                ReadCallback readCb);

private:
    StreamDataReadTask& mStreamDataReadTask;
    IFileDecompressor& mDecompressor;
    fs::IFileSystem& mFileSystem;
    std::string_view mPath;
    std::array<uint8_t, MAX_FILE_SIZE>& mReadFileBuffer;
};

template<std::size_t MAX_FILE_SIZE>
template<std::size_t READ_BUFF_SIZE,
         std::size_t STREAM_BUFF_SIZE,
         int32_t STREAM_TIMEOUT,
         typename InitCallback,
         typename ReadCallback>
bool CompressedFileObject<MAX_FILE_SIZE>::doRead(
    std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
    std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
    std::size_t streamTriggerLevel,
    std::array<uint8_t, READ_BUFF_SIZE>& sourceReadChunkBuffer,
    InitCallback initCb,
    ReadCallback readCb)
{
    StreamCompressedFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>
        reader(readBuffer,
               streamBufferStorage,
               streamTriggerLevel,
               mStreamDataReadTask,
               mDecompressor,
               mFileSystem,
               mPath,
               mReadFileBuffer,
               sourceReadChunkBuffer);

    return this->commonRead(reader, initCb, readCb);
}

template<std::size_t MAX_FILE_SIZE>
CompressedFileObject<MAX_FILE_SIZE>::CompressedFileObject(
    StreamDataReadTask& streamDataReadTask,
    IFileDecompressor& decompressor,
    fs::IFileSystem& fileSystem,
    std::string_view fileName,
    std::array<uint8_t, MAX_FILE_SIZE>& readFileBuffer)
    : mStreamDataReadTask(streamDataReadTask)
    , mDecompressor(decompressor)
    , mFileSystem(fileSystem)
    , mPath(fileName)
    , mReadFileBuffer(readFileBuffer)
{
}

#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_COMPRESSEDCompressedFileObject_HPP_ */
