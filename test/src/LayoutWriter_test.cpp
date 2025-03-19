#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "LayoutWriter.hpp"
#include "MockFileSystem.hpp"
#include "MockLayoutMetadataCodec.hpp"
#include "gtest_helpers.hpp"
#include "LinuxFileSystem.hpp"
#include "CborLayoutMetadataCodec.hpp"
#include <fstream>
#include <filesystem>

using ::testing::_;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::SetArgReferee;

using namespace configuration;

template<typename FileSystem, typename Codec>
class LayoutWriterTestBase : public ::testing::Test
{
public:
    LayoutWriterTestBase(etl::string_view directoryPath)
        : layoutWriter(uuid::UuidView(id), fileSystem, directoryPath, codec)
    {
    }


protected:
    etl::array<std::byte, uuid::SIZE> id{std::byte(0x01), std::byte(0x00)};

    FileSystem fileSystem;
    Codec codec;

    LayoutWriter layoutWriter;
};

class LayoutWriterTest : public LayoutWriterTestBase<MockFileSystem, MockLayoutMetadataCodec>
{
public:
    LayoutWriterTest()
        : LayoutWriterTestBase(DIRECTORY_PATH)
    {
        ON_CALL(fileSystem, fileOpen).WillByDefault(Return(fs::FS_ERR_OK));
        ON_CALL(fileSystem, fileRead).WillByDefault(ReturnArg<2>());
    }

protected:
    static constexpr char DIRECTORY_PATH[] = "/";
    std::string fileName{"01000000000000000000000000000000.lyt"};
    std::filesystem::path filePath{std::filesystem::path(DIRECTORY_PATH) / fileName};
};

class LayoutWriterLinuxFileSystemTest
    : public LayoutWriterTestBase<LinuxFileSystem, CborLayoutMetadataCodec>
{
public:
    LayoutWriterLinuxFileSystemTest()
        : LayoutWriterTestBase(DIRECTORY_PATH)
    {
        std::filesystem::remove_all(DIRECTORY_PATH);
        std::filesystem::create_directory(DIRECTORY_PATH);
    }

protected:
    static constexpr char DIRECTORY_PATH[] = "tmp";
    std::string filePath{"tmp/01000000000000000000000000000000.lyt"};
};

TEST_F(LayoutWriterTest, InitWrite)
{
    EXPECT_CALL(fileSystem, fileOpen(_, Eq(filePath), _)).Times(1).WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = layoutWriter.initWrite();

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(LayoutWriterTest, InitWriteError)
{
    EXPECT_CALL(fileSystem, fileOpen(_, Eq(filePath), _)).Times(1).WillOnce(Return(fs::FS_ERR_IO));

    Ret ret = layoutWriter.initWrite();

    EXPECT_EQ(ret, Ret::FS);
}

TEST_F(LayoutWriterTest, Write)
{
    etl::array<std::byte, 5> data;
    EXPECT_CALL(fileSystem, fileWrite(_, data.data(), data.size()))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = layoutWriter.write(data);

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(LayoutWriterTest, WriteError)
{
    etl::array<std::byte, 5> data;
    EXPECT_CALL(fileSystem, fileWrite(_, data.data(), data.size()))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_IO));

    Ret ret = layoutWriter.write(data);

    EXPECT_EQ(ret, Ret::FS);
}

TEST_F(LayoutWriterLinuxFileSystemTest, EndWrite)
{
    std::string inputPackageFilePath{RESOURCE_FILES_PATH "/LayoutWriterTest/layout_package.lyt"};


    size_t fileSize = std::filesystem::file_size(inputPackageFilePath);
    std::ifstream input(inputPackageFilePath, std::ifstream::in | std::ifstream::binary);

    Ret ret = layoutWriter.initWrite();
    ASSERT_EQ(ret, Ret::SUCCESS);

    std::byte buffer[fileSize];
    if(input.read(reinterpret_cast<char*>(buffer), fileSize))
    {
        ret = layoutWriter.write(etl::array_view{buffer, fileSize});
        ASSERT_EQ(ret, Ret::SUCCESS);
    }

    ret = layoutWriter.endWrite();

    EXPECT_EQ(ret, Ret::SUCCESS);

    EXPECT_TRUE(std::filesystem::exists(filePath));
}

TEST_F(LayoutWriterLinuxFileSystemTest, EndWriteInvalidData)
{
    std::string inputPackageFilePath{RESOURCE_FILES_PATH
                                     "/LayoutWriterTest/layout_package_invalid.lyt"};

    size_t fileSize = std::filesystem::file_size(inputPackageFilePath);
    std::ifstream input(inputPackageFilePath, std::ifstream::in | std::ifstream::binary);

    Ret ret = layoutWriter.initWrite();
    ASSERT_EQ(ret, Ret::SUCCESS);

    std::byte buffer[fileSize];
    if(input.read(reinterpret_cast<char*>(buffer), fileSize))
    {
        ret = layoutWriter.write(etl::array_view{buffer, fileSize});
        ASSERT_EQ(ret, Ret::SUCCESS);
    }

    ret = layoutWriter.endWrite();

    EXPECT_EQ(ret, Ret::INVALID_DATA);

    EXPECT_FALSE(std::filesystem::exists(filePath));
}

TEST_F(LayoutWriterTest, Move)
{
    std::filesystem::path destDirectoryPath = "layout";
    std::filesystem::path destPath = destDirectoryPath / fileName;
    EXPECT_CALL(fileSystem, rename(Eq(filePath), Eq(destPath)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = layoutWriter.move(etl::string_view(destDirectoryPath.c_str()));

    EXPECT_EQ(ret, Ret::SUCCESS);
}