/*
 * BleHid.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Labo
 */

#ifndef BLE_BLEHID_HPP_
#define BLE_BLEHID_HPP_

#include "AbstractHID.h"
#include "IBleChip.hpp"
#include "BleHidOutputReportCmd.hpp"
#include "cpp_helper.hpp"

class BleHid : public AbstractHID, public BleHidOutputReportCmdObserver
{
public:
    explicit BleHid(IBleChip& bleChip);
    virtual ~BleHid();
    COPYABLE_MOVABLE(BleHid, delete);

    void init();
    void notification(const BleHidOutputReportEvent& event) final;

private:
    IBleChip& mBleChip;
    BleHidOutputReportCmd mOutputReportCmd;

    void sendReport(const uint8_t* reportBuf, size_t reportBufSize);
};

#endif /* BLE_BLEHID_HPP_ */
