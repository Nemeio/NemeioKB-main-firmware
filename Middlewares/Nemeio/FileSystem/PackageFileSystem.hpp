/*
 * PackageFileSystem.hpp
 *
 *  Created on: 28 janv. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_PACKAGEFILESYSTEM_HPP_
#define NEMEIO_FIRMWAREUPDATE_PACKAGEFILESYSTEM_HPP_

#include <string>
#include <cstdint>
#include "utils.hpp"
#include "fs.hpp"
#include "etl/string.h"

inline constexpr char FIRMWARE_PACKAGE_STM32_PATH[] = "1";
inline constexpr char FIRMWARE_PACKAGE_NRF_PATH[] = "2";
inline constexpr char FIRMWARE_PACKAGE_ITE_PATH[] = "3";

using eFirmwareFileIndex = uint8_t;
static constexpr size_t FIRMWARE_MAX_AND_INVALID_INDEX = 7;

enum eCompressionType : uint8_t
{
    FIRMWARE_COMPRESSION_NONE = 0,
    FIRMWARE_COMPRESSION_ZIP = 1,
    FIRMWARE_COMPRESSION_LZ4 = 2,
};

PACK((struct packageVersion
      {
          uint8_t major;
          uint8_t minor;
          uint8_t unused;
          uint32_t unused2;
      }));

class PackageFileSystem : public fs::IFileSystem
{
public:
    explicit PackageFileSystem(fs::IFileSystem& fileSystem, std::string_view path);
    virtual ~PackageFileSystem() = default;

    bool getVersion(const std::string_view path, packageVersion& version);
    uint8_t getFilesNumber() const;

    int init() override;
    int deinit() override;
    int format() override;
    int mount() override;
    int unmount() override;
    int remove(const std::string_view path) override;
    int remove_recursive(const std::string_view path) override;
    int rename(std::string_view oldpath, std::string_view newpath) override;
    int stat(const std::string& path, fscommon::FSInfo& info) override;
    int fileOpen(fscommon::FileHandle& file, std::string_view path, int flags) override;
    int fileClose(fscommon::FileHandle& file) override;
    int fileSync(fscommon::FileHandle& file) override;
    ssize_t fileRead(fscommon::FileHandle& file, void* buffer, std::size_t size) override;
    ssize_t fileWrite(fscommon::FileHandle& file, const void* buffer, std::size_t size) override;
    ssize_t fileSeek(fscommon::FileHandle& file, ssize_t off, fscommon::SeekFlag flag) override;
    int fileTruncate(fscommon::FileHandle& file, std::size_t size) override;
    ssize_t fileTell(fscommon::FileHandle& file) override;
    int fileRewind(fscommon::FileHandle& file) override;
    ssize_t fileSize(fscommon::FileHandle& file) override;
    int mkdir(const char* path) override;
    int dirOpen(fscommon::DirHandle& dir, std::string_view path) override;
    int dirClose(fscommon::DirHandle& dir) override;
    int dirRead(fscommon::DirHandle& dir, fscommon::FSInfo& info) override;
    int dirSeek(fscommon::DirHandle& dir, std::size_t off) override;
    ssize_t dirTell(fscommon::DirHandle& dir) override;
    int dirRewind(fscommon::DirHandle& dir) override;
    int findFirstFile(fscommon::DirHandle& dir,
                      const std::string& path,
                      const std::string& pattern,
                      fscommon::FSInfo& info) override;
    int findNextFile(fscommon::DirHandle& dir,
                     const std::string& pattern,
                     fscommon::FSInfo& info) override;
    int traverse(fscommon::TraverseCallback& callback) override;
    std::size_t getBlockCount() const override;
    int bdRead(std::size_t block, std::size_t off, void* buffer, std::size_t size) override;
    int bdProg(std::size_t block, std::size_t off, const void* buffer, std::size_t size) override;
    int bdErase(std::size_t block) override;
    int bdSync() override;
    void lock() override;
    void unlock() override;

protected:
    static constexpr uint8_t LATEST_VERSION = 3;
    PACK((struct s_globalHeaderCommon
          {
              uint32_t magicNumber;
              uint8_t version;
              uint32_t size;
          }));

    PACK((struct s_globalHeaderSpecific_V1 { uint8_t dummy[32]; }));

    PACK((struct s_globalHeaderSpecific_V2
          {
              uint8_t dummy[32];
              uint8_t firmwareNumber;
          }));

    PACK((struct s_globalHeaderSpecific_V3
          {
              uint8_t signature[64];
              uint8_t firmwareNumber;
          }));

    PACK((struct s_globalHeader
          {
              s_globalHeaderCommon common;
              union
              {
                  s_globalHeaderSpecific_V1 v1;
                  s_globalHeaderSpecific_V2 v2;
                  s_globalHeaderSpecific_V3 v3;
                  s_globalHeaderSpecific_V3 latest;
              };
          }));

    PACK((struct s_FirmwareHeader
          {
              enum eCompressionType compression;
              packageVersion version;
              uint32_t firmware_offset;
              uint32_t firmware_size;
          }));

    struct handle
    {
        bool used;
        s_FirmwareHeader* info;
        size_t offset;
    };

    fs::File mFirmwarePackageFile;
    etl::string<16> mFirmwarePackageFilePath;
    RecursiveMutex mMutex;

    struct s_FirmwarePackageMetadata
    {
        s_globalHeader global;
        s_FirmwareHeader firmwares[FIRMWARE_MAX_AND_INVALID_INDEX];
    } mFirmwarePackageMetadata;

private:
    static constexpr uint32_t HEADER_MAGIC_NUMBER = 0x1d1c1d1c;
    std::array<handle, FIRMWARE_MAX_AND_INVALID_INDEX>
        mHandles; /* The same file can be opened only once at the same time */

    eFirmwareFileIndex getFirmwareIndex(std::string_view path) const;
    bool isMounted() const;
    [[nodiscard]] bool tryFileSeek(const fscommon::FileHandle& file, std::size_t off) const;
    int loadFirmwaresInformation();
};

#endif /* NEMEIO_FIRMWAREUPDATE_PACKAGEFILESYSTEM_HPP_ */
