/*
 * FirmwareUpdateEnvironment.cpp
 *
 *  Created on: 26 mars 2021
 *      Author: thomas
 */

#include "FirmwareEnvironmentRedundant.hpp"
#include "assertTools.h"

using namespace fs;

FirmwareEnvironmentRedundant::FirmwareEnvironmentRedundant(ICRC32& crc,
                                                           fs::IFileSystem& fileSystemEnvA,
                                                           std::string_view pathEnvA,
                                                           fs::IFileSystem& fileSystemEnvB,
                                                           std::string_view pathEnvB)
    : mCrc(crc)
    , mEnvA{fileSystemEnvA, std::string(pathEnvA)}
    , mEnvB{fileSystemEnvB, std::string(pathEnvB)}
{
}

bool FirmwareEnvironmentRedundant::reset()
{
    FirmwareEnvironment env;

    eraseAll();

    mActiveEnv = envBank::ENV_B;
    mId = 0;

    return writeEnvironment(env);
}

bool FirmwareEnvironmentRedundant::loadEnvMetadata(
    FirmwareEnvironmentRedundant::env_access_info& info, env_metadata_t& metadata) const
{
    bool ret = false;
    if(fs::File file(info.fileSystem, info.path, fs::OpenFlag::RDONLY);
       file.seek(0, SeekFlag::ABSOLUTE_POS) == 0
       && file.read(&metadata, sizeof(metadata)) == sizeof(metadata))
    {
        ret = true;
    }
    else
    {
        memset(&metadata, 0, sizeof(metadata));
    }

    return ret;
}

bool FirmwareEnvironmentRedundant::loadEnv(FirmwareEnvironmentRedundant::env_access_info& info,
                                           env_t& env) const
{
    bool ret = false;
    memset(&env, 0, sizeof(env));

    if(fs::File file = fs::File(info.fileSystem, info.path, fs::OpenFlag::RDONLY);
       file.seek(0, SeekFlag::ABSOLUTE_POS) == 0 && file.read(&env, sizeof(env)) == sizeof(env))
    {
        ret = validateEnv(env);
    }
    else
    {
        memset(&env, 0, sizeof(env));
    }

    if(ret && env.metadata.version != LATEST_VERSION)
    {
        env = migrate(env);
    }

    return ret;
}

bool FirmwareEnvironmentRedundant::validateEnv(env_t& env) const
{
    bool ret = false;
    uint32_t calculatedCrc;
    uint32_t envCrc = env.metadata.crc;
    size_t validateDataSize = 0;
    env.metadata.crc = 0; /* metadata is a copy, so we can modify it */

    if(getEnvTotalSize(env, validateDataSize)
       && computeCrc32(reinterpret_cast<uint8_t*>(&env), validateDataSize, calculatedCrc))
    {
        ret = (calculatedCrc == envCrc);
    }

    return ret;
}

FirmwareEnvironmentRedundant::env_access_info& FirmwareEnvironmentRedundant::getEnvInfo(envBank bank)
{
    ASSERT(bank == envBank::ENV_A || bank == envBank::ENV_B);
    return (bank == envBank::ENV_A ? mEnvA : mEnvB);
}

FirmwareEnvironmentRedundant::envBank FirmwareEnvironmentRedundant::getNextBank(envBank bank) const
{
    ASSERT(bank == envBank::ENV_A || bank == envBank::ENV_B);
    return (bank == envBank::ENV_A ? envBank::ENV_B : envBank::ENV_A);
}

bool FirmwareEnvironmentRedundant::getActiveEnvironment(FirmwareEnvironment& env)
{
    env_access_info& info = getEnvInfo(mActiveEnv);
    bool ret = false;

    if(loadEnv(info, mCacheEnv))
    {
        env = FirmwareEnvironment(
            etl::array_view<std::byte>(mCacheEnv.latest.data, sizeof(mCacheEnv.latest.data)));
        ret = true;
    }
    else
    {
        /* Should never happen */
        ASSERT(false);
    }

    return ret;
}

bool FirmwareEnvironmentRedundant::init()
{
    env_metadata_t metadataEnvB;
    bool ret = false;

    /* Set bank A as default environment */
    mActiveEnv = envBank::ENV_A;
    loadEnvMetadata(getEnvInfo(envBank::ENV_A), mCacheEnv.metadata);
    loadEnvMetadata(getEnvInfo(envBank::ENV_B), metadataEnvB);

    /* We use the latest env */
    if(mCacheEnv.metadata.id < metadataEnvB.id)
    {
        mActiveEnv = envBank::ENV_B;
    }

    /* We check content is valid */
    env_access_info& info = getEnvInfo(mActiveEnv);
    ret = loadEnv(info, mCacheEnv);

    /* If content is not valid we fallback on the other bank */
    if(!ret)
    {
        mActiveEnv = getNextBank(mActiveEnv);
        env_access_info& info = getEnvInfo(mActiveEnv);
        ret = loadEnv(info, mCacheEnv);
    }

    if(!ret)
    {
        mId = 0;
    }
    else
    {
        mId = mCacheEnv.metadata.id;
    }

    return ret;
}

void FirmwareEnvironmentRedundant::generateCacheCRC()
{
    uint32_t crc = 0;
    mCacheEnv.metadata.crc = 0;
    computeCrc32(reinterpret_cast<const uint8_t*>(&mCacheEnv),
                 static_cast<size_t>(sizeof(mCacheEnv)),
                 crc);
    mCacheEnv.metadata.crc = crc;
}

void FirmwareEnvironmentRedundant::resetCacheEnv()
{
    memset(&mCacheEnv, 0, sizeof(mCacheEnv));
    mCacheEnv.metadata.magicNumber = MAGIC_NUMBER;
    mCacheEnv.metadata.version = LATEST_VERSION;
    mCacheEnv.metadata.id = ++mId;
}

bool FirmwareEnvironmentRedundant::writeEnvironment(const FirmwareEnvironment& env)
{
    bool ret = false;

    envBank nextActiveEnv = getNextBank(mActiveEnv);
    env_access_info& info = getEnvInfo(nextActiveEnv);

    fs::File file(info.fileSystem);
    file.remove(info.path);
    file.open(info.path, fscommon::OpenFlag::CREAT | fs::OpenFlag::WRONLY | fs::OpenFlag::TRUNC);

    resetCacheEnv();
    if(env.toBytes(etl::array_view<std::byte>(mCacheEnv.latest.data, sizeof(mCacheEnv.latest.data)))
       > 0)
    {
        generateCacheCRC();

        ret = (file.write(&mCacheEnv, sizeof(mCacheEnv)) == sizeof(mCacheEnv));
    }

    if(ret)
    {
        mActiveEnv = nextActiveEnv;
    }

    file.close();

    return ret;
}

void FirmwareEnvironmentRedundant::eraseBank(envBank bank)
{
    env_access_info& info = getEnvInfo(bank);
    fs::File file(info.fileSystem);
    file.remove(info.path);
}

void FirmwareEnvironmentRedundant::eraseAll()
{
    eraseBank(envBank::ENV_A);
    eraseBank(envBank::ENV_B);
}

void FirmwareEnvironmentRedundant::dumpMetadata()
{
    std::array<envBank, 2> banks = {envBank::ENV_A, envBank::ENV_B};

    for(auto& bank: banks)
    {
        printf("bank %d\r\n", bank);
        env_access_info& info = getEnvInfo(bank);
        uint8_t data[sizeof(env_metadata_t)] = {0};
        if(fs::File file = fs::File(info.fileSystem, info.path, fs::OpenFlag::RDONLY);
           file.seek(0, SeekFlag::ABSOLUTE_POS) == 0
           && file.read(data, sizeof(env_metadata_t)) == sizeof(env_metadata_t))
        {
            for(uint16_t i = 0; i < sizeof(env_metadata_t); i++)
            {
                printf("%x\t", data[i]);
            }
            printf("\r\n");
        }
    }
}

bool FirmwareEnvironmentRedundant::getEnvTotalSize(const env_t& env, size_t& envTotalSize) const
{
    bool ret = true;

    switch(env.metadata.version)
    {
    case 2:
        envTotalSize = getStructTotalSize(sizeof(env.v2));
        break;
    case 1:
        envTotalSize = getStructTotalSize(sizeof(env.v1));
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}

size_t FirmwareEnvironmentRedundant::getStructTotalSize(size_t structSize) const
{
    return structSize + member_size(env_t, metadata);
}

FirmwareEnvironmentRedundant::env_t FirmwareEnvironmentRedundant::migrate(const env_t& env) const
{
    env_t migrated_env;

    memset(&migrated_env, 0, sizeof(migrated_env));

    switch(env.metadata.version)
    {
    case 1:
        memcpy(&migrated_env, &env, getStructTotalSize(sizeof(env.v1)));
        migrated_env.metadata.version = LATEST_VERSION;
        break;
    default:
        break;
    }

    return migrated_env;
}

bool FirmwareEnvironmentRedundant::computeCrc32(const uint8_t* buffer,
                                                size_t size,
                                                uint32_t& crc) const
{
    AutoLock crcLock(mCrc);

    mCrc.configure();

    return mCrc.compute(buffer, size, crc);
}