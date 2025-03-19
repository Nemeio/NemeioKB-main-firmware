/*
 * StreamDataReadTask.hpp
 *
 *  Created on: Oct 22, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMDATAREADTASK_HPP_
#define DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMDATAREADTASK_HPP_

#include "IStreamDataReader.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

class StreamDataReadTask
{
public:
    StreamDataReadTask();
    virtual ~StreamDataReadTask() = default;

    // Note: dataReader must be valid until read is terminated
    bool startRead(IStreamDataReader& dataReader);
    void waitReadDone();

private:
    static constexpr UBaseType_t STREAM_DATA_READ_TASK_PRIORITY = 1;
    static constexpr configSTACK_DEPTH_TYPE STREAM_DATA_READ_TASK_STACK_SIZE = 300;
    static constexpr TickType_t STREAM_DATA_READ_QUEUE_SIZE = 1;
    static constexpr TickType_t STREAM_DATA_READ_MSG_DELAY = 10;
    struct StreamDataReadMsg
    {
        IStreamDataReader* pReader;
    };

    QueueHandle_t mStreamDataReadQueue;
    TaskHandle_t mStreamDataReadTaskHandle;

    static void vStreamDataReadTask(void* pvParameters);
};

#endif /* DISPLAY_SCREEN_EINKICE_UTIL_DATAREADER_STREAMDATAREADTASK_HPP_ */
