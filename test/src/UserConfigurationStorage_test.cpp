#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "UserConfigurationStorage.hpp"
#include "MockFileSystem.hpp"
#include "MockUserConfigurationCodec.hpp"
#include "MockLayoutConfigurationCodec.hpp"
#include "LinuxFileSystem.hpp"
#include "CborUserConfigurationCodec.hpp"
#include "CborLayoutConfigurationCodec.hpp"
#include "SharedBufferContainer.hpp"
#include <fstream>
#include <filesystem>
#include "uuid_helper.hpp"
#include "UuidListIteratorWrapper.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;

using namespace configuration;

template<typename FileSystem, typename UserConfigurationCodec, typename LayoutConfigurationCodec>
class UserConfigurationStorageTestBase : public ::testing::Test
{
public:
    UserConfigurationStorageTestBase(etl::string_view directoryPath)
        : id1(makeUuid("feab1e5f624e40df9965e204c9a94455"))
        , id2(makeUuid("ffab1e5f624e40df9965e204c9a94455"))
        , userConfigurationStorage(fileSystem,
                                   directoryPath,
                                   userConfigurationCodec,
                                   layoutConfigurationCodec,
                                   sharedBufferContainer)
    {
    }

    bool getList(std::vector<uuid::Uuid>& list)
    {
        UuidListIteratorWrapper itWrapper(userConfigurationStorage);

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
    uuid::Uuid id1;
    uuid::Uuid id2;
    FileSystem fileSystem;
    UserConfigurationCodec userConfigurationCodec;
    LayoutConfigurationCodec layoutConfigurationCodec;
    SharedBufferContainer sharedBufferContainer;

    UserConfigurationStorage userConfigurationStorage;
};

class UserConfigurationStorageTest
    : public UserConfigurationStorageTestBase<MockFileSystem,
                                              MockUserConfigurationCodec,
                                              MockLayoutConfigurationCodec>
{
public:
    UserConfigurationStorageTest()
        : UserConfigurationStorageTestBase(DIRECTORY_PATH)
    {
    }

protected:
    static constexpr char DIRECTORY_PATH[] = "ucfg";
};

class UserConfigurationStorageLinuxFileSystemTest
    : public UserConfigurationStorageTestBase<LinuxFileSystem,
                                              CborUserConfigurationCodec,
                                              CborLayoutConfigurationCodec>
{
public:
    UserConfigurationStorageLinuxFileSystemTest()
        : UserConfigurationStorageTestBase(DIRECTORY_PATH)
    {
        std::filesystem::remove_all(DIRECTORY_PATH);
        std::filesystem::create_directory(DIRECTORY_PATH);
    }

    void setupDefault()
    {
        std::filesystem::path inputDir = RESOURCE_FILES_PATH "/UserConfigurationStorageTest/";
        std::filesystem::path outputDir = DIRECTORY_PATH;

        std::filesystem::copy(inputDir / "user_configuration_valid",
                              outputDir / "feab1e5f624e40df9965e204c9a94455",
                              std::filesystem::copy_options::recursive);
        std::filesystem::copy(inputDir / "user_configuration_invalid",
                              outputDir / "ffab1e5f624e40df9965e204c9a94455",
                              std::filesystem::copy_options::recursive);
    }

protected:
    static constexpr char DIRECTORY_PATH[] = "tmp";
};

TEST_F(UserConfigurationStorageTest, DeleteUserConfiguration)
{
    std::string dirPath{"ucfg/feab1e5f624e40df9965e204c9a94455"};

    EXPECT_CALL(fileSystem, remove_recursive(Eq(dirPath))).Times(1).WillOnce(Return(fs::FS_ERR_OK));

    Ret ret = userConfigurationStorage.deleteUserConfiguration(uuid::UuidView(id1));

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationStorageTest, DeleteUserConfigurationError)
{
    EXPECT_CALL(fileSystem, remove_recursive).Times(1).WillOnce(Return(fs::FS_ERR_NOENT));

    Ret ret = userConfigurationStorage.deleteUserConfiguration(uuid::UuidView(id1));

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(UserConfigurationStorageTest, Clear)
{
    EXPECT_CALL(fileSystem, remove_recursive(Eq(DIRECTORY_PATH)))
        .Times(1)
        .WillOnce(Return(fs::FS_ERR_OK));

    userConfigurationStorage.clear();
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, GetUserConfiguration)
{
    setupDefault();

    UserConfiguration expectedUserConfiguration{uuid::UuidView(id1)};
    expectedUserConfiguration.addLayoutConfiguration(
        LayoutConfigurationDescriptor{makeUuid("6c2b7853d113489bb8680e28f1b9e7dc"),
                                      makeUuid("5a24b671e47433bc9f29c3622b3e1a5f"),
                                      makeUuid("e73e13489929d97be8afb5183e850bda")});

    expectedUserConfiguration.addLayoutConfiguration(
        LayoutConfigurationDescriptor{makeUuid("67c52e1ce352feb25f2c577126742aff"),
                                      makeUuid("d0203751e25e979be0c716fc971e136b"),
                                      makeUuid("860a681aadd447c6f4226e0c6df0e09c")});

    etl::optional<UserConfiguration> userConfiguration;
    Ret ret = userConfigurationStorage.getUserConfiguration(uuid::UuidView(id1), userConfiguration);

    EXPECT_EQ(ret, Ret::SUCCESS);
    ASSERT_TRUE(userConfiguration.has_value());
    EXPECT_EQ(userConfiguration, expectedUserConfiguration);
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, GetUserConfigurationNotExisting)
{
    etl::optional<UserConfiguration> userConfiguration;
    Ret ret = userConfigurationStorage.getUserConfiguration(uuid::UuidView(id1), userConfiguration);

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, GetUserConfigurationInvalid)
{
    setupDefault();

    etl::optional<UserConfiguration> userConfiguration;
    Ret ret = userConfigurationStorage.getUserConfiguration(uuid::UuidView(id2), userConfiguration);

    EXPECT_EQ(ret, Ret::INVALID_DATA);
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, List)
{
    setupDefault();
    std::vector<uuid::Uuid> list;
    ASSERT_TRUE(getList(list));

    std::vector<uuid::Uuid> expectedList{uuid::Uuid(id1), uuid::Uuid(id2)};

    ASSERT_EQ(expectedList.size(), 2);
    for(auto& expectedId: expectedList)
    {
        auto it = etl::find(list.begin(), list.end(), expectedId);
        EXPECT_NE(it, list.end());
    }
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, EmptyList)
{
    std::vector<uuid::Uuid> list;
    ASSERT_TRUE(getList(list));

    std::vector<uuid::Uuid> expectedList;

    EXPECT_EQ(list, expectedList);
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, IsExistingUserConfigurationTrue)
{
    setupDefault();

    EXPECT_TRUE(userConfigurationStorage.isExistingUserConfiguration(uuid::UuidView(id1)));
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, IsExistingUserConfigurationFalse)
{
    setupDefault();

    uuid::Uuid wrongId(makeUuid("faab1e5f624e40df9965e204c9a94455"));

    EXPECT_FALSE(userConfigurationStorage.isExistingUserConfiguration(uuid::UuidView(wrongId)));
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, GetLayoutConfiguration)
{
    setupDefault();

    uuid::Uuid layoutConfId(makeUuid("6c2b7853d113489bb8680e28f1b9e7dc"));

    etl::vector<KeySequence, 1> emptyKeySequences;
    LayoutConfiguration expectedLayoutConfiguration(uuid::UuidView(layoutConfId),
                                                    uuid::UuidView(makeUuid(
                                                        "e73e13489929d97be8afb5183e850bda")),
                                                    etl::nullopt,
                                                    OperatingSystem::WINDOWS,
                                                    emptyKeySequences,
                                                    emptyKeySequences);

    etl::optional<LayoutConfiguration> layoutConfiguration;
    Ret ret = userConfigurationStorage.getLayoutConfiguration(uuid::UuidView(id1),
                                                              uuid::UuidView(layoutConfId),
                                                              layoutConfiguration);

    EXPECT_EQ(ret, Ret::SUCCESS);
    ASSERT_TRUE(layoutConfiguration.has_value());
    EXPECT_EQ(layoutConfiguration, expectedLayoutConfiguration);
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, GetLayoutConfigurationNotExisting)
{
    setupDefault();

    uuid::Uuid wrongLayoutConfId(makeUuid("6a2b7853d113489bb8680e28f1b9e7dc"));

    etl::optional<LayoutConfiguration> layoutConfiguration;
    Ret ret = userConfigurationStorage.getLayoutConfiguration(uuid::UuidView(id1),
                                                              uuid::UuidView(wrongLayoutConfId),
                                                              layoutConfiguration);

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, GetLayoutConfigurationWrongUserConfiguration)
{
    setupDefault();

    uuid::Uuid wrongId(makeUuid("faab1e5f624e40df9965e204c9a94455"));
    uuid::Uuid layoutConfId(makeUuid("6c2b7853d113489bb8680e28f1b9e7dc"));

    etl::optional<LayoutConfiguration> layoutConfiguration;
    Ret ret = userConfigurationStorage.getLayoutConfiguration(uuid::UuidView(wrongId),
                                                              uuid::UuidView(layoutConfId),
                                                              layoutConfiguration);

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(UserConfigurationStorageLinuxFileSystemTest, GetLayoutConfigurationInvalid)
{
    setupDefault();

    uuid::Uuid layoutConfId(makeUuid("ae580a9cdbdeeba2c8032d93528edb82"));

    etl::optional<LayoutConfiguration> layoutConfiguration;
    Ret ret = userConfigurationStorage.getLayoutConfiguration(uuid::UuidView(id2),
                                                              uuid::UuidView(layoutConfId),
                                                              layoutConfiguration);

    EXPECT_EQ(ret, Ret::INVALID_DATA);
}