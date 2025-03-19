/*
 * FirmwareUpdateBuilder.cpp
 *
 *  Created on: Feb 10, 2020
 *      Author: Labo
 */

#include "FirmwareUpdateAssembly.hpp"
#include "FirmwareUpdateComponent.hpp"

FirmwareUpdateAssembly::FirmwareUpdateAssembly(IFlashInterface& flashInterface,
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
                                               ISharedBufferContainer& sharedBufferContainer)
    : mSignatureVerifier(signatureVerifier)
    , mSTM32FirmwareUpdate(flashInterface, sfu)
    , mITEFirmwareUpdate(iteBlockDevice)
    , mNRFFirmwareUpdate(assembly.getBleItem())
    , mUpdateEngine(streamDataReadTask,
                    decompressor,
                    FirmwareUpdateComponent(Identification::ModuleId::SYSTEM, mSTM32FirmwareUpdate),
                    FirmwareUpdateComponent(Identification::ModuleId::DISPLAY, mITEFirmwareUpdate),
                    FirmwareUpdateComponent(Identification::ModuleId::BLE, mNRFFirmwareUpdate),
                    mSignatureVerifier,
                    sharedBufferContainer)
    , mUpdateEngineRollback(streamDataReadTask,
                            decompressor,
                            FirmwareUpdateComponent(Identification::ModuleId::SYSTEM,
                                                    mSTM32FirmwareUpdateRollback),
                            FirmwareUpdateComponent(Identification::ModuleId::DISPLAY,
                                                    mITEFirmwareUpdate),
                            FirmwareUpdateComponent(Identification::ModuleId::BLE,
                                                    mNRFFirmwareUpdate),
                            mSignatureVerifier,
                            sharedBufferContainer)
    , mSystemUpdateFile(updateFileSystem, updateFilePath)
    , mSystemRollbackFile(rollbackFileSystem, rollbackFilePath)
    , mSystemUpdate(mUpdateEngine, mSystemUpdateFile)
    , mSystemRollback(mUpdateEngineRollback, mSystemRollbackFile)
{
}

SystemUpdate& FirmwareUpdateAssembly::getSystemUpdateItem()
{
    return mSystemUpdate;
}

SystemUpdate& FirmwareUpdateAssembly::getSystemRollbackItem()
{
    return mSystemRollback;
}

const SystemUpdateFile& FirmwareUpdateAssembly::getSystemUpdateFile() const
{
    return mSystemUpdateFile;
}

const SystemUpdateFile& FirmwareUpdateAssembly::getSystemRollbackFile() const
{
    return mSystemRollbackFile;
}