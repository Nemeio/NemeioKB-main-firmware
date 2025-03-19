#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "LayoutStorage.hpp"
#include "MockFileSystem.hpp"
#include "MockLayoutMetadataCodec.hpp"
#include "LinuxFileSystem.hpp"
#include "CborLayoutMetadataCodec.hpp"
#include <fstream>
#include <filesystem>
#include "UuidListIteratorWrapper.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;

using namespace configuration;

template<typename FileSystem, typename Codec>
class LayoutStorageTestBase : public ::testing::Test
{
public:
    LayoutStorageTestBase(etl::string_view directoryPath)
        : id1({reinterpret_cast<std::byte*>(id1U8), sizeof(id1U8)})
        , id2({reinterpret_cast<std::byte*>(id2U8), sizeof(id2U8)})
        , layoutStorage(fileSystem, directoryPath, codec)
    {
    }

    bool getList(std::vector<uuid::Uuid>& list)
    {
        UuidListIteratorWrapper itWrapper(layoutStorage);

        if(!itWrapper.it().has_value())
        {
            return false;
        }

        auto& it = itWrapper.it().value().get();
        auto id = it.next();
        while(id.has_value())
        {
            list.push_back(id.value());
            id = it.next();
        }

        return true;
    }

protected:
    uint8_t id1U8[16]{0xfe,
                      0xab,
                      0x1e,
                      0x5f,
                      0x62,
                      0x4e,
                      0x40,
                      0xdf,
                      0x99,
                      0x65,
                      0xe2,
                      0x04,
                      0xc9,
                      0xa9,
                      0x44,
                      0x55};
    uuid::UuidView id1;

    uint8_t id2U8[16]{0xff,
                      0xab,
                      0x1e,
                      0x5f,
                      0x62,
                      0x4e,
                      0x40,
                      0xdf,
                      0x99,
                      0x65,
                      0xe2,
                      0x04,
                      0xc9,
                      0xa9,
                      0x44,
                      0x55};
    uuid::UuidView id2;

    FileSystem fileSystem;
    Codec codec;

    LayoutStorage layoutStorage;
};

class LayoutStorageTest : public LayoutStorageTestBase<MockFileSystem, MockLayoutMetadataCodec>
{
public:
    LayoutStorageTest()
        : LayoutStorageTestBase(DIRECTORY_PATH)
    {
    }

protected:
    static constexpr char DIRECTORY_PATH[] = "layout";
};

class LayoutStorageLinuxFileSystemTest
    : public LayoutStorageTestBase<LinuxFileSystem, CborLayoutMetadataCodec>
{
public:
    LayoutStorageLinuxFileSystemTest()
        : LayoutStorageTestBase(DIRECTORY_PATH)
    {
        std::filesystem::remove_all(DIRECTORY_PATH);
        std::filesystem::create_directory(DIRECTORY_PATH);
    }

    void setupDefault()
    {
        std::filesystem::path inputDir = RESOURCE_FILES_PATH "/LayoutStorageTest/";
        std::filesystem::path outputDir = DIRECTORY_PATH;


        std::filesystem::copy(inputDir / "layout_valid.lyt",
                              outputDir / "feab1e5f624e40df9965e204c9a94455.lyt");
        std::filesystem::copy(inputDir / "layout_invalid.lyt",
                              outputDir / "ffab1e5f624e40df9965e204c9a94455.lyt");
    }

protected:
    static constexpr char DIRECTORY_PATH[] = "tmp";
};

TEST_F(LayoutStorageTest, DeleteLayout)
{
    std::string filePath{"layout/feab1e5f624e40df9965e204c9a94455.lyt"};

    EXPECT_CALL(fileSystem, remove(Eq(filePath))).Times(1).WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = layoutStorage.deleteLayout(id1);

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(LayoutStorageTest, DeleteLayoutError)
{
    EXPECT_CALL(fileSystem, remove).Times(1).WillOnce(Return(fs::FS_ERR_NOENT));

    Ret ret = layoutStorage.deleteLayout(id1);

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(LayoutStorageTest, Clear)
{
    EXPECT_CALL(fileSystem, remove_recursive(Eq(DIRECTORY_PATH)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));

    layoutStorage.clear();
}

TEST_F(LayoutStorageLinuxFileSystemTest, List)
{
    setupDefault();

    std::vector<uuid::Uuid> list;
    ASSERT_TRUE(getList(list));

    std::vector<uuid::Uuid> expectedList{uuid::Uuid(id1), uuid::Uuid(id2)};

    ASSERT_EQ(list.size(), 2);
    for(auto& expectedId: expectedList)
    {
        auto it = etl::find(list.begin(), list.end(), expectedId);
        EXPECT_NE(it, list.end());
    }
}

TEST_F(LayoutStorageLinuxFileSystemTest, EmptyList)
{
    std::vector<uuid::Uuid> list;
    ASSERT_TRUE(getList(list));

    std::vector<uuid::Uuid> expectedList;

    EXPECT_EQ(list, expectedList);
}

TEST_F(LayoutStorageLinuxFileSystemTest, GetLayout)
{
    setupDefault();

    LayoutMetadata expectedMetadata{ScreenDefs::BackgroundColor::WHITE,
                                    ScreenDefs::PixelFormat::_JPEG};

    Layout expectedLayout(id1,
                          expectedMetadata,
                          1,
                          fileSystem,
                          "tmp/feab1e5f624e40df9965e204c9a94455.lyt");

    etl::optional<Layout> layout;
    Ret ret = layoutStorage.getLayout(id1, layout);

    EXPECT_EQ(ret, Ret::SUCCESS);
    ASSERT_TRUE(layout.has_value());
    EXPECT_EQ(layout.value(), expectedLayout);
}

TEST_F(LayoutStorageLinuxFileSystemTest, GetLayoutInvalid)
{
    setupDefault();

    etl::optional<Layout> layout;
    Ret ret = layoutStorage.getLayout(id2, layout);

    EXPECT_EQ(ret, Ret::INVALID_DATA);
    EXPECT_FALSE(layout.has_value());
}

TEST_F(LayoutStorageLinuxFileSystemTest, GetLayoutNotExisting)
{
    etl::optional<Layout> layout;
    Ret ret = layoutStorage.getLayout(id2, layout);

    EXPECT_EQ(ret, Ret::NOT_FOUND);
    EXPECT_FALSE(layout.has_value());
}