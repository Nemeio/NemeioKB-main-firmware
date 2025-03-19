/*
 * BleHid.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: Labo
 */

#include "BleHid.hpp"
#include "usbd_hid.h"
#include "BleHidReportDescriptorCmd.hpp"
#include "BleHidInputReportCmd.hpp"

BleHid::BleHid(IBleChip& bleChip)
    : AbstractHID()
    , mBleChip(bleChip)
{
}

BleHid::~BleHid()
{
    mOutputReportCmd.remove_observer(*this);
    mBleChip.unregisterBleCommandListener(mOutputReportCmd);
}

void BleHid::init()
{
    mOutputReportCmd.add_observer(*this);
    mBleChip.registerBleCommandListener(mOutputReportCmd);

    mBleChip.setHidReportMap(etl::array_view(gReportLengths, gMaxReportID),
                             etl::array_view(HID_KB_ReportDesc, sizeof(HID_KB_ReportDesc)));
}

void BleHid::sendReport(const uint8_t* reportBuf, size_t reportBufSize)
{
    if(reportBufSize < 1)
    {
        return;
    }

    uint8_t reportID = reportBuf[0];
    size_t reportSize = reportBufSize - 1;
    reportBuf++;

    BleHidInputReportCmd cmd;
    cmd.setReportId(reportID);
    cmd.setInputReport(etl::array_view(reportBuf, reportSize));

    mBleChip.sendCommand(BleReadyForBleTxCommandRef(etl::reference_wrapper(cmd)));
}

void BleHid::notification(const BleHidOutputReportEvent& event)
{
    receivedOutputReport(event.data.data(), event.data.size());
}