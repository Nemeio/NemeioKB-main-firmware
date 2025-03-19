/*
 * Demo.hpp
 *
 *  Created on: Nov 27, 2018
 *      Author: Labo
 */

#ifndef DEMO_DEMO_HPP_
#define DEMO_DEMO_HPP_

#include "BrightnessManager.hpp"
#include "IConfigurationSwitcher.hpp"
#include "IDemoListener.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "IWorkQueue.hpp"
#include "iNemeioKeypadPressedListener.h"
#include <vector>

class Demo : public iNemeioKeypadPressedListener
{
public:
    Demo(BrightnessManager& brightnessManager,
         IConfigurationSwitcher& configurationSwitcher,
         IWorkQueue& workQueue);
    virtual ~Demo();

    void setDemoMode(bool bDemoMode);

    void setPowerMax(uint8_t powerMax);

    void registerListener(IDemoListener& demoListener);

private:
    static constexpr UBaseType_t DEMO_TASK_PRIORITY = 3;
    static constexpr configSTACK_DEPTH_TYPE DEMO_TASK_STACK_SIZE = 512;

    enum DemoMessage
    {
        DEMO_START,
        DEMO_STOP,
    };

    enum FrontlightDirection
    {
        FRONTLIGHT_INC,
        FRONTLIGHT_DEC
    };

    void start();
    void stop();
    void init();
    void end();
    void notifyStart();
    void notifyStop();
    void updateFrontlight();
    void updateConfigWork(etl::array_view<uint8_t> data);
    void checkUpdateConfig();

    void pressedKey(const ButtonID& button);
    void unpressedKey(const ButtonID& button);
    void terminateUpdateKeys();

    bool mDemoMode;
    BrightnessManager& mBrightnessManager;
    IConfigurationSwitcher& mConfigurationSwitcher;
    IWorkQueue& mWorkQueue;
    TickType_t mDelay;
    uint32_t mTElapsed;
    uint8_t mRefreshCnt;
    FrontlightDirection mFrontlightDir;
    float mBrightness;
    float mStep;
    uint8_t mBrightnessMax;
    Work mUpdateConfigWork;

    std::vector<IDemoListener*> mDemoListeners;

    QueueHandle_t mQueue;
    TaskHandle_t mHandle;

    static void vDemoTask(void* pvParameters);
};

#endif /* DEMO_DEMO_HPP_ */
