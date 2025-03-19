/*
 * BrightnessManager.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: amongatcheucheu
 */

#include <brightnessManager/BrightnessManager.hpp>
#include "timing_macros.h"
#include "cmsis_os.h"
#include "assertTools.h"

BrightnessManager::BrightnessManager(ILedController& frontLight)
    : mFrontLight(frontLight)
{
    mQueue = xQueueCreate(5, sizeof(BrightnessData));
    xTaskCreate(vBrightnessTask,
                "BrightnessTask",
                BRIGHTNESS_TASK_STACK_SIZE,
                this,
                BRIGHTNESS_TASK_PRIORITY,
                &mHandle);
}

void BrightnessManager::setBrightnessPercentage(uint8_t brightnessPercent)
{
    BrightnessData data;
    data.action = ACTION_SET_BRIGHTNESS;
    data.brightnessPercent = brightnessPercent;

    if(xQueueSendToBack(mQueue, (void*) &data, (TickType_t) 10) != pdPASS)
    {
    }
}

void BrightnessManager::setBrightnessPercentageProgressive(uint8_t brightnessPercent,
                                                           uint8_t stepPercent,
                                                           DelayMs delay)
{
    BrightnessData data;
    data.brightnessIncDecParms.targetBrightness = brightnessPercent;
    data.brightnessIncDecParms.startBrightness = getBrightnessPercentage();
    data.brightnessIncDecParms.delay = delay.getMs();
    data.brightnessIncDecParms.step = stepPercent;
    data.action = ACTION_PROGRESSIVE_CHANGE;
    if(xQueueSendToBack(mQueue, (void*) &data, (TickType_t) 10) != pdPASS)
    {
    }
}

uint8_t BrightnessManager::getBrightnessPercentage() const
{
    return mTargetBrightnessPercentage;
}

void BrightnessManager::setReferenceMaxBrightness(uint8_t value)
{
    BrightnessData data;
    data.ledPower = value;
    data.action = ACTION_SET_POWER_MAX;
    if(xQueueSendToBack(mQueue, (void*) &data, (TickType_t) 10) != pdPASS)
    {
    }
}

void BrightnessManager::execSetPowerMax(uint8_t value)
{
    mPowerMaxPercentage = value;
}

uint8_t BrightnessManager::applyReferenceMaxBrightness(uint8_t value)
{
    return mPowerMaxPercentage * value / 100;
}

void BrightnessManager::vBrightnessTask(void* pvParameters)
{
    BrightnessManager* bm = (BrightnessManager*) pvParameters;
    BrightnessData data;
    TickType_t delay = portMAX_DELAY;
    int16_t b = 0; /* Must be signed in case of currentBrightness - step < 0 */
    uint16_t currentIteration = 1;
    for(;;)
    {
        TRACE_FUNC_ENTRY();
        if(pdPASS == xQueueReceive(bm->mQueue, &data, delay))
        {
            currentIteration = 1;
        }

        if(data.action != ACTION_PROGRESSIVE_CHANGE)
        {
            delay = portMAX_DELAY;
        }

        switch(data.action)
        {
        case ACTION_SET_BRIGHTNESS:
            bm->mTargetBrightnessPercentage = data.brightnessPercent;
            bm->execSetBrightness(bm->mTargetBrightnessPercentage);
            break;
        case ACTION_SET_POWER_MAX:
            bm->mTargetBrightnessPercentage = data.ledPower;
            bm->execSetPowerMax(bm->mTargetBrightnessPercentage);
            break;
        case ACTION_PROGRESSIVE_CHANGE:
            bm->mTargetBrightnessPercentage = data.brightnessIncDecParms.targetBrightness;
            delay = data.brightnessIncDecParms.delay;
            if(data.brightnessIncDecParms.startBrightness >= bm->mTargetBrightnessPercentage)
            {
                b = data.brightnessIncDecParms.startBrightness
                    - currentIteration * data.brightnessIncDecParms.step;
                if(b <= bm->mTargetBrightnessPercentage)
                {
                    b = bm->mTargetBrightnessPercentage;
                    delay = portMAX_DELAY;
                }
                bm->execSetBrightness(b);
            }
            else
            {
                b = data.brightnessIncDecParms.startBrightness
                    + currentIteration * data.brightnessIncDecParms.step;
                if(b >= bm->mTargetBrightnessPercentage)
                {
                    b = bm->mTargetBrightnessPercentage;
                    delay = portMAX_DELAY;
                }
                bm->execSetBrightness(b);
            }
            ++currentIteration;
            break;
        default:
            ASSERT(false);
            break;
        }

        TRACE_FUNC_EXIT();
    }
}

void BrightnessManager::execSetBrightness(uint8_t brightnessPercent)
{
    if(brightnessPercent <= 100)
    {
        mActiveBrightnessPercentage = brightnessPercent;
    }
    else
    {
        mActiveBrightnessPercentage = 100;
    }
    mFrontLight.setLightIntensity(applyReferenceMaxBrightness(mActiveBrightnessPercentage));
}
