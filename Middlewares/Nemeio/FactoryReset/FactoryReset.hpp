/*
 * FactoryReset.hpp
 *
 *  Created on: Dec 6, 2020
 *      Author: eskoric
 */

#ifndef NEMEIO_FACTORYRESET_FACTORYRESET_HPP_
#define NEMEIO_FACTORYRESET_FACTORYRESET_HPP_

#include "IFactoryReset.hpp"
#include "IApplication.hpp"
#include "ISystem.hpp"
#include "IBleChip.hpp"
#include "Signal.hpp"
#include "BleFactoryResetCmdObserver.hpp"

class FactoryReset : public IFactoryReset, public BleFactoryResetCmdObserver
{
public:
    FactoryReset(IApplication& application, ISystem& system, IBleChip& bleChip);
    virtual ~FactoryReset() = default;

    void run(const std::function<void(ErrCode)>& actionBeforeSystemReset);
    void notification(FactoryResetEvent event) final;
    void notification(BleTxRequestTimeout event) final;

private:
    IApplication& mApplication;
    ISystem& mSystem;
    IBleChip& mBLEChip;
    Signal mSignal;
    BLEDefs::ErrCode mBleFactoryResetRet{BLEDefs::ErrCode::UNKNOWN};
};

#endif /* NEMEIO_FACTORYRESET_FACTORYRESET_HPP_ */
