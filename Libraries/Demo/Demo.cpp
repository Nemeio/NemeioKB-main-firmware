/*
 * Demo.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: Labo
 */

#include "Demo.hpp"

const TickType_t DEMO_SWITCH_PERIOD_MS = 9000;
const uint32_t DEMO_FRONTLIGHT_PERIOD_MS = 3000;
const TickType_t DEMO_UPDATE_FRONTLIGHT_DELAY_MS = 10;
const ButtonID BUTTON_STOPDEMO = BTN_ESCAPE;

Demo::Demo(BrightnessManager& brightnessManager,
           IConfigurationSwitcher& configurationSwitcher,
           IWorkQueue& workQueue)
    : mDemoMode(false)
    , mBrightnessManager(brightnessManager)
    , mConfigurationSwitcher(configurationSwitcher)
    , mWorkQueue(workQueue)
    , mDelay(portMAX_DELAY)
    , mTElapsed(0)
    , mRefreshCnt(0)
    , mFrontlightDir(FRONTLIGHT_DEC)
    , mBrightness(0)
    , mStep(0)
    , mBrightnessMax(100)
    , mUpdateConfigWork([this](etl::array_view<uint8_t> data) { updateConfigWork(data); })
{
    mHandle = NULL;
    mQueue = xQueueCreate(2, sizeof(DemoMessage));
    xTaskCreate(vDemoTask, "DemoTask", DEMO_TASK_STACK_SIZE, this, DEMO_TASK_PRIORITY, &mHandle);
}

Demo::~Demo()
{
}

void Demo::start()
{
    DemoMessage msg = DEMO_START;
    xQueueSendToBack(mQueue, (void*) &msg, 10);
}

void Demo::stop()
{
    DemoMessage msg = DEMO_STOP;
    xQueueSendToBack(mQueue, (void*) &msg, 10);
}

void Demo::init()
{
    mDelay = DEMO_UPDATE_FRONTLIGHT_DELAY_MS;
    mTElapsed = 0;
    mRefreshCnt = 0;
    mFrontlightDir = FRONTLIGHT_DEC;
    mBrightness = mBrightnessMax;
    mStep = (float) mBrightnessMax / (float) DEMO_FRONTLIGHT_PERIOD_MS
            * DEMO_UPDATE_FRONTLIGHT_DELAY_MS;

    notifyStart();
}

void Demo::end()
{
    mDelay = portMAX_DELAY;
    notifyStop();
}

void Demo::setPowerMax(uint8_t powerMax)
{
    mBrightnessMax = powerMax;
}

void Demo::registerListener(IDemoListener& demoListener)
{
    mDemoListeners.push_back(&demoListener);
}

void Demo::notifyStart()
{
    for(auto ite = mDemoListeners.begin(); ite != mDemoListeners.end(); ite++)
    {
        if(*ite != NULL)
        {
            (*ite)->demoStart();
        }
    }
}

void Demo::notifyStop()
{
    for(auto ite = mDemoListeners.begin(); ite != mDemoListeners.end(); ite++)
    {
        if(*ite != NULL)
        {
            (*ite)->demoStop();
        }
    }
}

void Demo::updateFrontlight()
{
    float newBrightness = 0;

    if(mFrontlightDir == FRONTLIGHT_INC)
    {
        newBrightness = mBrightness + mStep;
        if(newBrightness > mBrightnessMax)
        {
            newBrightness = mBrightnessMax;
        }
    }
    else if(mFrontlightDir == FRONTLIGHT_DEC)
    {
        newBrightness = mBrightness - mStep;
        if(newBrightness < 0)
        {
            newBrightness = 0;
        }
    }

    if((uint8_t) newBrightness != (uint8_t) mBrightness)
    {
        mBrightnessManager.setBrightnessPercentage((uint8_t) newBrightness);
    }
    mBrightness = newBrightness;

    if(mFrontlightDir == FRONTLIGHT_INC && mBrightness >= mBrightnessMax)
    {
        mFrontlightDir = FRONTLIGHT_DEC;
    }
    else if(mFrontlightDir == FRONTLIGHT_DEC && mBrightness == 0)
    {
        mFrontlightDir = FRONTLIGHT_INC;
    }
}

void Demo::updateConfigWork(etl::array_view<uint8_t>)
{
    mConfigurationSwitcher.setNextConfiguration();
}

void Demo::checkUpdateConfig()
{
    mTElapsed += mDelay;

    if(mTElapsed == DEMO_SWITCH_PERIOD_MS)
    {
        mTElapsed = 0;
        mWorkQueue.submit(mUpdateConfigWork);
    }
}

void Demo::pressedKey(const ButtonID& button)
{
    if(mDemoMode && button.x == BUTTON_STOPDEMO.x && button.y == BUTTON_STOPDEMO.y)
    {
        mDemoMode = false;
        stop();
    }
}

void Demo::unpressedKey(const ButtonID& button)
{
}

void Demo::terminateUpdateKeys()
{
}

void Demo::setDemoMode(bool bDemoMode)
{
    if(mDemoMode != bDemoMode)
    {
        mDemoMode = bDemoMode;
        if(mDemoMode)
        {
            start();
        }
        else
        {
            stop();
        }
    }
}

void Demo::vDemoTask(void* pvParameters)
{
    Demo* pDemo = (Demo*) pvParameters;
    DemoMessage msg;

    for(;;)
    {
        if(pdPASS == xQueueReceive(pDemo->mQueue, &msg, pDemo->mDelay))
        {
            switch(msg)
            {
            case DEMO_START:
                pDemo->init();
                break;
            case DEMO_STOP:
                pDemo->end();
                break;
            }
        }
        else
        {
            pDemo->updateFrontlight();
            pDemo->checkUpdateConfig();
        }
    }
}
