/*
 * FirmwareUpdateBuilder.hpp
 *
 *  Created on: Feb 10, 2020
 *      Author: Labo
 */

#ifndef FIRMWAREUPDATE_FIRMWAREUPDATEASSEMBLY_HPP_
#define FIRMWAREUPDATE_FIRMWAREUPDATEASSEMBLY_HPP_

#include <IBlockDevice.hpp>
#include <string>
#include "STM32FirmwareUpdate.hpp"
#include "STM32FirmwareUpdateRollback.hpp"
#include "IT8951FirmwareUpdate.hpp"
#include "NRFFirmwareUpdate.hpp"
#include "AbstractWatchdog.hpp"
#include "IFlashInterface.hpp"
#include "fs.hpp"
#include "IAppCommTxListener/IAppCommTxListener.hpp"
#include "IUpdateStatusListener.hpp"
#include "SystemUpdateFile.hpp"
#include "SystemUpdate.hpp"
#include "UpdateEngine.hpp"
#include "ISFU.hpp"
#include "ISignatureVerifier.hpp"
#include "ISharedBufferContainer.hpp"

class FirmwareUpdateAssembly
{
public:
    FirmwareUpdateAssembly(IFlashInterface& flashInterface,
                           ISFU& sfu,
                           AssemblyHardware& assembly,
                           StreamDataReadTask& streamDataReadTask,
                           IFileDecompressor& decompressor,
                           fs::IFileSystem& updateFileSystem,
                           std::string_view updateFilePath,
                           fs::IFileSystem& rollbackFileSystem,
                           std::string_view rollbackFilePath,
                           IBlockDevice& iteBlockDevice,
                           ISignatureVerifier& signatureVerifier,
                           ISharedBufferContainer& sharedBufferContainer);
    virtual ~FirmwareUpdateAssembly() = default;

    [[nodiscard]] SystemUpdate& getSystemUpdateItem();
    [[nodiscard]] SystemUpdate& getSystemRollbackItem();

    [[nodiscard]] const SystemUpdateFile& getSystemUpdateFile() const;
    [[nodiscard]] const SystemUpdateFile& getSystemRollbackFile() const;

private:
    ISignatureVerifier& mSignatureVerifier;
    STM32FirmwareUpdate mSTM32FirmwareUpdate;
    STM32FirmwareUpdateRollback mSTM32FirmwareUpdateRollback;
    IT8951FirmwareUpdate mITEFirmwareUpdate;
    NRFFirmwareUpdate mNRFFirmwareUpdate;
    UpdateEngine mUpdateEngine;
    UpdateEngine mUpdateEngineRollback;
    SystemUpdateFile mSystemUpdateFile;
    SystemUpdateFile mSystemRollbackFile;
    SystemUpdate mSystemUpdate;
    SystemUpdate mSystemRollback;
};

#endif /* FIRMWAREUPDATE_FIRMWAREUPDATEASSEMBLY_HPP_ */
