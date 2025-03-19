/*
 * Battery.cpp
 *
 *  Created on: May 17, 2018
 *      Author: amongatcheucheu
 */

#include "Battery.h"

#include <algorithm>
#include "assertTools.h"
#include "AutoLock.h"

Battery::Battery(IFuelGauge& fuelGauge, fs::IFileSystem& fs)
    : mFuelGauge(fuelGauge)
    , mFuelGaugeParametersFile(fs)
{ /* File should not be open yet, since FileSystem is not started yet */

    mBatteryMeasurementQueue = xQueueCreate(BATTERY_QUEUE_SIZE, sizeof(uint8_t));
    mBatteryLevelTimer = xTimerCreate("batteryLevelTimer",
                                      10000,
                                      pdTRUE,
                                      this,
                                      batteryLevelTimerCallback);
}

Battery::RetCode Battery::getBatteryInformation(BatteryInformation& batteryInformation)
{
    AutoLock autolock(mMutex);

    if(RetCode::SUCCESS == mbBatteryInformationRet)
    {
        batteryInformation = mBatteryInformation;
    }

    return mbBatteryInformationRet;
}

void Battery::startBatteryMeasurement()
{
    requestUpdateBatteryLevel();
    xTimerStart(mBatteryLevelTimer, 0);
}

void Battery::stopBatteryMeasurement()
{
    xTimerStop(this->mBatteryLevelTimer, 0);
}

void Battery::registerBatteryLevelListener(IBatteryLevelListener& listener)
{
    std::vector<IBatteryLevelListener*>::iterator ite = std::find(mBatteryLevelListenerList.begin(),
                                                                  mBatteryLevelListenerList.end(),
                                                                  &listener);
    if(mBatteryLevelListenerList.end() == ite)
    {
        mBatteryLevelListenerList.push_back(&listener);
    }
}

void Battery::unregisterBatteryLevelListener(IBatteryLevelListener& observer)
{
    std::vector<IBatteryLevelListener*>::iterator ite = std::find(mBatteryLevelListenerList.begin(),
                                                                  mBatteryLevelListenerList.end(),
                                                                  &observer);
    if(mBatteryLevelListenerList.end() != ite)
    {
        mBatteryLevelListenerList.erase(ite);
    }
}

void Battery::notifyBatteryLevel(uint8_t batteryLevel)
{
    for(std::vector<IBatteryLevelListener*>::iterator ite = mBatteryLevelListenerList.begin();
        ite != mBatteryLevelListenerList.end();
        ++ite)
    {
        (*ite)->batteryLevel(mBatteryInformation.stateOfChargePercent);
    }
}

void Battery::updateBatteryInformation()
{
    uint8_t batteryLevel = 0;

    {
        AutoLock autolock(mMutex);

        mbBatteryInformationRet = mFuelGauge.getBatteryInformation(mBatteryInformation)
                                      ? RetCode::SUCCESS
                                      : RetCode::FUELGAUGE_ERROR;

        batteryLevel = mBatteryInformation.stateOfChargePercent;
    }

    if(RetCode::SUCCESS == mbBatteryInformationRet)
    {
        notifyBatteryLevel(batteryLevel);
    }
}

void Battery::batteryLevelTimerCallback(TimerHandle_t xTimer)
{
    Battery* battery = (Battery*) pvTimerGetTimerID(xTimer);

    battery->requestUpdateBatteryLevel();
}

void Battery::requestUpdateBatteryLevel()
{
    uint8_t data = 0;
    xQueueSendToBack(mBatteryMeasurementQueue, (void*) &data, (TickType_t) 0);
}

void Battery::vBatteryMeasurementTask(void* pvParameters)
{
    ASSERT(pvParameters != nullptr);

    Battery& batt = *(reinterpret_cast<Battery*>(pvParameters));
    TickType_t delay = portMAX_DELAY;
    uint8_t data;

    for(;;)
    {
        if(pdPASS == xQueueReceive(batt.mBatteryMeasurementQueue, &data, delay))
        {
            batt.updateBatteryInformation();
            batt.mFuelGauge.updateLearnedParameters(batt.mFuelGaugeParametersFile);
        }
    }
}

void Battery::init()
{
    ASSERT(pdPASS
           == xTaskCreate(vBatteryMeasurementTask,
                          "BatteryMeasurementTask",
                          BATTERY_MEASUREMENT_TASK_STACK_SIZE,
                          this,
                          BATTERY_MEASUREMENT_TASK_PRIORITY,
                          &mBatteryMeasurementTaskHandle));

    mFuelGaugeParametersFile.open(FUEL_GAUGE_FILE_NAME, fs::OpenFlag::CREAT | fs::OpenFlag::RDWR);
    mFuelGauge.configure(
        mFuelGaugeParametersFile); /* Even if we can't open the file, we don't assert, it's not a critical part */
}
