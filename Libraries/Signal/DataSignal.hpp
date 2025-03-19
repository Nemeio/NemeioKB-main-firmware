/*
 * DataSignal.h
 *
 *  Created on: Feb 12, 2021
 *      Author: thomas
 */

#ifndef SIGNAL_DATASIGNAL_H_
#define SIGNAL_DATASIGNAL_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "FreeRTOSUtils.hpp"
#include "queue.h"
#include "task.h"
#include "diag/Trace.h"

static constexpr int32_t DEFAULT_SET_WAIT_DELAY_MS = 10;

template<typename T, size_t SIZE>
class DataSignal
{
public:
    DataSignal();
    virtual ~DataSignal() = default;

    bool waitData(DelayMs delay, T& data);
    void clear();
    void setFromISRData(T& data);
    void setData(const T& data);
    void setData(DelayMs delay, const T& data);

private:
    QueueHandle_t mXQueue;
    StaticQueue_t mXStaticQueue;
    uint8_t mXQueueStorageArea[SIZE * sizeof(T)];
};

template<typename T, size_t SIZE>
DataSignal<T, SIZE>::DataSignal()
{
    mXQueue = xQueueCreateStatic(SIZE, sizeof(T), mXQueueStorageArea, &mXStaticQueue);

    configASSERT(mXQueue);
}

template<typename T, size_t SIZE>
bool DataSignal<T, SIZE>::waitData(DelayMs delay, T& data)
{
    if(pdTRUE == xQueueReceive(mXQueue, &data, delayToSystemTicks(delay)))
    {
        return true;
    }
    return false;
}

template<typename T, size_t SIZE>
void DataSignal<T, SIZE>::setFromISRData(T& data)
{
    BaseType_t taskWoken = pdFALSE;
    xQueueSendFromISR(mXQueue, &data, &taskWoken);
    portEND_SWITCHING_ISR(taskWoken);
}

template<typename T, size_t SIZE>
void DataSignal<T, SIZE>::setData(DelayMs delay, const T& data)
{
    xQueueSendToBack(mXQueue, (void*) &data, delayToSystemTicks(delay));
}

template<typename T, size_t SIZE>
void DataSignal<T, SIZE>::setData(const T& data)
{
    return setData(DelayMs(DEFAULT_SET_WAIT_DELAY_MS), data);
}

template<typename T, size_t SIZE>
void DataSignal<T, SIZE>::clear()
{
    xQueueReset(mXQueue);
}


#endif /* SIGNAL_SIGNAL_H_ */
