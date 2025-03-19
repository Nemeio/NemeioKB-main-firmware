/*
 * FileSystemCoordinator.cpp
 *
 *  Created on: Jul 25, 2018
 *      Author: Labo
 */
#include "FileSystemTrimming.hpp"
#include <stddef.h>
#include <vector>
#include "timing_macros.h"
#include "FatFS_utils.hpp"
#include "assertTools.h"
#include "AutoLock.h"

using namespace fscommon;

FileSystemTrimming::FileSystemTrimming(std::array<std::reference_wrapper<fscommon::IFileSystem>,
                                                  MAX_NUMBER_TRIMMING_FILESYSTEM> fileSystem)
    : mFileSystem(fileSystem)
{
    mQueueAsynchronousNotify = xQueueCreate(COUNT_QUEUE_ASYNC, sizeof(FSNotify));

    ASSERT(mQueueAsynchronousNotify != nullptr);
}

void FileSystemTrimming::init()
{
    xTaskCreate(vAsynchronousFSTask,
                "FSAsynchronousTask",
                FSASYNCHRONOUS_TASK_STACK_SIZE,
                this,
                ASYNCHRONOUS_TASK_PRIORITY,
                &mHandle);
}

void FileSystemTrimming::notifyWrite()
{
    FSNotify fsWriting;

    // Since any activity will trigger the trim,
    // do not notify write if activities are already waiting in the queue
    if(!isAsynchronousFSActivityInProgress())
    {
        xQueueSend(mQueueAsynchronousNotify, &fsWriting, DELAY_QUEUE_ASYNC_TICKS);
    }
}

void FileSystemTrimming::notifyFSRequest()
{
}

bool FileSystemTrimming::isAsynchronousFSActivityInProgress()
{
    return uxQueueMessagesWaiting(mQueueAsynchronousNotify) > 0;
}

bool FileSystemTrimming::checkIfActivityAndEraseBlock(IFileSystem& fileSystem, std::size_t block)
{
    bool bRet = false;

    if(runExistingActivityWithinTimeout(0))
    {
        bRet = true;
    }
    else
    {
        fileSystem.bdErase(block);
    }

    return bRet;
}

void FileSystemTrimming::trim()
{
    while(runExistingActivityWithinTimeout(TIMEOUT_BEFORE_TRIM))
    {
        // Wait for no activity
    }

    bool continueTrim = true;
    for(const auto& fs: mFileSystem)
    {
        std::vector<bool> usedBlocks(fs.get().getBlockCount(), false);

        TraverseCallback traverseCallback = [&usedBlocks](std::size_t blockNum)
        {
            ASSERT(blockNum < usedBlocks.size());
            usedBlocks[blockNum] = true;
            return FS_ERR_OK;
        };

        if(fs.get().traverse(traverseCallback) == FS_ERR_OK)
        {
            for(auto it = usedBlocks.begin(); it != usedBlocks.end(); it++)
            {
                if(!(*it))
                {
                    fs.get().lock();
                    bool bActivityHappened = checkIfActivityAndEraseBlock(fs,
                                                                          it - usedBlocks.begin());
                    fs.get().unlock();

                    if(bActivityHappened)
                    {
                        continueTrim = false;
                        break;
                    }
                }
            }
        }

        if(!continueTrim)
        {
            break;
        }
    }
}

void FileSystemTrimming::vAsynchronousFSTask(void* pvParameters)
{
    auto pFS = reinterpret_cast<FileSystemTrimming*>(pvParameters);
    ASSERT(pFS != nullptr);
    TickType_t delay = portMAX_DELAY;
    for(;;)
    {
        if(pFS->runExistingActivityWithinTimeout(delay))
        {
            pFS->trim();
        }
    }
}

bool FileSystemTrimming::runExistingActivityWithinTimeout(const uint32_t timeoutMs)
{
    FSNotify fsNotification;
    bool bReturn = (xQueueReceive(mQueueAsynchronousNotify, &fsNotification, timeoutMs) == pdPASS);
    if(bReturn)
    {
        fsNotification.doFsAction();
    }

    return bReturn;
}
