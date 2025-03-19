/*
 * VersionsCmd.hpp
 *
 *  Created on: May 20, 2019
 *      Author: Labo
 */

#ifndef APPCOMM_VERSIONS_VERSIONSCMD_HPP_
#define APPCOMM_VERSIONS_VERSIONSCMD_HPP_

#include "utils.hpp"
#include "AppCommCmd.hpp"
#include "Version.h"
#include "IBleChip.hpp"
#include "BleFwVersionObserver.hpp"
#include "IScreenVersionsReader.hpp"
#include "Signal.hpp"

class VersionsCmd : public AppCommCmd, public BleFwVersionObserver
{
public:
    VersionsCmd(IAppCommTxListener& appCommTxListener,
                IBleChip& mBLEChip,
                IScreenVersionsReader& screenVersionsReader);
    virtual ~VersionsCmd() = default;

    void dataReceived(const uint8_t* pData, size_t dataLen);
    void notification(const BleFwVersionEvent& event) final;
    void notification(BleTxRequestTimeout event) final;

private:
    PACK((struct VersionList
          {
              AppCommDefs::AppCommErrCode errCodeCPUVersion = AppCommDefs::APPCOMMERR_UNEXPECTED;
              char cpuVersion[VERSION_MAX_SZ];

              AppCommDefs::AppCommErrCode errCodeBLEVersion = AppCommDefs::APPCOMMERR_UNEXPECTED;
              char bleVersion[VERSION_MAX_SZ];

              AppCommDefs::AppCommErrCode errCodeDisplayChipVersions =
                  AppCommDefs::APPCOMMERR_UNEXPECTED;
              char displayChipFirmwareVersion[VERSION_MAX_SZ];
              char displayChipLUTVersion[VERSION_MAX_SZ];

              VersionList()
              {
                  memset(cpuVersion, 0, sizeof(cpuVersion));
                  memset(bleVersion, 0, sizeof(bleVersion));
                  memset(displayChipFirmwareVersion, 0, sizeof(displayChipFirmwareVersion));
                  memset(displayChipLUTVersion, 0, sizeof(displayChipLUTVersion));
              };
          }));

    IBleChip& mBLEChip;
    IScreenVersionsReader& mScreenVersionsReader;
    Signal mSignal;

    VersionList mComponentVersions;

    void getCPUVersion(VersionList& versionList);
    void getDisplayChipVersions(VersionList& versionList);
    void transmitVersionList(const VersionList& versionList);
};

#endif /* APPCOMM_VERSIONS_VERSIONSCMD_HPP_ */
