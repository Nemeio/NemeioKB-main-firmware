#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "LayoutConfigurationWriter.hpp"
#include "MockFileSystem.hpp"
#include "MockLayoutConfigurationCodec.hpp"
#include "SharedBufferContainer.hpp"
#include "gtest_helpers.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::StrictMock;

using namespace configuration;

class LayoutConfigurationWriterTest : public ::testing::Test
{
public:
    LayoutConfigurationWriterTest()
        : layoutConfigurationWriter(uuid::UuidView(id),
                                    fileSystem,
                                    directoryPath,
                                    codec,
                                    sharedBufferContainer)
    {
    }


protected:
    etl::string<3> directoryPath{"new"};
    std::string filePath{"new/01000000000000000000000000000000.lcfg"};

    etl::array<std::byte, uuid::SIZE> id{std::byte(0x01), std::byte(0x00)};

    StrictMock<MockFileSystem> fileSystem;
    MockLayoutConfigurationCodec codec;
    SharedBufferContainer sharedBufferContainer;

    LayoutConfigurationWriter layoutConfigurationWriter;
};

TEST_F(LayoutConfigurationWriterTest, InitWrite)
{
    EXPECT_CALL(fileSystem, fileOpen(_, Eq(filePath), _)).Times(1).WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = layoutConfigurationWriter.initWrite();

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(LayoutConfigurationWriterTest, InitWriteError)
{
    EXPECT_CALL(fileSystem, fileOpen(_, Eq(filePath), _)).Times(1).WillOnce(Return(fs::FS_ERR_IO));

    Ret ret = layoutConfigurationWriter.initWrite();

    EXPECT_EQ(ret, Ret::FS);
}

TEST_F(LayoutConfigurationWriterTest, Write)
{
    etl::array<std::byte, 5> data;
    EXPECT_CALL(fileSystem, fileWrite(_, data.data(), data.size()))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = layoutConfigurationWriter.write(data);

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(LayoutConfigurationWriterTest, WriteError)
{
    etl::array<std::byte, 5> data;
    EXPECT_CALL(fileSystem, fileWrite(_, data.data(), data.size()))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_IO));

    Ret ret = layoutConfigurationWriter.write(data);

    EXPECT_EQ(ret, Ret::FS);
}

TEST_F(LayoutConfigurationWriterTest, EndWrite)
{
    etl::array<std::byte, 3> readData{std::byte(0xaa), std::byte(0xbb), std::byte(0xcc)};

    EXPECT_CALL(fileSystem, fileRewind).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, fileSize).Times(1).WillOnce(Return(readData.size()));
    EXPECT_CALL(fileSystem, fileRead)
        .Times(1)
        .WillOnce(DoAll(SetArgPointerContent<1>(readData.data(), readData.size()),
                        Return(readData.size())));

    EXPECT_CALL(codec, decode(etl::array_view<const std::byte>(readData), _, _))
        .Times(1)
        .WillOnce(Return(true));
    EXPECT_CALL(fileSystem, fileClose).Times(1).WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = layoutConfigurationWriter.endWrite();

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(LayoutConfigurationWriterTest, EndWriteInvalidData)
{
    etl::array<std::byte, 3> readData{std::byte(0xaa), std::byte(0xbb), std::byte(0xcc)};

    EXPECT_CALL(fileSystem, fileRewind).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, fileSize).Times(1).WillOnce(Return(readData.size()));
    EXPECT_CALL(fileSystem, fileRead)
        .Times(1)
        .WillOnce(DoAll(SetArgPointerContent<1>(readData.data(), readData.size()),
                        Return(readData.size())));

    EXPECT_CALL(codec, decode).Times(1).WillOnce(Return(false));
    EXPECT_CALL(fileSystem, fileClose).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, remove(Eq(filePath))).Times(1).WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = layoutConfigurationWriter.endWrite();

    EXPECT_EQ(ret, Ret::INVALID_DATA);
}

TEST_F(LayoutConfigurationWriterTest, EndWriteFsError)
{
    size_t fileSize = 2;
    EXPECT_CALL(fileSystem, fileRewind).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, fileSize).Times(1).WillOnce(Return(fileSize));
    EXPECT_CALL(fileSystem, fileRead).Times(1).WillOnce(Return(fs::FS_ERR_CORRUPT));

    EXPECT_CALL(fileSystem, fileClose).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, remove(Eq(filePath))).Times(1).WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = layoutConfigurationWriter.endWrite();

    EXPECT_EQ(ret, Ret::FS);
}