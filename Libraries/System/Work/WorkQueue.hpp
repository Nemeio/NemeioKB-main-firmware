#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "IWorkQueue.hpp"
#include "assertTools.h"
#include "systemUtils.h"

template<std::size_t STACK_SIZE, std::size_t QUEUE_NB_ITEMS, osPriority PRIORITY>
class WorkQueue : public IWorkQueue
{
public:
    WorkQueue();
    virtual ~WorkQueue() = default;

    bool submit(Work& work) final;
    bool submitData(Work& work, etl::array_view<const uint8_t> data) final;

private:
    static constexpr TickType_t WORK_QUEUE_SEND_DELAY = 10;
    static constexpr size_t WORK_QUEUE_MAX_DATA_SIZE = 4;

    struct Message
    {
        Work* work;
        size_t dataLength;
        uint8_t data[WORK_QUEUE_MAX_DATA_SIZE];
    };

    static void sWorkQueueTask(const void* argument);
    void workQueueTask();

    std::array<uint8_t, QUEUE_NB_ITEMS * sizeof(Message)> m_queueStorageArea;

    StaticTask_t m_workQueueTask{nullptr};
    std::array<StackType_t, STACK_SIZE> m_workQueueThreadStack{0};
    osThreadStaticDef(m_workQueueThreadDef,
                      sWorkQueueTask,
                      PRIORITY,
                      0,
                      STACK_SIZE,
                      m_workQueueThreadStack.data(),
                      &m_workQueueTask);
    StaticQueue_t m_staticQueue;
    QueueHandle_t m_queue;
};

template<std::size_t STACK_SIZE, std::size_t QUEUE_NB_ITEMS, osPriority PRIORITY>
WorkQueue<STACK_SIZE, QUEUE_NB_ITEMS, PRIORITY>::WorkQueue()
{
    m_queue = xQueueCreateStatic(QUEUE_NB_ITEMS,
                                 sizeof(Message),
                                 m_queueStorageArea.data(),
                                 &m_staticQueue);

    ASSERT(m_queue);

    osThreadCreate(osThread(m_workQueueThreadDef), this);
}

template<std::size_t STACK_SIZE, std::size_t QUEUE_NB_ITEMS, osPriority PRIORITY>
void WorkQueue<STACK_SIZE, QUEUE_NB_ITEMS, PRIORITY>::sWorkQueueTask(const void* argument)
{
    auto workQueue = reinterpret_cast<WorkQueue<STACK_SIZE, QUEUE_NB_ITEMS, PRIORITY>*>(
        const_cast<void*>(argument));

    ASSERT(workQueue);

    workQueue->workQueueTask();
}

template<std::size_t STACK_SIZE, std::size_t QUEUE_NB_ITEMS, osPriority PRIORITY>
void WorkQueue<STACK_SIZE, QUEUE_NB_ITEMS, PRIORITY>::workQueueTask()
{
    Message message;

    for(;;)
    {
        xQueueReceive(m_queue, &message, delayToSystemTicks(DelayMs(DelayMs::INFINITE_DELAY)));
        if(message.work)
        {
            message.work->run({message.data, message.dataLength});
        }
    }
}


template<std::size_t STACK_SIZE, std::size_t QUEUE_NB_ITEMS, osPriority PRIORITY>
bool WorkQueue<STACK_SIZE, QUEUE_NB_ITEMS, PRIORITY>::submitData(Work& work,
                                                                 etl::array_view<const uint8_t> data)
{
    ASSERT(data.size() <= WORK_QUEUE_MAX_DATA_SIZE);

    Message message;
    message.work = &work;
    message.dataLength = data.size();
    etl::copy(data.begin(), data.end(), message.data);

    BaseType_t ret;

    if(isInISRContext())
    {
        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;
        ret = xQueueSendToBackFromISR(m_queue,
                                      reinterpret_cast<void*>(&message),
                                      &xHigherPriorityTaskWoken);
        if(xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else
    {
        ret = xQueueSendToBack(m_queue, reinterpret_cast<void*>(&message), WORK_QUEUE_SEND_DELAY);
    }

    return ret == pdPASS;
}

template<std::size_t STACK_SIZE, std::size_t QUEUE_NB_ITEMS, osPriority PRIORITY>
bool WorkQueue<STACK_SIZE, QUEUE_NB_ITEMS, PRIORITY>::submit(Work& work)
{
    return submitData(work, etl::array_view<uint8_t>{});
}