/*
 * FileObject.hpp
 *
 *  Created on: Jan 27, 2020
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_FILEOBJECT_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_FILEOBJECT_HPP_

#include <StreamFileDataReader.hpp>

#include "StreamObject.hpp"

template<std::size_t MAX_FILE_SIZE>
class FileObject : public StreamObject<FileObject<MAX_FILE_SIZE>>
{
public:
    FileObject(StreamDataReadTask& streamDataReadTask,
               fs::IFileSystem& fileSystem,
               std::string_view fileName,
               std::array<uint8_t, MAX_FILE_SIZE>& readFileBuffer);
    virtual ~FileObject() = default;

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
bool FileObject<MAX_FILE_SIZE>::doRead(std::array<uint8_t, READ_BUFF_SIZE>& readBuffer,
                                       std::array<uint8_t, STREAM_BUFF_SIZE>& streamBufferStorage,
                                       std::size_t streamTriggerLevel,
                                       std::array<uint8_t, READ_BUFF_SIZE>& sourceReadChunkBuffer,
                                       InitCallback initCb,
                                       ReadCallback readCb)
{
    StreamFileDataReader<READ_BUFF_SIZE, STREAM_BUFF_SIZE, STREAM_TIMEOUT, MAX_FILE_SIZE>
        reader(readBuffer,
               streamBufferStorage,
               streamTriggerLevel,
               mStreamDataReadTask,
               mFileSystem,
               mPath,
               mReadFileBuffer,
               sourceReadChunkBuffer);

    return this->commonRead(reader, initCb, readCb);
}

template<std::size_t MAX_FILE_SIZE>
FileObject<MAX_FILE_SIZE>::FileObject(StreamDataReadTask& streamDataReadTask,
                                      fs::IFileSystem& fileSystem,
                                      std::string_view fileName,
                                      std::array<uint8_t, MAX_FILE_SIZE>& readFileBuffer)
    : mStreamDataReadTask(streamDataReadTask)
    , mFileSystem(fileSystem)
    , mPath(fileName)
    , mReadFileBuffer(readFileBuffer)
{
}

#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_COMPRESSEDFILEOBJECT_HPP_ */
