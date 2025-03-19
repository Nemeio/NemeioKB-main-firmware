/*
 * STM32PWM.cpp
 *
 *  Created on: Aug 2, 2018
 *      Author: Labo
 */

#include "STM32PWM.hpp"
#include "AutoLock.h"
#include "STM32HWTimerUtil.hpp"

STM32PWM::STM32PWM(TIM_TypeDef* instance, uint32_t channel)
    : mHTIM({})
    , mInstance(instance)
    , mChannel(channel)
    , mPeriod(0)
{
}

STM32PWM::~STM32PWM()
{
    // TODO Auto-generated destructor stub
}

void STM32PWM::init(uint32_t freqHz)
{
    AutoLock autoLock(mPWMMutex);

    mPeriod = (100000 / freqHz);

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    uint32_t apbFreq = STM32HWTimerUtil::getTimerSourceClockFreq(mInstance);

    mHTIM.Instance = mInstance;
    mHTIM.Init.Prescaler = (apbFreq / 100000) - 1;
    mHTIM.Init.CounterMode = TIM_COUNTERMODE_UP;
    mHTIM.Init.Period = mPeriod - 1;
    mHTIM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    mHTIM.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if(HAL_TIM_Base_Init(&mHTIM) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if(HAL_TIM_ConfigClockSource(&mHTIM, &sClockSourceConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if(HAL_TIM_PWM_Init(&mHTIM) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if(HAL_TIMEx_MasterConfigSynchronization(&mHTIM, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if(HAL_TIM_PWM_ConfigChannel(&mHTIM, &sConfigOC, mChannel) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    HAL_TIM_MspPostInit(&mHTIM);

    HAL_TIM_PWM_Start(&mHTIM, mChannel);
}

void STM32PWM::setDutyCycle(uint8_t dutyCycle)
{
    AutoLock autoLock(mPWMMutex);

    TIM_OC_InitTypeDef sConfigOC = {0};

    uint32_t pulse = ((mPeriod * dutyCycle) / 100);

    __HAL_TIM_SET_COMPARE(&mHTIM, mChannel, pulse);
}
