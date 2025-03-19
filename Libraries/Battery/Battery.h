/*
 * Battery.h
 *
 *  Created on: May 17, 2018
 *      Author: amongatcheucheu
 */

#ifndef BATTERY_BATTERY_H_
#define BATTERY_BATTERY_H_

#include "IBattery.hpp"
#include "IBatteryCharger.h"
#include "IFuelGauge.h"
#include "IBatteryLevelListener.hpp"
#include "RecursiveMutex.hpp"

#include <vector>

#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"
#include "task.h"
#include "fs.hpp"


class Battery : public IBattery
{
public:
    Battery(IFuelGauge& fuelGauge, fs::IFileSystem& fs);
    virtual ~Battery() = default;

    void init();

    RetCode getBatteryInformation(BatteryInformation& batteryInformation);

    void startBatteryMeasurement();
    void stopBatteryMeasurement();
    void registerBatteryLevelListener(IBatteryLevelListener& listener);
    void unregisterBatteryLevelListener(IBatteryLevelListener& listener);

private:
    static constexpr UBaseType_t BATTERY_MEASUREMENT_TASK_PRIORITY = 3;
    static constexpr configSTACK_DEPTH_TYPE BATTERY_MEASUREMENT_TASK_STACK_SIZE = 300;
    static constexpr size_t BATTERY_QUEUE_SIZE = 3;
    static constexpr char FUEL_GAUGE_FILE_NAME[] = "MAX17055Parameters";

    void updateBatteryInformation();
    void notifyBatteryLevel(uint8_t batteryLevel);
    void requestUpdateBatteryLevel();
    static void batteryLevelTimerCallback(TimerHandle_t xTimer);

    IFuelGauge& mFuelGauge;
    RecursiveMutex mMutex;
    fs::File mFuelGaugeParametersFile;

    BatteryInformation mBatteryInformation;
    RetCode mbBatteryInformationRet = RetCode::INFORMATION_NOT_READY;

    std::vector<IBatteryLevelListener*> mBatteryLevelListenerList;
    TimerHandle_t mBatteryLevelTimer;

    TaskHandle_t mBatteryMeasurementTaskHandle;
    QueueHandle_t mBatteryMeasurementQueue;
    static void vBatteryMeasurementTask(void* pvParameters);
};

#endif /* BATTERY_BATTERY_H_ */
