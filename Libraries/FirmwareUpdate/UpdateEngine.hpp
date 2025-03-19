/*
 * UpdateEngine.hpp
 *
 *  Created on: 30 janv. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_UPDATE_ENGINE_HPP_
#define NEMEIO_FIRMWAREUPDATE_UPDATE_ENGINE_HPP_

#include <string>
#include <array>
#include "IFirmwareUpdate.hpp"
#include "fs.hpp"
#include "VerifiedPackageFileSystem.hpp"
#include "StreamDataReadTask.hpp"
#include "Decompression/IFileDecompressor.hpp"
#include "FirmwareUpdateComponent.hpp"
#include "ISystemUpdateFile.hpp"
#include "AssemblyHardware.h"
#include "Logger.hpp"
#include "ISignatureVerifier.hpp"

class UpdateEngine
{
public:
    UpdateEngine(StreamDataReadTask& streamDataReadTask,
                 IFileDecompressor& decompressor,
                 FirmwareUpdateComponent STM32UpdateComponent,
                 FirmwareUpdateComponent ITEUpdateComponent,
                 FirmwareUpdateComponent NRFUpdateComponent,
                 ISignatureVerifier& signatureVerifier,
                 ISharedBufferContainer& sharedBufferContainer);
    virtual ~UpdateEngine() = default;

    FirmwareUpdateDefs::ErrCode run(ISystemUpdateFile& file,
                                    IUpdateListener& updateListener,
                                    IUpdateStatusListener& updateStatusListener);

    static bool formatPackage(fs::IFileSystem& fileSystem, std::string_view systemPackagePath);
    [[nodiscard]] static bool isExpectedFirmware(AssemblyHardware& hardwareAssembly,
                                                 fs::IFileSystem& fileSystem,
                                                 std::string_view systemPackagePath,
                                                 Logger& logger);
    [[nodiscard]] static bool isUpdateValid(AssemblyHardware& hardwareAssembly,
                                            fs::IFileSystem& fileSystem,
                                            std::string_view systemPackagePath,
                                            packageVersion& expectedNrfVersion,
                                            Logger& logger,
                                            ISignatureVerifier& signatureVerifier,
                                            ISharedBufferContainer& sharedBufferContainer);

private:
    static constexpr ssize_t COPY_ARRAY_SIZE = 256;
    static constexpr ssize_t STREAM_BUFFER_ARRAY_SIZE = COPY_ARRAY_SIZE * 2;

    FirmwareUpdateComponent mSTM32UpdateComponent;
    FirmwareUpdateComponent mITEUpdateComponent;
    FirmwareUpdateComponent mNRFUpdateComponent;

    IFileDecompressor& mDecompressor;
    StreamDataReadTask& mStreamDataReadTask;
    ISignatureVerifier& mSignatureVerifier;
    ISharedBufferContainer& mSharedBufferContainer;
    std::array<uint8_t, COPY_ARRAY_SIZE> mCopyArray;

    std::array<uint8_t, STREAM_BUFFER_ARRAY_SIZE> mReadFromStreamBuffer;
    std::array<uint8_t, STREAM_BUFFER_ARRAY_SIZE> mStreamBufferStorage;
    std::array<uint8_t, STREAM_BUFFER_ARRAY_SIZE> mSourceReadChunkBuffer;

    FirmwareUpdateDefs::ErrCode decompressAndApplyUpdate(fs::IFileSystem& filesystem,
                                                         std::string_view file,
                                                         FirmwareUpdateComponent& updater,
                                                         IUpdateStatusListener& updateStatusListener);

    static void notifyProgress(IUpdateStatusListener& updateStatusListener,
                               Identification::ModuleId moduleId,
                               uint8_t pourcentage);
    static void notifyError(IUpdateStatusListener& updateStatusListener,
                            Identification::ModuleId moduleId);
};

#endif /* NEMEIO_FIRMWAREUPDATE_UPDATE_ENGINE_HPP_ */
