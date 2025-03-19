/*
 * BrightnessManager.h
 *
 *  Manage frontLight brightness via keypad's buttons
 *
 *  Created on: Feb 27, 2018
 *      Author: amongatcheucheu
 */

#ifndef FRONTLIGHT_BRIGHTNESSMANAGER_BRIGHTNESSMANAGER_HPP_
#define FRONTLIGHT_BRIGHTNESSMANAGER_BRIGHTNESSMANAGER_HPP_

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "ILedController.hpp"
#include "IBatteryStateListener.hpp"
#include "IDemoListener.hpp"
#include "SystemTypes.hpp"

enum BRIGHTNESS_ACTION
{
    ACTION_PROGRESSIVE_CHANGE,
    ACTION_TURN_ON_FRONTLIGHT,
    ACTION_INCREASE_ACTIVE_BRIGHTNESS,
    ACTION_DECREASE_ACTIVE_BRIGHTNESS,
    ACTION_SET_BRIGHTNESS,
    ACTION_SET_BRIGHTNESS_STEP,
    ACTION_SET_POWER_MAX,
    ACTION_SET_POWER_INACTIVE
};

struct BrightnessIncDecParms
{
    int startBrightness;
    uint8_t targetBrightness;
    int step;
    int32_t delay;
};

typedef uint8_t TurnOnParms;

typedef uint8_t SetBrightnessParms;

typedef uint8_t SetBrightnessStepParms;

typedef uint8_t SetLedPowerParms;

struct BrightnessData
{
    BRIGHTNESS_ACTION action;
    union
    {
        BrightnessIncDecParms brightnessIncDecParms;
        TurnOnParms turnOnBrightness;
        SetBrightnessParms brightnessPercent;
        SetLedPowerParms ledPower;
    };
};

enum BrightnessManagerState
{
    ENLIGHTENED,
    UNENLIGHTENED,
    OFF,
    DEMO
};

class BrightnessManager
{
public:
    explicit BrightnessManager(ILedController& frontLight);
    virtual ~BrightnessManager() = default;

    /* The "maxPercentage" value will be applied to the parameters of setBrightnessPercentage or setBrightnessPercentage
	 *
	 * setReferenceMaxBrightness(80);
	 * setBrightnessPercentage(25); //The real applied pourcentage will be (25 * 80) / 100 = 20%
	 */
    void setReferenceMaxBrightness(uint8_t maxPercentage);

    void setBrightnessPercentage(uint8_t brightnessPercent);
    void setBrightnessPercentageProgressive(uint8_t brightnessPercent,
                                            uint8_t stepPercent,
                                            DelayMs delay = DelayMs(10));
    uint8_t getBrightnessPercentage() const;

private:
    static constexpr UBaseType_t BRIGHTNESS_TASK_PRIORITY = 3;
    static constexpr configSTACK_DEPTH_TYPE BRIGHTNESS_TASK_STACK_SIZE = configMINIMAL_STACK_SIZE;

    ILedController& mFrontLight;
    uint8_t mTargetBrightnessPercentage = 0;
    uint8_t mActiveBrightnessPercentage = 0;
    uint8_t mPowerMaxPercentage = 100;

    uint8_t applyReferenceMaxBrightness(uint8_t value);

    void execSetBrightness(uint8_t brightnessPercent);
    void execSetPowerMax(uint8_t value);

    QueueHandle_t mQueue = nullptr;
    TaskHandle_t mHandle = nullptr;

    static void vBrightnessTask(void* pvParameters);
};

#endif /* FRONTLIGHT_BRIGHTNESSMANAGER_BRIGHTNESSMANAGER_HPP_ */
