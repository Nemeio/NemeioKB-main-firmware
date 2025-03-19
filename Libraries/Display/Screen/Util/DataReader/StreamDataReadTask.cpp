/*
 * StreamDataReadTask.cpp
 *
 *  Created on: Oct 22, 2019
 *      Author: Labo
 */

#include "StreamDataReadTask.hpp"
#include "assertTools.h"

#include "StreamCompressedFileDataReader.hpp"

StreamDataReadTask::StreamDataReadTask()
{
    BaseType_t ret = xTaskCreate(vStreamDataReadTask,
                                 "StreamDataReadTask",
                                 STREAM_DATA_READ_TASK_STACK_SIZE,
                                 this,
                                 STREAM_DATA_READ_TASK_PRIORITY,
                                 &mStreamDataReadTaskHandle);
    ASSERT(ret == pdPASS);

    mStreamDataReadQueue = xQueueCreate(STREAM_DATA_READ_QUEUE_SIZE, sizeof(StreamDataReadMsg));
    ASSERT(mStreamDataReadQueue != nullptr);
}

bool StreamDataReadTask::startRead(IStreamDataReader& dataReader)
{
    bool bRet = true;

    StreamDataReadMsg msg = {.pReader = &dataReader};

    if(xQueueSendToBack(mStreamDataReadQueue, &msg, STREAM_DATA_READ_MSG_DELAY) != pdPASS)
    {
        dataReader.notifyReadFinish();
        bRet = false;
    }

    return bRet;
}

void StreamDataReadTask::vStreamDataReadTask(void* pvParameters)
{
    StreamDataReadTask* pStreamDataReadTask = reinterpret_cast<StreamDataReadTask*>(pvParameters);
    ASSERT(pStreamDataReadTask != nullptr);

    StreamDataReadMsg msg;
    while(xQueueReceive(pStreamDataReadTask->mStreamDataReadQueue, &msg, portMAX_DELAY) == pdPASS)
    {
        IStreamDataReader* pReader = msg.pReader;
        ASSERT(pReader != nullptr);
        pReader->notifyReadStart();
        pReader->runRead();
        pReader->notifyReadFinish();
    }
}
