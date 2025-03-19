/*
 * FactoryReset.cpp
 *
 *  Created on: Dec 6, 2020
 *      Author: eskoric
 */

#include "FactoryReset.hpp"
#include "BleFactoryResetCmd.hpp"

FactoryReset::FactoryReset(IApplication& application, ISystem& system, IBleChip& bleChip)
    : mApplication(application)
    , mSystem(system)
    , mBLEChip(bleChip)
{
}

void FactoryReset::run(const std::function<void(ErrCode)>& actionBeforeSystemReset)
{
    ErrCode ret = ErrCode::SUCCESS;
    mApplication.factoryReset();

    mSignal.clear();

    BleFactoryResetCmd bleFactoryResetCmd;

    bleFactoryResetCmd.add_observer(*this);

    if(BLEDefs::ErrCode bleReqRet = mBLEChip.sendRequest(etl::reference_wrapper(bleFactoryResetCmd));
       bleReqRet == BLEDefs::ErrCode::SUCCESS)
    {
        mSignal.wait(DelayMs::INFINITE_DELAY);
    }

    if(mBleFactoryResetRet != BLEDefs::ErrCode::SUCCESS)
    {
        ret = ErrCode::BLE;
    }

    if(actionBeforeSystemReset)
    {
        actionBeforeSystemReset(ret);
    }

    mSystem.reset();
}

void FactoryReset::notification(FactoryResetEvent event)
{
    mBleFactoryResetRet = event.err;
    mSignal.set(0);
}

void FactoryReset::notification(BleTxRequestTimeout event)
{
    mBleFactoryResetRet = BLEDefs::ErrCode::RESP_TIMEOUT;
    mSignal.set(0);
}