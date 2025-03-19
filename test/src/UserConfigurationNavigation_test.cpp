#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "UserConfigurationNavigation.hpp"
#include "MockApplication.hpp"
#include "MockConfChangeListener.hpp"
#include "ConfigurationChangeNotifier.hpp"
#include "MockUserConfigurationStorage.hpp"
#include "MockLayoutStorage.hpp"

using ::testing::_;
using ::testing::AtMost;
using ::testing::DoAll;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Ref;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SetArgReferee;
using ::testing::StrictMock;

using namespace configuration;

class UserConfigurationNavigationTest : public ::testing::Test
{
public:
    UserConfigurationNavigationTest()
        : id(etl::array<std::byte, uuid::SIZE>{std::byte(0x00), std::byte(0x01), std::byte(0x00)})
        , layoutConfigs(makeLayoutConfigs())
        , confChangeNotifier({&listener1, &listener2})
        , storageContext(userConfigurationStorage, layoutStorage)
    {
    }

    UserConfiguration makeUserconfiguration()
    {
        uuid::Uuid zeroIdUuid(zeroId);

        UserConfiguration ret(uuid::UuidView{id});
        ret.addLayoutConfiguration(
            LayoutConfigurationDescriptor{uuid::Uuid(idCfg1), zeroIdUuid, zeroIdUuid});
        ret.addLayoutConfiguration(
            LayoutConfigurationDescriptor{uuid::Uuid(idCfg2), zeroIdUuid, zeroIdUuid});
        ret.addLayoutConfiguration(
            LayoutConfigurationDescriptor{uuid::Uuid(idCfg3), zeroIdUuid, zeroIdUuid});
        ret.addLayoutConfiguration(
            LayoutConfigurationDescriptor{uuid::Uuid(idCfg4), zeroIdUuid, zeroIdUuid});

        return ret;
    }

    etl::array<LayoutConfiguration, 4> makeLayoutConfigs()
    {
        uuid::UuidView zeroIdView(zeroId);
        etl::vector<configuration::KeySequence, 1> defaultKeySequence;

        etl::array<LayoutConfiguration, 4>
            layoutConfigs{LayoutConfiguration(uuid::UuidView(idCfg1),
                                              zeroIdView,
                                              etl::nullopt,
                                              OperatingSystem::WINDOWS,
                                              defaultKeySequence,
                                              defaultKeySequence),
                          LayoutConfiguration(uuid::UuidView(idCfg2),
                                              zeroIdView,
                                              etl::nullopt,
                                              OperatingSystem::WINDOWS,
                                              defaultKeySequence,
                                              defaultKeySequence),
                          // Custom configuration
                          LayoutConfiguration(uuid::UuidView(idCfg3),
                                              zeroIdView,
                                              uuid::UuidView(idCfg4),
                                              OperatingSystem::WINDOWS,
                                              defaultKeySequence,
                                              defaultKeySequence),
                          LayoutConfiguration(uuid::UuidView(idCfg4),
                                              zeroIdView,
                                              etl::nullopt,
                                              OperatingSystem::WINDOWS,
                                              defaultKeySequence,
                                              defaultKeySequence)};

        return layoutConfigs;
    }

    UserConfigurationNavigation makeUserConfigurationNavigation(UserConfiguration& userConfiguration)
    {
        EXPECT_CALL(userConfigurationStorage, getUserConfiguration(uuid::UuidView(id), _))
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<1>(userConfiguration), Return(Ret::SUCCESS)));

        return UserConfigurationNavigation(uuid::UuidView(id),
                                           storageContext,
                                           application,
                                           confChangeNotifier);
    }

    void mockGetLayoutConfiguration(LayoutConfiguration& layoutConfiguration, Ret ret = Ret::SUCCESS)
    {
        EXPECT_CALL(userConfigurationStorage,
                    getLayoutConfiguration(uuid::UuidView(id), layoutConfiguration.getId(), _))
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<2>(ret == Ret::SUCCESS
                                                 ? layoutConfiguration
                                                 : etl::optional<LayoutConfiguration>(etl::nullopt)),
                            Return(ret)));
    }

    void mockApplyConfiguration(LayoutConfiguration& layoutConfiguration, bool ret = true)
    {
        EXPECT_CALL(application, applyLayoutConfiguration(Ref(layoutStorage), layoutConfiguration))
            .Times(1)
            .WillOnce(Return(ret));
    }

protected:
    uuid::Uuid id;

    etl::array<std::byte, uuid::SIZE> zeroId{std::byte(0)};

    etl::array<std::byte, uuid::SIZE> idCfg1{std::byte(0x01), std::byte(0x00)};
    etl::array<std::byte, uuid::SIZE> idCfg2{std::byte(0x02), std::byte(0x00)};
    etl::array<std::byte, uuid::SIZE> idCfg3{std::byte(0x03), std::byte(0x00)};
    etl::array<std::byte, uuid::SIZE> idCfg4{std::byte(0x04), std::byte(0x00)};

    etl::array<LayoutConfiguration, 4> layoutConfigs;

    MockApplication application;

    NiceMock<MockConfChangeListener> listener1, listener2;
    ConfigurationChangeNotifier<2> confChangeNotifier;

    StrictMock<MockUserConfigurationStorage> userConfigurationStorage;
    MockLayoutStorage layoutStorage;
    ConfigurationStorageContext storageContext;
};

TEST_F(UserConfigurationNavigationTest, ApplyIdHid)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[1];

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    Ret ret = navigation.applyId(layoutConfiguration.getId());

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationNavigationTest, ApplyWrongId)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    etl::array<std::byte, uuid::SIZE> idCfg6{std::byte(0x06)};

    Ret ret = navigation.applyId(uuid::UuidView(idCfg6));

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(UserConfigurationNavigationTest, ApplyIdNotFoundInStorage)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[1];

    mockGetLayoutConfiguration(layoutConfiguration, Ret::NOT_FOUND);

    Ret ret = navigation.applyId(layoutConfigs[1].getId());

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(UserConfigurationNavigationTest, ApplyIdOtherFsErr)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[1];

    mockGetLayoutConfiguration(layoutConfiguration, Ret::FS);

    Ret ret = navigation.applyId(layoutConfigs[1].getId());

    EXPECT_EQ(ret, Ret::FS);
}

TEST_F(UserConfigurationNavigationTest, ApplyIdApplyErr)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[1];

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration, false);

    Ret ret = navigation.applyId(layoutConfigs[1].getId());

    EXPECT_EQ(ret, Ret::UNEXPECTED);
}

TEST_F(UserConfigurationNavigationTest, ApplyIdCustomInHidMode)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[2];

    mockGetLayoutConfiguration(layoutConfiguration);

    Ret ret = navigation.applyId(layoutConfiguration.getId());

    EXPECT_EQ(ret, Ret::STATE);
}

TEST_F(UserConfigurationNavigationTest, ApplyIdCustom)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[2];

    navigation.enableCustom();

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    Ret ret = navigation.applyId(layoutConfiguration.getId());

    EXPECT_EQ(ret, Ret::SUCCESS);

    Mock::VerifyAndClearExpectations(&application);
    Mock::VerifyAndClearExpectations(&userConfigurationStorage);

    layoutConfiguration = layoutConfigs[3];

    mockGetLayoutConfiguration(layoutConfiguration, Ret::SUCCESS);
    mockApplyConfiguration(layoutConfiguration);

    navigation.disableCustom();
}

TEST_F(UserConfigurationNavigationTest, ApplyCycle)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& firstLayoutConfiguration = layoutConfigs[0];

    mockGetLayoutConfiguration(firstLayoutConfiguration);
    mockApplyConfiguration(firstLayoutConfiguration);

    navigation.enableCustom();

    Ret ret = navigation.applyFirst();

    EXPECT_EQ(ret, Ret::SUCCESS);

    Mock::VerifyAndClearExpectations(&application);
    Mock::VerifyAndClearExpectations(&userConfigurationStorage);

    for(size_t i = 1; i < layoutConfigs.size(); ++i)
    {
        auto& layoutConfiguration = layoutConfigs[i];

        mockGetLayoutConfiguration(layoutConfiguration);
        mockApplyConfiguration(layoutConfiguration);

        ret = navigation.applyNext(UserConfigurationNavigation::SwitchDirection::FORWARD);

        EXPECT_EQ(ret, Ret::SUCCESS);

        Mock::VerifyAndClearExpectations(&application);
        Mock::VerifyAndClearExpectations(&userConfigurationStorage);
    }

    mockGetLayoutConfiguration(firstLayoutConfiguration);
    mockApplyConfiguration(firstLayoutConfiguration);

    ret = navigation.applyNext(UserConfigurationNavigation::SwitchDirection::FORWARD);
}

TEST_F(UserConfigurationNavigationTest, ApplyBackward)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[0];

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    Ret ret = navigation.applyFirst();

    EXPECT_EQ(ret, Ret::SUCCESS);

    Mock::VerifyAndClearExpectations(&application);
    Mock::VerifyAndClearExpectations(&userConfigurationStorage);

    layoutConfiguration = layoutConfigs[3];

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    ret = navigation.applyNext(UserConfigurationNavigation::SwitchDirection::BACKWARD);
}

TEST_F(UserConfigurationNavigationTest, ApplyNextAfterId)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[1];

    navigation.enableCustom();

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    Ret ret = navigation.applyId(layoutConfiguration.getId());

    EXPECT_EQ(ret, Ret::SUCCESS);

    Mock::VerifyAndClearExpectations(&application);
    Mock::VerifyAndClearExpectations(&userConfigurationStorage);

    layoutConfiguration = layoutConfigs[2];

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    ret = navigation.applyNext(UserConfigurationNavigation::SwitchDirection::FORWARD);

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationNavigationTest, ApplyNextNotFoundInStorage)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[0];

    navigation.enableCustom();

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    Ret ret = navigation.applyFirst();

    EXPECT_EQ(ret, Ret::SUCCESS);

    Mock::VerifyAndClearExpectations(&application);
    Mock::VerifyAndClearExpectations(&userConfigurationStorage);

    auto& layoutConfiguration1 = layoutConfigs[1];
    auto& layoutConfiguration2 = layoutConfigs[2];

    mockGetLayoutConfiguration(layoutConfiguration1, Ret::NOT_FOUND);

    mockGetLayoutConfiguration(layoutConfiguration2);
    mockApplyConfiguration(layoutConfiguration2);

    ret = navigation.applyNext(UserConfigurationNavigation::SwitchDirection::FORWARD);

    EXPECT_EQ(ret, Ret::SUCCESS);

    Mock::VerifyAndClearExpectations(&application);
    Mock::VerifyAndClearExpectations(&userConfigurationStorage);

    mockGetLayoutConfiguration(layoutConfiguration1, Ret::NOT_FOUND);

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    ret = navigation.applyNext(UserConfigurationNavigation::SwitchDirection::BACKWARD);

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationNavigationTest, ApplyNextApplyError)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[0];

    navigation.enableCustom();

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    Ret ret = navigation.applyFirst();

    EXPECT_EQ(ret, Ret::SUCCESS);

    Mock::VerifyAndClearExpectations(&application);
    Mock::VerifyAndClearExpectations(&userConfigurationStorage);

    auto& layoutConfiguration1 = layoutConfigs[1];
    auto& layoutConfiguration2 = layoutConfigs[2];

    mockGetLayoutConfiguration(layoutConfiguration1);
    mockApplyConfiguration(layoutConfiguration1, false);

    mockGetLayoutConfiguration(layoutConfiguration2);
    mockApplyConfiguration(layoutConfiguration2);

    ret = navigation.applyNext(UserConfigurationNavigation::SwitchDirection::FORWARD);

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationNavigationTest, ApplyNextCustomInHidMode)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[1];

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    Ret ret = navigation.applyId(layoutConfiguration.getId());

    EXPECT_EQ(ret, Ret::SUCCESS);

    Mock::VerifyAndClearExpectations(&application);
    Mock::VerifyAndClearExpectations(&userConfigurationStorage);

    auto customLayoutConfiguration = layoutConfigs[2];
    layoutConfiguration = layoutConfigs[3];

    mockGetLayoutConfiguration(customLayoutConfiguration);

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    ret = navigation.applyNext(UserConfigurationNavigation::SwitchDirection::FORWARD);

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(UserConfigurationNavigationTest, ApplyFirstAllApplyFail)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    // It is allowed to retry for the first configuration
    auto firstLayoutConfiguration = layoutConfigs[0];
    EXPECT_CALL(userConfigurationStorage,
                getLayoutConfiguration(uuid::UuidView(id), firstLayoutConfiguration.getId(), _))
        .Times(AtMost(2))
        .WillRepeatedly(DoAll(SetArgReferee<2>(firstLayoutConfiguration), Return(Ret::SUCCESS)));

    for(size_t i = 1; i < layoutConfigs.size(); ++i)
    {
        auto& layoutConfiguration = layoutConfigs[i];
        EXPECT_CALL(userConfigurationStorage,
                    getLayoutConfiguration(uuid::UuidView(id), layoutConfiguration.getId(), _))
            .Times(1)
            .WillOnce(DoAll(SetArgReferee<2>(layoutConfiguration), Return(Ret::SUCCESS)));
    }

    EXPECT_CALL(application, applyLayoutConfiguration(_, _)).WillRepeatedly(Return(false));

    Ret ret = navigation.applyFirst();

    EXPECT_EQ(ret, Ret::UNEXPECTED);
}

TEST_F(UserConfigurationNavigationTest, ApplyFirstEmptyList)
{
    UserConfiguration userConfiguration(uuid::UuidView{id});
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    Ret ret = navigation.applyFirst();

    EXPECT_EQ(ret, Ret::UNEXPECTED);
}

TEST_F(UserConfigurationNavigationTest, Notification)
{
    auto userConfiguration = makeUserconfiguration();
    auto navigation = makeUserConfigurationNavigation(userConfiguration);

    auto& layoutConfiguration = layoutConfigs[1];

    mockGetLayoutConfiguration(layoutConfiguration);
    mockApplyConfiguration(layoutConfiguration);

    const LayoutConfiguration *pLayoutConfiguration1 = nullptr, *pLayoutConfiguration2 = nullptr;
    EXPECT_CALL(listener1, configurationChanged)
        .Times(1)
        .WillOnce(SaveArg<0>(&pLayoutConfiguration1));
    EXPECT_CALL(listener2, configurationChanged)
        .Times(1)
        .WillOnce(SaveArg<0>(&pLayoutConfiguration2));

    Ret ret = navigation.applyId(layoutConfiguration.getId());

    EXPECT_EQ(ret, Ret::SUCCESS);
    ASSERT_NE(pLayoutConfiguration1, nullptr);
    ASSERT_NE(pLayoutConfiguration2, nullptr);
    EXPECT_EQ(layoutConfiguration, *pLayoutConfiguration1);
    EXPECT_EQ(layoutConfiguration, *pLayoutConfiguration2);
}