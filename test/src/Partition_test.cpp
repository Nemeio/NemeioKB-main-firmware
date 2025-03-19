#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gtest_helpers.hpp"

#include "Partition.hpp"
#include "MockBlockDevice.hpp"

using ::testing::Return;
using ::testing::StrictMock;

class PartitionTest : public ::testing::Test
{
protected:
    PartitionTest()
    {
        EXPECT_CALL(mBlockDevice, get_read_size).WillRepeatedly(Return(BLOCK_DEFAULT_SIZE));
        EXPECT_CALL(mBlockDevice, get_erase_size).WillRepeatedly(Return(BLOCK_DEFAULT_SIZE));
        EXPECT_CALL(mBlockDevice, get_program_size).WillRepeatedly(Return(BLOCK_DEFAULT_SIZE));
    }
    StrictMock<MockBlockDevice> mBlockDevice;

    static constexpr size_t BLOCK_DEFAULT_SIZE = 4096;
    static constexpr size_t DEFAULT_START_ADDRESS = 4096 * 3;
    static constexpr size_t DEFAULT_SIZE = BLOCK_DEFAULT_SIZE * 5;
};

TEST_F(PartitionTest, ProgramNominal)
{
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_CALL(mBlockDevice, program(nullptr, DEFAULT_START_ADDRESS, BLOCK_DEFAULT_SIZE)).Times(1);

    EXPECT_EQ(part.program(nullptr, 0, BLOCK_DEFAULT_SIZE), 0);
};

TEST_F(PartitionTest, ProgramNominalOffset)
{
    size_t offset = BLOCK_DEFAULT_SIZE;
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_CALL(mBlockDevice, program(nullptr, DEFAULT_START_ADDRESS + offset, BLOCK_DEFAULT_SIZE))
        .Times(1);

    EXPECT_EQ(part.program(nullptr, offset, BLOCK_DEFAULT_SIZE), 0);
};

TEST_F(PartitionTest, ProgramOutOfBoundAddress)
{
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_ASSERTION_FAILED(part.program(nullptr, 0 + DEFAULT_SIZE, BLOCK_DEFAULT_SIZE));
};

TEST_F(PartitionTest, ProgramOutOfBoundSize)
{
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS + DEFAULT_SIZE, DEFAULT_SIZE);

    EXPECT_ASSERTION_FAILED(part.program(nullptr, 0, DEFAULT_SIZE + BLOCK_DEFAULT_SIZE));
};

TEST_F(PartitionTest, ReadNomimal)
{
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_CALL(mBlockDevice, read(nullptr, DEFAULT_START_ADDRESS, BLOCK_DEFAULT_SIZE)).Times(1);

    EXPECT_EQ(part.read(nullptr, 0, BLOCK_DEFAULT_SIZE), 0);
};

TEST_F(PartitionTest, ReadNomimalOffset)
{
    size_t offset = BLOCK_DEFAULT_SIZE;
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_CALL(mBlockDevice, read(nullptr, DEFAULT_START_ADDRESS + offset, BLOCK_DEFAULT_SIZE))
        .Times(1);

    EXPECT_EQ(part.read(nullptr, offset, BLOCK_DEFAULT_SIZE), 0);
};


TEST_F(PartitionTest, ReadOutOfBoundAddress)
{
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_ASSERTION_FAILED(part.read(nullptr, 0 + DEFAULT_SIZE, BLOCK_DEFAULT_SIZE));
};

TEST_F(PartitionTest, ReadOutOfBoundSize)
{
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_ASSERTION_FAILED(part.read(nullptr, 0, DEFAULT_SIZE + BLOCK_DEFAULT_SIZE));
};


TEST_F(PartitionTest, EraseNominal)
{
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_CALL(mBlockDevice, erase(DEFAULT_START_ADDRESS, BLOCK_DEFAULT_SIZE)).Times(1);

    EXPECT_EQ(part.erase(0, BLOCK_DEFAULT_SIZE), 0);
};

TEST_F(PartitionTest, EraseNominalOffset)
{
    size_t offset = BLOCK_DEFAULT_SIZE;
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_CALL(mBlockDevice, erase(DEFAULT_START_ADDRESS + offset, BLOCK_DEFAULT_SIZE)).Times(1);

    EXPECT_EQ(part.erase(offset, BLOCK_DEFAULT_SIZE), 0);
};

TEST_F(PartitionTest, EraseOutOfBoundAddress)
{
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_ASSERTION_FAILED(part.erase(DEFAULT_START_ADDRESS + DEFAULT_SIZE, BLOCK_DEFAULT_SIZE));
};

TEST_F(PartitionTest, EraseOutOfBoundSize)
{
    Partition part(mBlockDevice, DEFAULT_START_ADDRESS, DEFAULT_SIZE);

    EXPECT_ASSERTION_FAILED(part.erase(DEFAULT_START_ADDRESS, DEFAULT_SIZE + BLOCK_DEFAULT_SIZE));
};
