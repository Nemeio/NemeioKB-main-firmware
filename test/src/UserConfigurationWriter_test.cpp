#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "UserConfigurationWriter.hpp"
#include "MockFileSystem.hpp"
#include "MockUserConfigurationCodec.hpp"
#include "MockLayoutConfigurationCodec.hpp"
#include "SharedBufferContainer.hpp"
#include "gtest_helpers.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Return;
using ::testing::StrictMock;

using namespace configuration;

class UserConfigurationWriterTest : public ::testing::Test
{
public:
    UserConfigurationWriterTest()
        : userConfigurationWriter(uuid::UuidView(id),
                                  fileSystem,
                                  etl::string_view(directoryPath.c_str()),
                                  codec,
                                  layoutConfigurationCodec,
                                  sharedBufferContainer)
    {
    }


protected:
    std::string directoryPath{"new"};
    std::string filePath{"new/.ucfg"};

    etl::array<std::byte, uuid::SIZE> id{std::byte(0x01), std::byte(0x00)};

    StrictMock<MockFileSystem> fileSystem;
    MockUserConfigurationCodec codec;
    MockLayoutConfigurationCodec layoutConfigurationCodec;
    SharedBufferContainer sharedBufferContainer;

    UserConfigurationWriter userConfigurationWriter;
};

TEST_F(UserConfigurationWriterTest, InitWrite)
{
    EXPECT_CALL(fileSystem, mkdir(Eq(directoryPath))).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, remove_recursive(Eq(directoryPath)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, fileOpen(_, Eq(filePath), _)).Times(1).WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = userConfigurationWriter.initWrite();

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationWriterTest, InitWriteError)
{
    EXPECT_CALL(fileSystem, mkdir).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, remove_recursive).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, fileOpen).Times(1).WillOnce(Return(fs::FS_ERR_IO));

    Ret ret = userConfigurationWriter.initWrite();

    EXPECT_EQ(ret, Ret::FS);
}

TEST_F(UserConfigurationWriterTest, Write)
{
    etl::array<std::byte, 5> data;
    EXPECT_CALL(fileSystem, fileWrite(_, data.data(), data.size()))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = userConfigurationWriter.write(data);

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationWriterTest, WriteError)
{
    etl::array<std::byte, 5> data;
    EXPECT_CALL(fileSystem, fileWrite(_, data.data(), data.size()))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_IO));

    Ret ret = userConfigurationWriter.write(data);

    EXPECT_EQ(ret, Ret::FS);
}

TEST_F(UserConfigurationWriterTest, EndWrite)
{
    etl::array<std::byte, 3> readData{std::byte(0xaa), std::byte(0xbb), std::byte(0xcc)};

    EXPECT_CALL(fileSystem, fileRewind).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, fileSize).Times(1).WillOnce(Return(readData.size()));
    EXPECT_CALL(fileSystem, fileRead)
        .Times(1)
        .WillOnce(DoAll(SetArgPointerContent<1>(readData.data(), readData.size()),
                        Return(readData.size())));

    EXPECT_CALL(codec, decode(etl::array_view<const std::byte>(readData), _))
        .Times(1)
        .WillOnce(Return(true));
    EXPECT_CALL(fileSystem, fileClose).Times(1).WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = userConfigurationWriter.endWrite();

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationWriterTest, EndWriteInvalidData)
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
    EXPECT_CALL(fileSystem, remove_recursive(Eq(directoryPath)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = userConfigurationWriter.endWrite();

    EXPECT_EQ(ret, Ret::INVALID_DATA);
}

TEST_F(UserConfigurationWriterTest, EndWriteFsError)
{
    size_t fileSize = 2;
    EXPECT_CALL(fileSystem, fileRewind).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, fileSize).Times(1).WillOnce(Return(fileSize));
    EXPECT_CALL(fileSystem, fileRead).Times(1).WillOnce(Return(fs::FS_ERR_CORRUPT));

    EXPECT_CALL(fileSystem, fileClose).Times(1).WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, remove_recursive(Eq(directoryPath)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = userConfigurationWriter.endWrite();

    EXPECT_EQ(ret, Ret::FS);
}

TEST_F(UserConfigurationWriterTest, Move)
{
    std::string destDirectoryPath{"dest"};
    std::string destDirectoryPathBkp{"dest_bkp"};
    EXPECT_CALL(fileSystem, rename(Eq(destDirectoryPath), Eq(destDirectoryPathBkp)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, remove_recursive(Eq(destDirectoryPathBkp)))
        .Times(2)
        .WillRepeatedly(Return(fs::FS_ERR_OK));

    EXPECT_CALL(fileSystem, rename(Eq(directoryPath), Eq(destDirectoryPath)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = userConfigurationWriter.move(etl::string_view(destDirectoryPath.c_str()));

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationWriterTest, MoveError)
{
    std::string destDirectoryPath{"dest"};
    std::string destDirectoryPathBkp{"dest_bkp"};
    EXPECT_CALL(fileSystem, rename(Eq(destDirectoryPath), Eq(destDirectoryPathBkp)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, rename(Eq(destDirectoryPathBkp), Eq(destDirectoryPath)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));
    EXPECT_CALL(fileSystem, remove_recursive(Eq(destDirectoryPathBkp)))
        .Times(1)
        .WillRepeatedly(Return(fs::FS_ERR_OK));

    EXPECT_CALL(fileSystem, rename(Eq(directoryPath), Eq(destDirectoryPath)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_NOENT));

    Ret ret = userConfigurationWriter.move(etl::string_view(destDirectoryPath.c_str()));

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}