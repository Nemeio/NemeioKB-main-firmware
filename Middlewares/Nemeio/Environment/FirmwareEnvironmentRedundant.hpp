/*
 * FirmwareUpdateEnvironmentRedundant.hpp
 *
 *  Created on: 26 mars 2021
 *      Author: thomas
 */

#ifndef NEMEIO_FIRMWAREUPDATE_FIRMWAREUPDATEENVIRONMENTREDUNDANT_HPP_
#define NEMEIO_FIRMWAREUPDATE_FIRMWAREUPDATEENVIRONMENTREDUNDANT_HPP_

#include "FirmwareEnvironment.hpp"
#include <array>
#include "fs.hpp"
#include "cpp_helper.hpp"
#include "ICRC32.hpp"
#include "etl/array_view.h"

class FirmwareEnvironmentRedundant
{
public:
    explicit FirmwareEnvironmentRedundant(ICRC32& crc,
                                          fs::IFileSystem& fileSystemEnvA,
                                          std::string_view pathEnvA,
                                          fs::IFileSystem& fileSystemEnvB,
                                          std::string_view pathEnvB);
    virtual ~FirmwareEnvironmentRedundant() = default;

    FirmwareEnvironmentRedundant() = delete;
    COPYABLE_MOVABLE(FirmwareEnvironmentRedundant, delete);

    bool init();
    bool reset();
    [[nodiscard]] bool getActiveEnvironment(FirmwareEnvironment& env);
    [[nodiscard]] bool writeEnvironment(const FirmwareEnvironment& env);

    void dumpMetadata();

private:
    enum class envBank
    {
        UNKNOWN,
        ENV_A,
        ENV_B,
    };

    struct env_access_info
    {
        fs::IFileSystem& fileSystem;
        const std::string path;
    };

    ICRC32& mCrc;
    env_access_info mEnvA;
    env_access_info mEnvB;

    PACK((struct env_metadata_t
          {
              uint32_t magicNumber;
              uint32_t version;
              uint32_t crc; // CRC field is set to 0 when calculated */
              uint32_t id;  // Auto increment for each write
          }));
    static constexpr uint32_t MAGIC_NUMBER = 0xDEADBEEF;
    static constexpr uint32_t LATEST_VERSION = 2;
    static constexpr size_t ENV_MAX_SIZE = 128;

    PACK((struct envV1_t { std::byte data[22]; }));

    PACK((struct envV2_t { std::byte data[ENV_MAX_SIZE]; }));

    PACK((struct env_t
          {
              env_metadata_t metadata;
              union
              {
                  envV1_t v1;
                  envV2_t v2;
                  envV2_t latest;
              };
          }));

    env_t mCacheEnv;

    envBank mActiveEnv = envBank::UNKNOWN;
    uint32_t mId = 0;

    bool loadEnvMetadata(env_access_info& info, env_metadata_t& metadata) const;
    bool loadEnv(env_access_info& info, env_t& env) const;
    bool validateEnv(env_t& env) const;
    env_access_info& getEnvInfo(envBank bank);
    envBank getNextBank(envBank bank) const;
    void resetCacheEnv();
    void generateCacheCRC();
    void eraseBank(envBank bank);
    void eraseAll();
    bool getEnvTotalSize(const env_t& env, size_t& envTotalSize) const;
    size_t getStructTotalSize(size_t structSize) const;
    env_t migrate(const env_t& env) const;
    bool computeCrc32(const uint8_t* buffer, size_t size, uint32_t& crc) const;
};

#endif /* NEMEIO_FIRMWAREUPDATE_FIRMWAREUPDATEENVIRONMENTREDUNDANT_HPP_ */
