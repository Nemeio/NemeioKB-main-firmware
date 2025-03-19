/*
 * FileSystemCoordinator.hpp
 *
 *  Created on: Jul 25, 2018
 *      Author: Labo
 */

#ifndef FILESYSTEM_FILESYSTEMTRIMMING_HPP_
#define FILESYSTEM_FILESYSTEMTRIMMING_HPP_

#include <functional>
#include <cstring>
#include <cstddef>
#include <array>
#include "Signal.hpp"
#include "task.h"
#include "queue.h"
#include "cmsis_os.h"
#include "IFileSystemCoordinator.hpp"
#include "FileSystemCommon.hpp"
#include "IFileSystem.hpp"
#include "FSNotify.hpp"

namespace fscommon
{
inline constexpr size_t MAX_NUMBER_TRIMMING_FILESYSTEM = 1;

class FileSystemTrimming : public IFileSystemCoordinator
{
public:
    explicit FileSystemTrimming(std::array<std::reference_wrapper<fscommon::IFileSystem>,
                                           MAX_NUMBER_TRIMMING_FILESYSTEM> fileSystem);
    virtual ~FileSystemTrimming() = default;

    void init();
    void notifyWrite();
    void notifyFSRequest();

private:
    static constexpr UBaseType_t ASYNCHRONOUS_TASK_PRIORITY = 0;
    static constexpr configSTACK_DEPTH_TYPE FSASYNCHRONOUS_TASK_STACK_SIZE = 386;
    static constexpr uint32_t TIMEOUT_BEFORE_TRIM = 2000;
    static constexpr TickType_t DELAY_QUEUE_ASYNC_TICKS = 10;
    static constexpr uint32_t COUNT_QUEUE_ASYNC = 5;


    static void vAsynchronousFSTask(void* pvParameters);
    void trim();

    bool isAsynchronousFSActivityInProgress();

    bool runExistingActivityWithinTimeout(const uint32_t timeoutMs);
    bool checkIfActivityAndEraseBlock(IFileSystem& fileSystem, std::size_t block);

    TaskHandle_t mHandle = nullptr;
    QueueHandle_t mQueueAsynchronousNotify = nullptr;
    std::array<std::reference_wrapper<fscommon::IFileSystem>, MAX_NUMBER_TRIMMING_FILESYSTEM>
        mFileSystem;
};
}; // namespace fscommon

#endif /* FILESYSTEM_FILESYSTEMTRIMMING_HPP_ */
