/*
 * FileLogAdapter.hpp
 *
 *  Created on: Jun 3, 2019
 *      Author: Labo
 */

#ifndef LOG_LOGADAPTER_FILELOGADAPTER_HPP_
#define LOG_LOGADAPTER_FILELOGADAPTER_HPP_

#include "fs.hpp"

#include "../LogAdapter/AbstractNBufferLogAdapter.hpp"

template<std::size_t LogBufferSize>
class FileLogAdapter : public AbstractNBufferLogAdapter<LogBufferSize>
{
public:
    FileLogAdapter(fs::IFileSystem& fileSystem, const std::string& path);
    virtual ~FileLogAdapter() = default;

private:
    fs::File mFile;
    const std::string mPath;

    LogErrCode writeLog(const uint8_t* pData, std::size_t len);
};

template<std::size_t LogBufferSize>
inline FileLogAdapter<LogBufferSize>::FileLogAdapter(fs::IFileSystem& fileSystem,
                                                     const std::string& path)
    : AbstractNBufferLogAdapter<LogBufferSize>()
    , mFile(fileSystem)
    , mPath(path)
{
}

template<std::size_t LogBufferSize>
inline LogErrCode FileLogAdapter<LogBufferSize>::writeLog(const uint8_t* pData, std::size_t len)
{
    if(fs::FS_ERR_OK
       != mFile.open(mPath, fs::OpenFlag::CREAT | fs::OpenFlag::WRONLY | fs::OpenFlag::APPEND))
    {
        return LogErrCode::ADAPTER;
    }

    LogErrCode ret = LogErrCode::SUCCESS;


    if(ssize_t szDataWritten = mFile.write(pData, len);
       szDataWritten < 0 || static_cast<std::size_t>(szDataWritten) != len)
    {
        ret = LogErrCode::ADAPTER;
    }

    if(fscommon::FS_ERR_OK != mFile.close())
    {
        ret = LogErrCode::ADAPTER;
    }

    return ret;
}

#endif /* LOG_LOGADAPTER_FILELOGADAPTER_HPP_ */
