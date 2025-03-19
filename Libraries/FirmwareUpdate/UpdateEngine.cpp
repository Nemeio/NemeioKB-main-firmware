/*
 * UpdateEngine.cpp
 *
 *  Created on: 30 janv. 2021
 *      Author: thomas
 */

#include "UpdateEngine.hpp"
#include "CompressedFileObject.hpp"
#include "StreamObject.hpp"
#include "TAPLog.hpp"
#include "ComponentsTesting.hpp"
#include "FirmwareUpdateTimeouts.hpp"
#include "EventLogFileDataTransport.hpp"

UpdateEngine::UpdateEngine(StreamDataReadTask& streamDataReadTask,
                           IFileDecompressor& decompressor,
                           FirmwareUpdateComponent STM32UpdateComponent,
                           FirmwareUpdateComponent ITEUpdateComponent,
                           FirmwareUpdateComponent NRFUpdateComponent,
                           ISignatureVerifier& signatureVerifier,
                           ISharedBufferContainer& sharedBufferContainer)
    : mSTM32UpdateComponent(STM32UpdateComponent)
    , mITEUpdateComponent(ITEUpdateComponent)
    , mNRFUpdateComponent(NRFUpdateComponent)
    , mDecompressor(decompressor)
    , mStreamDataReadTask(streamDataReadTask)
    , mSignatureVerifier(signatureVerifier)
    , mSharedBufferContainer(sharedBufferContainer)
{
}

FirmwareUpdateDefs::ErrCode UpdateEngine::run(ISystemUpdateFile& file,
                                              IUpdateListener& updateListener,
                                              IUpdateStatusListener& updateStatusListener)
{
    FirmwareUpdateDefs::ErrCode ret = FirmwareUpdateDefs::ErrCode::UNEXPECTED;

    updateListener.onUpdateEvent(UpdateStatus::START);

    VerifiedPackageFileSystem packageFileSystem(file.getFileSystem(),
                                                file.getFilePath(),
                                                mSignatureVerifier,
                                                mSharedBufferContainer);

    if(packageFileSystem.init() == fs::FS_ERR_OK && packageFileSystem.mount() == fs::FS_ERR_OK)
    {
        ret = FirmwareUpdateDefs::ErrCode::SUCCESS;
    }

#ifndef NO_ITE_UPDATE
    if(FirmwareUpdateDefs::ErrCode::SUCCESS == ret)
    {
        ret = decompressAndApplyUpdate(packageFileSystem,
                                       FIRMWARE_PACKAGE_ITE_PATH,
                                       mITEUpdateComponent,
                                       updateStatusListener);
#define ITE_NOT_WIRED
#ifdef ITE_NOT_WIRED // ITE flash is not connected on all keyboard
        ret = FirmwareUpdateDefs::ErrCode::SUCCESS;
#endif
    }
#endif

    if(FirmwareUpdateDefs::ErrCode::SUCCESS == ret)
    {
        ret = decompressAndApplyUpdate(packageFileSystem,
                                       FIRMWARE_PACKAGE_NRF_PATH,
                                       mNRFUpdateComponent,
                                       updateStatusListener);
    }

    if(FirmwareUpdateDefs::ErrCode::SUCCESS == ret)
    {
        ret = decompressAndApplyUpdate(packageFileSystem,
                                       FIRMWARE_PACKAGE_STM32_PATH,
                                       mSTM32UpdateComponent,
                                       updateStatusListener);
    }

    packageFileSystem.unmount();
    packageFileSystem.deinit();

    if(ret == FirmwareUpdateDefs::ErrCode::SUCCESS)
    {
        updateListener.onUpdateEvent(UpdateStatus::END);
    }
    else
    {
        updateListener.onUpdateEvent(UpdateStatus::ERROR);
    }

    return ret;
}

FirmwareUpdateDefs::ErrCode UpdateEngine::decompressAndApplyUpdate(
    fs::IFileSystem& fileSystem,
    std::string_view file,
    FirmwareUpdateComponent& component,
    IUpdateStatusListener& updateStatusListener)
{
    FirmwareUpdateDefs::ErrCode ret = FirmwareUpdateDefs::ErrCode::SUCCESS;
    fs::File FirmwareFile(fileSystem);
    std::size_t decompressionSize = 0;

    auto readInitCb = [&component, &decompressionSize](ISizeReader& reader)
    {
        bool lambdaRet = false;

        if(reader.getSize(decompressionSize))
        {
            lambdaRet = component.getUpdater().initUpdate(decompressionSize)
                                == FirmwareUpdateDefs::ErrCode::SUCCESS
                            ? true
                            : false;
        }

        return lambdaRet;
    };

    auto readStreamCb =
        [&component, &updateStatusListener, &decompressionSize](IStreamDataReader& reader)
    {
        bool success = false;

        size_t totalReceivedSize = 0;
        size_t receivedSize = 0;
        do
        {
            const uint8_t* pBuffer = reader.read(COPY_ARRAY_SIZE, receivedSize);
            if(receivedSize != 0)
            {
                success = (component.getUpdater().writeFirmware(pBuffer, receivedSize)
                           == FirmwareUpdateDefs::ErrCode::SUCCESS);
            }
            if(success)
            {
                totalReceivedSize += receivedSize;
                notifyProgress(updateStatusListener,
                               component.getModuleId(),
                               (totalReceivedSize) *100 / decompressionSize);
            }
        } while(receivedSize == COPY_ARRAY_SIZE && success);


        return decompressionSize == totalReceivedSize;
    };

    notifyProgress(updateStatusListener, component.getModuleId(), 0);

    CompressedFileObject<COPY_ARRAY_SIZE> compressedFileObject(mStreamDataReadTask,
                                                               mDecompressor,
                                                               fileSystem,
                                                               file,
                                                               mCopyArray);
    if(ret == FirmwareUpdateDefs::ErrCode::SUCCESS)
    {
        ret = (compressedFileObject.template read<
                   STREAM_BUFFER_ARRAY_SIZE,
                   STREAM_BUFFER_ARRAY_SIZE,
                   fwupdatetimeouts::FIRMWARE_PACKAGE_STREAM_TIMEOUT_MS>(mReadFromStreamBuffer,
                                                                         mStreamBufferStorage,
                                                                         COPY_ARRAY_SIZE,
                                                                         mSourceReadChunkBuffer,
                                                                         readInitCb,
                                                                         readStreamCb)
                   ? FirmwareUpdateDefs::ErrCode::SUCCESS
                   : FirmwareUpdateDefs::ErrCode::UNEXPECTED);
    }

    if(ret == FirmwareUpdateDefs::ErrCode::SUCCESS)
    {
        ret = component.getUpdater().runUpdate();
    }

    if(ret == FirmwareUpdateDefs::ErrCode::SUCCESS)
    {
        notifyProgress(updateStatusListener, component.getModuleId(), 100);
    }
    else
    {
        notifyError(updateStatusListener, component.getModuleId());
    }

    return ret;
}

void UpdateEngine::notifyProgress(IUpdateStatusListener& updateStatusListener,
                                  Identification::ModuleId moduleId,
                                  uint8_t pourcentage)
{
    UpdateStatusDefs::UpdateStatus
        status(moduleId,
               UpdateStatusDefs::UpdateStatusNotificationType::UPDATE_PROGRESS,
               pourcentage);
    updateStatusListener.notifyUpdateStatus(status);
}

void UpdateEngine::notifyError(IUpdateStatusListener& updateStatusListener,
                               Identification::ModuleId moduleId)
{
    UpdateStatusDefs::UpdateStatus
        status(moduleId,
               UpdateStatusDefs::UpdateStatusNotificationType::UPDATE_FAILED,
               FirmwareUpdateComponent::FirmwareUpdateError::UNKNOWN);
    updateStatusListener.notifyUpdateStatus(status);
}

bool UpdateEngine::formatPackage(fs::IFileSystem& fileSystem, std::string_view systemPackagePath)
{
    PackageFileSystem packageFileSystem(fileSystem, systemPackagePath);
    return (packageFileSystem.format() == fs::FSErrCode::FS_ERR_OK);
}

bool UpdateEngine::isExpectedFirmware(AssemblyHardware& hardwareAssembly,
                                      fs::IFileSystem& fileSystem,
                                      std::string_view systemPackagePath,
                                      Logger& logger)
{
    PackageFileSystem packageFileSystem(fileSystem, systemPackagePath);
    EventLogFileDataTransport eventLogFileDataTransport(logger);
    TAPLog tapLog(eventLogFileDataTransport);
    bool ret = false;

    if(packageFileSystem.init() == fs::FS_ERR_OK && packageFileSystem.mount() == fs::FS_ERR_OK)
    {
        benchmark::ComponentsTesting version(hardwareAssembly, tapLog, packageFileSystem);
        ret = version.run();
    }

    return ret;
}

bool UpdateEngine::isUpdateValid(AssemblyHardware& hardwareAssembly,
                                 fs::IFileSystem& fileSystem,
                                 std::string_view systemPackagePath,
                                 packageVersion& expectedNrfVersion,
                                 Logger& logger,
                                 ISignatureVerifier& signatureVerifier,
                                 ISharedBufferContainer& sharedBufferContainer)
{
    if(!isExpectedFirmware(hardwareAssembly, fileSystem, systemPackagePath, logger))
    {
        return false;
    }

    VerifiedPackageFileSystem packageFileSystem(fileSystem,
                                                systemPackagePath,
                                                signatureVerifier,
                                                sharedBufferContainer);
    bool ret = false;

    if(packageFileSystem.init() == fs::FS_ERR_OK && packageFileSystem.mount() == fs::FS_ERR_OK
       && packageFileSystem.getVersion(FIRMWARE_PACKAGE_NRF_PATH, expectedNrfVersion))
    {
        ret = true;
    }

    return ret;
}
