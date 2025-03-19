/*
 * FileSystemAssembly.hpp
 *
 *  Created on: 25 mars 2021
 *      Author: thomas
 */

#ifndef FILESYSTEM_FILESYSTEMASSEMBLY_HPP_
#define FILESYSTEM_FILESYSTEMASSEMBLY_HPP_

#include <DummyIBlockDevice.hpp>
#include <FlashBlockDevice.hpp>
#include <IBlockDeviceStrategy.hpp>
#include <IFlashIO.hpp>
#include <array>
#include "cpp_helper.hpp"
#include "Partition.hpp"
#include "LittleFileSystem.hpp"
#include "RawFileSystem.hpp"
#include "IAssembly.h"
#include "IFileSystem.hpp"
#include "FileSystemTrimming.hpp"
#include "FileSystemPartitions.hpp"
#include "InternalFlashBlockDevice.hpp"
#include "etl/variant.h"
#include "Module.hpp"

class FileSystemAssembly : public IAssembly, public Module<Identification::ModuleId::FILESYSTEM>
{
public:
    FileSystemAssembly();
    virtual ~FileSystemAssembly() = default;

    COPYABLE_MOVABLE(FileSystemAssembly, delete);

    void init_Assembly(IFlashIO& flash,
                       IFlashIO& secureFlash,
                       IFlashIO& iteFlash,
                       IFlashInterface& internalFlash);
    void start_Assembly() final;

    [[nodiscard]] fscommon::IFileSystem& getPartitionFileSystem(FileSystemPartitionLabel part) const;
    [[nodiscard]] IBlockDevice& getPartitionBlockDevice(FileSystemPartitionLabel part);

    void format();
    void formatApplication();

private:
    static const size_t MAX_PARTITIONS = static_cast<size_t>(
        FileSystemPartitionLabel::MAX_PARTITION);

    struct PartitionsInfo
    {
        uint32_t magicNumber;
        uint32_t fsVersion;
    };

    etl::variant<DummyBlockDevice, FlashBlockDevice> mBlockDeviceVariant;
    etl::variant<DummyBlockDevice, FlashBlockDevice> mSecureBlockDeviceVariant;
    etl::variant<DummyBlockDevice, FlashBlockDevice> mIteBlockDeviceVariant;
    etl::variant<DummyBlockDevice, InternalFlashBlockDevice> mInternalFlashBlockDeviceVariant;

    BlockDeviceStrategy mBlockDevice;
    BlockDeviceStrategy mSecureBlockDevice;
    BlockDeviceStrategy mIteBlockDevice;
    BlockDeviceStrategy mInternalFlashBlockDevice;

    std::array<Partition, MAX_PARTITIONS> mPartitions;
    Partition mInternalFlashOtpPartition;

    littlefs::LittleFileSystem mMainFileSystem;
    rawfs::RawFileSystem
        mFirmwareEnvAFileSystem; /* Store information about active bank and update state */
    rawfs::RawFileSystem
        mFirmwareEnvBFileSystem; /* Store information about active bank and update state */
    rawfs::RawFileSystem mFirmwareUpdateBankAFileSystem;
    rawfs::RawFileSystem mFirmwareUpdateBankBFileSystem;

    const std::array<std::reference_wrapper<fscommon::IFileSystem>, MAX_PARTITIONS> mFileSystem;

    fscommon::FileSystemTrimming mFileSystemTriming;

    void mountFileSystem(fscommon::IFileSystem& fs);
    void formatFileSystem(fscommon::IFileSystem& fs);
};

#endif /* FILESYSTEM_FILESYSTEMASSEMBLY_HPP_ */
