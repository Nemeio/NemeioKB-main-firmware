#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fs.hpp"
#include "LinuxBlockDevice.hpp"
#include "FirmwareEnvironmentRedundant.hpp"
#include "MockCRC32.hpp"
#include "MockFileSystem.hpp"
#include "gtest_helpers.hpp"


using ::testing::_;
using testing::DoAll;
using ::testing::Eq;
using testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using testing::SetArgPointee;
using testing::SetArgReferee;
using ::testing::StrictMock;

class FirmwareEnvironmentRedundantTest : public ::testing::Test
{
protected:
    static constexpr char pathA[] = "A";
    static constexpr char pathB[] = "B";

    FirmwareEnvironmentRedundantTest()
        : redundantEnv(crc, fileSystemEnvA, pathA, fileSystemEnvB, pathB)
    {
        ON_CALL(crc, lock).WillByDefault(Return(true));
        ON_CALL(crc, unlock).WillByDefault(Return(true));
    };

    StrictMock<MockFileSystem> fileSystemEnvA;
    StrictMock<MockFileSystem> fileSystemEnvB;
    NiceMock<MockCRC32> crc;
    FirmwareEnvironmentRedundant redundantEnv;

    static constexpr size_t ENV_MAX_SIZE = 128;

    PACK((struct env_metadata_t
          {
              uint32_t magicNumber;
              uint32_t version;
              uint32_t crc; // CRC field is set to 0 when calculated */
              uint32_t id;  // Auto increment for each write
          }));

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

    static constexpr size_t ENV_SIZE_V1 = member_size(env_t, metadata) + member_size(envV1_t, data);
    static constexpr size_t ENV_SIZE_V2 = member_size(env_t, metadata) + member_size(envV2_t, data);

    bool init()
    {
        /* File close is never called since we keep -1 as handle */
        env_t envA;
        env_t envB;

        memset(&envA, 0, sizeof(envA));
        envA.metadata.version = 2;
        envA.metadata.id = 0;
        envA.metadata.crc = 1;
        memset(&envB, 0, sizeof(envB));
        envB.metadata.version = 2;
        envB.metadata.id = 5;
        envB.metadata.crc = 50;

        EXPECT_CALL(fileSystemEnvA, fileOpen(_, _, _)).Times(2).WillRepeatedly(Return(true));
        EXPECT_CALL(fileSystemEnvA, fileSeek(_, Eq(0), Eq(fs::SeekFlag::ABSOLUTE_POS)))
            .Times(2)
            .WillRepeatedly(Return(0));
        /* Metadata */
        EXPECT_CALL(fileSystemEnvA, fileRead(_, _, Eq(sizeof(envA.metadata))))
            .WillOnce(DoAll(SetArgPointerContent<1>(&envA.metadata, sizeof(envA.metadata)),
                            Return(sizeof(envA.metadata))));
        /* Entire env */
        EXPECT_CALL(fileSystemEnvA, fileRead(_, _, Eq(sizeof(envA))))
            .WillOnce(DoAll(SetArgPointerContent<1>(&envA, sizeof(envA)), Return(sizeof(envA))));

        /* Env B */
        EXPECT_CALL(fileSystemEnvB, fileOpen(_, _, _)).Times(2).WillRepeatedly(Return(true));
        EXPECT_CALL(fileSystemEnvB, fileSeek(_, Eq(0), Eq(fs::SeekFlag::ABSOLUTE_POS)))
            .Times(2)
            .WillRepeatedly(Return(0));
        /* Metadata */
        EXPECT_CALL(fileSystemEnvB, fileRead(_, _, Eq(sizeof(envB.metadata))))
            .WillOnce(DoAll(SetArgPointerContent<1>(&envB.metadata, sizeof(envB.metadata)),
                            Return(sizeof(envB.metadata))));
        /* Entire env */
        EXPECT_CALL(fileSystemEnvB, fileRead(_, _, Eq(sizeof(envB))))
            .WillOnce(DoAll(SetArgPointerContent<1>(&envB, sizeof(envB)), Return(sizeof(envB))));

        /* Invalid CRC for both env */
        EXPECT_CALL(crc, compute(_, _, _))
            .Times(2)
            .WillRepeatedly(DoAll(SetArgReferee<2>(0xFF), Return(true)));

        return redundantEnv.init();
    }

    void testSaveEnv(MockFileSystem& expectedSaveFileSystem,
                     const char* expectedPath,
                     uint32_t expectedVersion,
                     uint32_t expectedId)
    {
        env_t expectedEnv;

        memset(&expectedEnv, 0, sizeof(expectedEnv));
        expectedEnv.metadata.version = expectedVersion;
        expectedEnv.metadata.id = expectedId;
        expectedEnv.metadata.crc = 1;

        uint32_t crcExpected = expectedEnv.metadata.crc;

        EXPECT_CALL(crc, compute(_, _, _))
            .WillOnce(DoAll(SetArgReferee<2>(crcExpected), Return(true)));
        EXPECT_CALL(expectedSaveFileSystem, remove(Eq(expectedPath))).WillOnce(Return(0));
        EXPECT_CALL(expectedSaveFileSystem, fileOpen(_, Eq(expectedPath), _)).WillOnce(Return(true));

        env_t writtenEnv;
        EXPECT_CALL(expectedSaveFileSystem, fileWrite(_, _, Eq(sizeof(env_t))))
            .WillOnce(
                DoAll(SaveArgPointerContent<1>(&writtenEnv, sizeof(env_t)), Return(sizeof(env_t))));

        EXPECT_CALL(expectedSaveFileSystem, fileClose(_)).WillOnce(Return(true));

        FirmwareEnvironment env;
        EXPECT_TRUE(redundantEnv.writeEnvironment(env));

        EXPECT_EQ(writtenEnv.metadata.crc, expectedEnv.metadata.crc);
        EXPECT_EQ(writtenEnv.metadata.id, expectedEnv.metadata.id);
        EXPECT_EQ(writtenEnv.metadata.version, expectedEnv.metadata.version);
    }
};

TEST_F(FirmwareEnvironmentRedundantTest, ExistingValidValuesBankA)
{
    env_t envA;
    env_t envB;

    memset(&envA, 0, sizeof(envA));
    envA.metadata.version = 2;
    envA.metadata.id = 5;
    envA.metadata.crc = 1;
    memset(&envB, 0, sizeof(envB));
    envB.metadata.version = 2;
    envB.metadata.id = 0;
    envB.metadata.crc = 50;

    uint32_t crcA = envA.metadata.crc;

    EXPECT_CALL(fileSystemEnvA, fileOpen(_, _, _)).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(fileSystemEnvA, fileSeek(_, Eq(0), Eq(fs::SeekFlag::ABSOLUTE_POS)))
        .Times(2)
        .WillRepeatedly(Return(0));
    /* Metadata */
    EXPECT_CALL(fileSystemEnvA, fileRead(_, _, Eq(sizeof(envA.metadata))))
        .WillOnce(DoAll(SetArgPointerContent<1>(&envA.metadata, sizeof(envA.metadata)),
                        Return(sizeof(envA.metadata))));
    /* Entire env */
    EXPECT_CALL(fileSystemEnvA, fileRead(_, _, Eq(sizeof(envA))))
        .WillOnce(DoAll(SetArgPointerContent<1>(&envA, sizeof(envA)), Return(sizeof(envA))));
    EXPECT_CALL(crc, compute(_, ENV_SIZE_V2, _))
        .WillOnce(DoAll(SetArgReferee<2>(crcA), Return(true)));

    EXPECT_CALL(fileSystemEnvB, fileOpen(_, _, _)).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(fileSystemEnvB, fileSeek(_, Eq(0), Eq(fs::SeekFlag::ABSOLUTE_POS)))
        .Times(1)
        .WillRepeatedly(Return(0));
    /* Metadata */
    EXPECT_CALL(fileSystemEnvB, fileRead(_, _, Eq(sizeof(envB.metadata))))
        .WillOnce(DoAll(SetArgPointerContent<1>(&envB.metadata, sizeof(envB.metadata)),
                        Return(sizeof(envB.metadata))));

    EXPECT_TRUE(redundantEnv.init());

    /* Save next environment */
    Mock::VerifyAndClearExpectations(&fileSystemEnvA);
    Mock::VerifyAndClearExpectations(&fileSystemEnvB);
    testSaveEnv(fileSystemEnvB, pathB, 2, 6);
};

TEST_F(FirmwareEnvironmentRedundantTest, ExistingValidValuesBankB)
{
    env_t envA;
    env_t envB;

    memset(&envA, 0, sizeof(envA));
    envA.metadata.version = 2;
    envA.metadata.id = 0;
    envA.metadata.crc = 1;
    memset(&envB, 0, sizeof(envB));
    envB.metadata.version = 2;
    envB.metadata.id = 7;
    envB.metadata.crc = 50;

    uint32_t crcB = envB.metadata.crc;

    EXPECT_CALL(fileSystemEnvA, fileOpen(_, _, _)).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(fileSystemEnvA, fileSeek(_, Eq(0), Eq(fs::SeekFlag::ABSOLUTE_POS)))
        .Times(1)
        .WillRepeatedly(Return(0));
    /* Metadata */
    EXPECT_CALL(fileSystemEnvA, fileRead(_, _, Eq(sizeof(envA.metadata))))
        .WillOnce(DoAll(SetArgPointerContent<1>(&envA.metadata, sizeof(envA.metadata)),
                        Return(sizeof(envA.metadata))));

    /* Env B */
    EXPECT_CALL(fileSystemEnvB, fileOpen(_, _, _)).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(fileSystemEnvB, fileSeek(_, Eq(0), Eq(fs::SeekFlag::ABSOLUTE_POS)))
        .Times(2)
        .WillRepeatedly(Return(0));
    /* Metadata */
    EXPECT_CALL(fileSystemEnvB, fileRead(_, _, Eq(sizeof(envB.metadata))))
        .WillOnce(DoAll(SetArgPointerContent<1>(&envB.metadata, sizeof(envB.metadata)),
                        Return(sizeof(envB.metadata))));
    /* Entire env */
    EXPECT_CALL(fileSystemEnvB, fileRead(_, _, Eq(sizeof(envB))))
        .WillOnce(DoAll(SetArgPointerContent<1>(&envB, sizeof(envB)), Return(sizeof(envB))));
    /* Valid CRC for env B */
    EXPECT_CALL(crc, compute(_, ENV_SIZE_V2, _))
        .WillOnce(DoAll(SetArgReferee<2>(crcB), Return(true)));

    EXPECT_TRUE(redundantEnv.init());

    /* Save next environment */
    Mock::VerifyAndClearExpectations(&fileSystemEnvA);
    Mock::VerifyAndClearExpectations(&fileSystemEnvB);
    testSaveEnv(fileSystemEnvA, pathA, 2, 8);
}

TEST_F(FirmwareEnvironmentRedundantTest, NoValidBank)
{
    EXPECT_EQ(init(), false);

    /* Remove and write env */
    EXPECT_CALL(fileSystemEnvA, remove(Eq(pathA))).Times(2).WillRepeatedly(Return(0));
    EXPECT_CALL(fileSystemEnvB, remove(Eq(pathB))).WillOnce(Return(0));

    EXPECT_CALL(crc, compute(_, _, _)).WillOnce(DoAll(SetArgReferee<2>(0), Return(true)));

    EXPECT_CALL(fileSystemEnvA, fileOpen(_, Eq(pathA), _)).WillOnce(Return(true));

    EXPECT_CALL(fileSystemEnvA, fileWrite(_, _, Eq(sizeof(env_t))))
        .WillOnce(DoAll(Return(sizeof(env_t))));

    EXPECT_CALL(fileSystemEnvA, fileClose(_)).WillOnce(Return(true));

    redundantEnv.reset();
}

TEST_F(FirmwareEnvironmentRedundantTest, MigrationV1ToV2)
{
    env_t envA;
    env_t envB;

    memset(&envA, 0, sizeof(envA));
    envA.metadata.version = 1;
    envA.metadata.id = 0;
    envA.metadata.crc = 1;
    memset(&envB, 0, sizeof(envB));
    envB.metadata.version = 1;
    envB.metadata.id = 5;
    envB.metadata.crc = 50;

    uint32_t crcB = envB.metadata.crc;

    EXPECT_CALL(fileSystemEnvA, fileOpen(_, _, _)).Times(1).WillRepeatedly(Return(true));
    EXPECT_CALL(fileSystemEnvA, fileSeek(_, Eq(0), Eq(fs::SeekFlag::ABSOLUTE_POS)))
        .Times(1)
        .WillRepeatedly(Return(0));
    /* Metadata */
    EXPECT_CALL(fileSystemEnvA, fileRead(_, _, Eq(sizeof(envA.metadata))))
        .WillOnce(DoAll(SetArgPointerContent<1>(&envA.metadata, sizeof(envA.metadata)),
                        Return(sizeof(envA.metadata))));

    /* Env B */
    EXPECT_CALL(fileSystemEnvB, fileOpen(_, _, _)).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(fileSystemEnvB, fileSeek(_, Eq(0), Eq(fs::SeekFlag::ABSOLUTE_POS)))
        .Times(2)
        .WillRepeatedly(Return(0));
    /* Metadata */
    EXPECT_CALL(fileSystemEnvB, fileRead(_, _, Eq(sizeof(envB.metadata))))
        .WillOnce(DoAll(SetArgPointerContent<1>(&envB.metadata, sizeof(envB.metadata)),
                        Return(sizeof(envB.metadata))));
    /* Entire env */
    EXPECT_CALL(fileSystemEnvB, fileRead(_, _, Eq(sizeof(envB))))
        .WillOnce(DoAll(SetArgPointerContent<1>(&envB, sizeof(envB)), Return(sizeof(envB))));
    /* Valid CRC for env B */
    EXPECT_CALL(crc, compute(_, ENV_SIZE_V1, _))
        .WillOnce(DoAll(SetArgReferee<2>(crcB), Return(true)));

    EXPECT_TRUE(redundantEnv.init());

    /* Save next environment */
    Mock::VerifyAndClearExpectations(&fileSystemEnvA);
    Mock::VerifyAndClearExpectations(&fileSystemEnvB);
    testSaveEnv(fileSystemEnvA, pathA, 2, 6);
}