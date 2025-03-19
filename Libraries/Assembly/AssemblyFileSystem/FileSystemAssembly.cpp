/*
 * FileSystemAssembly.cpp
 *
 *  Created on: 25 mars 2021
 *      Author: thomas
 */

#include <cstddef>
#include <cstdint>
#include "FileSystemAssembly.hpp"
#include "assertTools.h"
#include "utils.hpp"
#include "InternalFlashAddr.hpp"
#include "QSPIMapping.hpp"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "fsasm"

FileSystemAssembly::FileSystemAssembly()
    : mBlockDevice(etl::get<DummyBlockDevice>(mBlockDeviceVariant))
    , mSecureBlockDevice(etl::get<DummyBlockDevice>(mBlockDeviceVariant))
    , mIteBlockDevice(etl::get<DummyBlockDevice>(mBlockDeviceVariant))
    , mInternalFlashBlockDevice(etl::get<DummyBlockDevice>(mInternalFlashBlockDeviceVariant))
    , mPartitions{Partition(mBlockDevice, PARTITION_INFORMATION(MAIN)),
                  Partition(mBlockDevice, PARTITION_INFORMATION(FIRMWARE_ENV_BANK_A)),
                  Partition(mBlockDevice, PARTITION_INFORMATION(FIRMWARE_ENV_BANK_B)),
                  Partition(mBlockDevice, PARTITION_INFORMATION(FIRMWARE_UPDATE_BANK_A)),
                  Partition(mBlockDevice, PARTITION_INFORMATION(FIRMWARE_UPDATE_BANK_B))}
    , mInternalFlashOtpPartition(mInternalFlashBlockDevice,
                                 internal_flash_addr::OTP_AREA_START,
                                 internal_flash_addr::OTP_AREA_SIZE_BYTES)
    , mMainFileSystem(mFileSystemTriming,
                      mPartitions[static_cast<size_t>(FileSystemPartitionLabel::MAIN)])
    , mFirmwareEnvAFileSystem(
          mPartitions[static_cast<size_t>(FileSystemPartitionLabel::FIRMWARE_ENV_BANK_A)])
    , mFirmwareEnvBFileSystem(
          mPartitions[static_cast<size_t>(FileSystemPartitionLabel::FIRMWARE_ENV_BANK_B)])
    , mFirmwareUpdateBankAFileSystem(
          mPartitions[static_cast<size_t>(FileSystemPartitionLabel::FIRMWARE_UPDATE_BANK_A)])
    , mFirmwareUpdateBankBFileSystem(
          mPartitions[static_cast<size_t>(FileSystemPartitionLabel::FIRMWARE_UPDATE_BANK_B)])
    , mFileSystem{mMainFileSystem,
                  mFirmwareEnvAFileSystem,
                  mFirmwareEnvBFileSystem,
                  mFirmwareUpdateBankAFileSystem,
                  mFirmwareUpdateBankBFileSystem}
    , mFileSystemTriming(std::array<std::reference_wrapper<fscommon::IFileSystem>,
                                    fscommon::MAX_NUMBER_TRIMMING_FILESYSTEM>{mMainFileSystem})
{
}

void FileSystemAssembly::init_Assembly(IFlashIO& flash,
                                       IFlashIO& secureFlash,
                                       IFlashIO& iteFlash,
                                       IFlashInterface& internalFlash)
{
    ASSERT(flash.getFlashInfo().mTotalSize == QSPI_FLASH_SIZE);
    ASSERT(flash.getFlashInfo().mEraseSize == MINIMUM_PARTITION_SIZE);

    mBlockDeviceVariant.emplace<FlashBlockDevice>(flash);
    mSecureBlockDeviceVariant.emplace<FlashBlockDevice>(secureFlash);
    mIteBlockDeviceVariant.emplace<FlashBlockDevice>(iteFlash);
    mInternalFlashBlockDeviceVariant.emplace<InternalFlashBlockDevice>(internalFlash);

    mBlockDevice.setBlockDevice(etl::get<FlashBlockDevice>(mBlockDeviceVariant));
    mSecureBlockDevice.setBlockDevice(etl::get<FlashBlockDevice>(mSecureBlockDeviceVariant));
    mIteBlockDevice.setBlockDevice(etl::get<FlashBlockDevice>(mIteBlockDeviceVariant));
    mInternalFlashBlockDevice.setBlockDevice(
        etl::get<InternalFlashBlockDevice>(mInternalFlashBlockDeviceVariant));

    mBlockDevice.init();
    mSecureBlockDevice.init();
    /* mIteBlockDevice must not be initialized when ITE is on */

    for(auto& partition: mPartitions)
    {
        partition.init(); /* Validate block device after setting strategy */
    }
}

void FileSystemAssembly::start_Assembly()
{
    /* No need to mount/format update Bank */
    mountFileSystem(mMainFileSystem);

    mFileSystemTriming.init();
}

void FileSystemAssembly::mountFileSystem(fscommon::IFileSystem& fs)
{
    ASSERT(fs.init() == fscommon::FS_ERR_OK);
    if(!(fs.mount() == fscommon::FS_ERR_OK))
    {
        LOG_WARN(MODULE, "Could not mount fs " << etl::hex << etl::showbase << &fs << ", format");
        formatFileSystem(fs);
        ASSERT(fs.mount() == fscommon::FS_ERR_OK);
    }

    LOG_INFO(MODULE, "fs " << etl::hex << etl::showbase << &fs << " mounted");
}

void FileSystemAssembly::formatFileSystem(fscommon::IFileSystem& fs)
{
    fs.init();
    fs.unmount();
    fs.format();
}

void FileSystemAssembly::format()
{
    for(const auto& fs: mFileSystem)
    {
        formatFileSystem(fs);
    }
}

void FileSystemAssembly::formatApplication()
{
    formatFileSystem(mMainFileSystem);
}

fscommon::IFileSystem& FileSystemAssembly::getPartitionFileSystem(FileSystemPartitionLabel part) const
{
    ASSERT(static_cast<size_t>(part) < MAX_PARTITIONS);
    return mFileSystem[static_cast<size_t>(part)];
}

IBlockDevice& FileSystemAssembly::getPartitionBlockDevice(FileSystemPartitionLabel part)
{
    switch(part)
    {
    case FileSystemPartitionLabel::SECURE_PARTITION:
        return mSecureBlockDevice;
        break;
    case FileSystemPartitionLabel::ITE_PARTITION:
        return mIteBlockDevice;
        break;
    case FileSystemPartitionLabel::INTERNAL_OTP_PARTITION:
        return mInternalFlashOtpPartition;
    default:
        ASSERT(false);
        break;
    }
}
