#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ConfigurationSwitcher.hpp"
#include "MockApplication.hpp"
#include "MockUserConfigurationNavigationFactory.hpp"
#include "MockUserConfigurationNavigation.hpp"
#include "MockUserConfigurationStorage.hpp"
#include "MockLayoutStorage.hpp"
#include "ConfigurationChangeNotifier.hpp"
#include "MockConfChangeListener.hpp"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Ref;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;

using namespace configuration;

class ListIterator : public IUuidList::Iterator
{
public:
    ListIterator(const std::vector<uuid::Uuid>& list)
        : m_list(list)
        , m_it(m_list.begin())
    {
    }
    ~ListIterator() final = default;

    etl::optional<uuid::Uuid> next() final
    {
        etl::optional<uuid::Uuid> uuid;
        if(m_it != m_list.end())
        {
            uuid.emplace(*m_it);
            ++m_it;
        }
        return uuid;
    }

private:
    const std::vector<uuid::Uuid>& m_list;
    std::vector<uuid::Uuid>::const_iterator m_it;
};

class ConfigurationSwitcherTest : public ::testing::Test
{
public:
    ConfigurationSwitcherTest()
        : defaultUserConfigurationList(makeDefaultUserConfigurationList())
        , storageContext(userConfigurationStorage, layoutStorage)
        , factoryStorageContext(factoryUserConfigurationStorage, factoryLayoutStorage)
        , confChangeNotifier({&listener1, &listener2})
        , configurationSwitcher(userConfigurationNavigationFactory,
                                application,
                                storageContext,
                                factoryStorageContext,
                                confChangeNotifier)
    {
    }

    std::vector<uuid::Uuid> makeDefaultUserConfigurationList()
    {
        std::vector<uuid::Uuid> list;
        list.push_back(uuid::Uuid(idUserCfg));

        return list;
    }

    void mockGetUserConfigurationList(MockUserConfigurationStorage& storage,
                                      const std::vector<uuid::Uuid>& list)
    {
        auto deleteIteratorFunc = [this](IUuidList::Iterator& it) { delete &it; };
        IUuidList::Iterator* p = new ListIterator(list);
        etl::reference_wrapper<IUuidList::Iterator> itRef(*p);
        etl::optional<etl::reference_wrapper<IUuidList::Iterator>> iterator(itRef);

        EXPECT_CALL(storage, createIterator).Times(1).WillOnce(Return(iterator));
        EXPECT_CALL(storage, deleteIterator).Times(1).WillOnce(Invoke(deleteIteratorFunc));
    }

    void mockUserConfigurationNavigationFactory(uuid::UuidView id,
                                                ConfigurationStorageContext& storageContext)
    {
        EXPECT_CALL(userConfigurationNavigationFactory,
                    createUserConfigurationNavigation(id, Ref(storageContext), _, _))
            .Times(1)
            .WillOnce(ReturnRef(userConfigurationNavigation));
    }

    void initNominal()
    {
        mockGetUserConfigurationList(userConfigurationStorage, defaultUserConfigurationList);
        mockUserConfigurationNavigationFactory(uuid::UuidView(idUserCfg), storageContext);

        EXPECT_CALL(userConfigurationStorage, isExistingUserConfiguration(uuid::UuidView(idUserCfg)))
            .Times(1)
            .WillOnce(Return(true));
        EXPECT_CALL(userConfigurationNavigation, disableCustom)
            .Times(1)
            .WillOnce(Return(Ret::SUCCESS));
        EXPECT_CALL(userConfigurationNavigation, applyFirst).Times(1).WillOnce(Return(Ret::SUCCESS));

        Ret ret = configurationSwitcher.init();

        EXPECT_EQ(ret, Ret::SUCCESS);

        Mock::VerifyAndClearExpectations(&userConfigurationNavigationFactory);
        Mock::VerifyAndClearExpectations(&userConfigurationStorage);
        Mock::VerifyAndClearExpectations(&userConfigurationNavigation);
    }

    void initFactory()
    {
        mockGetUserConfigurationList(userConfigurationStorage, emptyUserConfigurationList);
        mockGetUserConfigurationList(factoryUserConfigurationStorage, defaultUserConfigurationList);
        mockUserConfigurationNavigationFactory(uuid::UuidView(idUserCfg), factoryStorageContext);

        EXPECT_CALL(factoryUserConfigurationStorage,
                    isExistingUserConfiguration(uuid::UuidView(idUserCfg)))
            .Times(1)
            .WillOnce(Return(true));
        EXPECT_CALL(userConfigurationNavigation, disableCustom)
            .Times(1)
            .WillOnce(Return(Ret::SUCCESS));
        EXPECT_CALL(userConfigurationNavigation, applyFirst).Times(1).WillOnce(Return(Ret::STATE));

        Ret ret = configurationSwitcher.init();

        EXPECT_EQ(ret, Ret::STATE);

        Mock::VerifyAndClearExpectations(&userConfigurationNavigationFactory);
        Mock::VerifyAndClearExpectations(&userConfigurationStorage);
        Mock::VerifyAndClearExpectations(&factoryUserConfigurationStorage);
        Mock::VerifyAndClearExpectations(&userConfigurationNavigation);
    }

    void initNoConfig()
    {
        mockGetUserConfigurationList(userConfigurationStorage, emptyUserConfigurationList);
        mockGetUserConfigurationList(factoryUserConfigurationStorage, emptyUserConfigurationList);

        Ret ret = configurationSwitcher.init();

        EXPECT_EQ(ret, Ret::NOT_FOUND);

        Mock::VerifyAndClearExpectations(&userConfigurationNavigationFactory);
        Mock::VerifyAndClearExpectations(&userConfigurationStorage);
        Mock::VerifyAndClearExpectations(&factoryUserConfigurationStorage);
        Mock::VerifyAndClearExpectations(&userConfigurationNavigation);
    }

protected:
    etl::array<std::byte, uuid::SIZE> idUserCfg{std::byte(0x00), std::byte(0x01), std::byte(0x00)};
    std::vector<uuid::Uuid> defaultUserConfigurationList;
    std::vector<uuid::Uuid> emptyUserConfigurationList;

    etl::array<std::byte, uuid::SIZE> idCfg1{std::byte(0x01), std::byte(0x00)};
    etl::array<std::byte, uuid::SIZE> idCfg2{std::byte(0x02), std::byte(0x00)};

    MockUserConfigurationNavigationFactory userConfigurationNavigationFactory;

    MockApplication application;

    MockUserConfigurationStorage userConfigurationStorage;
    MockLayoutStorage layoutStorage;
    ConfigurationStorageContext storageContext;

    NiceMock<MockUserConfigurationStorage> factoryUserConfigurationStorage;
    NiceMock<MockLayoutStorage> factoryLayoutStorage;
    ConfigurationStorageContext factoryStorageContext;

    MockConfChangeListener listener1, listener2;
    ConfigurationChangeNotifier<2> confChangeNotifier;

    StrictMock<MockUserConfigurationNavigation> userConfigurationNavigation;

    ConfigurationSwitcher configurationSwitcher;
};

TEST_F(ConfigurationSwitcherTest, InitNominal)
{
    initNominal();
}

TEST_F(ConfigurationSwitcherTest, InitFactory)
{
    initFactory();
}

TEST_F(ConfigurationSwitcherTest, InitNoConfig)
{
    initNoConfig();
}

TEST_F(ConfigurationSwitcherTest, InitFail)
{
    mockGetUserConfigurationList(userConfigurationStorage, emptyUserConfigurationList);
    mockGetUserConfigurationList(factoryUserConfigurationStorage, defaultUserConfigurationList);
    mockUserConfigurationNavigationFactory(uuid::UuidView(idUserCfg), factoryStorageContext);

    EXPECT_CALL(factoryUserConfigurationStorage,
                isExistingUserConfiguration(uuid::UuidView(idUserCfg)))
        .Times(1)
        .WillOnce(Return(true));
    EXPECT_CALL(userConfigurationNavigation, disableCustom).Times(1).WillOnce(Return(Ret::SUCCESS));
    EXPECT_CALL(userConfigurationNavigation, applyFirst).Times(1).WillOnce(Return(Ret::NOT_FOUND));

    Ret ret = configurationSwitcher.init();

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(ConfigurationSwitcherTest, InitIteratorFail)
{
    EXPECT_CALL(userConfigurationStorage, createIterator).Times(1).WillOnce(Return(etl::nullopt));
    EXPECT_CALL(factoryUserConfigurationStorage, createIterator)
        .Times(1)
        .WillOnce(Return(etl::nullopt));

    Ret ret = configurationSwitcher.init();

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}


TEST_F(ConfigurationSwitcherTest, SelectLayoutConfigurationNominal)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation, applyId(uuid::UuidView(idCfg1)))
        .Times(1)
        .WillOnce(Return(Ret::SUCCESS));

    Ret ret = configurationSwitcher.selectLayoutConfiguration(uuid::UuidView(idCfg1));

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(ConfigurationSwitcherTest, SelectLayoutConfigurationError)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation, applyId(uuid::UuidView(idCfg1)))
        .Times(1)
        .WillOnce(Return(Ret::NOT_FOUND));

    Ret ret = configurationSwitcher.selectLayoutConfiguration(uuid::UuidView(idCfg1));

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(ConfigurationSwitcherTest, SelectLayoutConfigurationNoConfig)
{
    initNoConfig();

    Ret ret = configurationSwitcher.selectLayoutConfiguration(uuid::UuidView(idCfg1));

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(ConfigurationSwitcherTest, SelectUserConfigurationNominal)
{
    initNominal();

    mockUserConfigurationNavigationFactory(uuid::UuidView(idUserCfg), storageContext);

    EXPECT_CALL(userConfigurationStorage, isExistingUserConfiguration(uuid::UuidView(idUserCfg)))
        .Times(1)
        .WillOnce(Return(true));

    EXPECT_CALL(userConfigurationNavigation, applyId(uuid::UuidView(idCfg2)))
        .Times(1)
        .WillOnce(Return(Ret::SUCCESS));
    EXPECT_CALL(userConfigurationNavigation, disableCustom).Times(1).WillOnce(Return(Ret::SUCCESS));

    Ret ret = configurationSwitcher.selectUserConfiguration(uuid::UuidView(idUserCfg),
                                                            uuid::UuidView(idCfg2));

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(ConfigurationSwitcherTest, SelectUserConfigurationNotExistingLayoutConfiguration)
{
    initNominal();

    mockUserConfigurationNavigationFactory(uuid::UuidView(idUserCfg), storageContext);

    EXPECT_CALL(userConfigurationStorage, isExistingUserConfiguration(uuid::UuidView(idUserCfg)))
        .Times(1)
        .WillOnce(Return(true));

    EXPECT_CALL(userConfigurationNavigation, applyId(uuid::UuidView(idCfg2)))
        .Times(1)
        .WillOnce(Return(Ret::NOT_FOUND));
    EXPECT_CALL(userConfigurationNavigation, disableCustom).Times(1).WillOnce(Return(Ret::SUCCESS));
    EXPECT_CALL(userConfigurationNavigation, applyFirst).Times(1).WillOnce(Return(Ret::SUCCESS));

    Ret ret = configurationSwitcher.selectUserConfiguration(uuid::UuidView(idUserCfg),
                                                            uuid::UuidView(idCfg2));

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(ConfigurationSwitcherTest, SelectUserConfigurationNotExisting)
{
    initNominal();

    EXPECT_CALL(userConfigurationStorage, isExistingUserConfiguration(uuid::UuidView(idUserCfg)))
        .Times(1)
        .WillOnce(Return(false));

    Ret ret = configurationSwitcher.selectUserConfiguration(uuid::UuidView(idUserCfg),
                                                            uuid::UuidView(idCfg2));

    EXPECT_EQ(ret, Ret::NOT_FOUND);
}

TEST_F(ConfigurationSwitcherTest, KeyProtocolModeChanged)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation, enableCustom).Times(1).WillOnce(Return(Ret::SUCCESS));
    configurationSwitcher.keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::CUSTOM);

    Mock::VerifyAndClearExpectations(&userConfigurationNavigation);

    EXPECT_CALL(userConfigurationNavigation, disableCustom).Times(1);
    configurationSwitcher.keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::HID);
}

TEST_F(ConfigurationSwitcherTest, KeyProtocolModeChangedNoConfig)
{
    initNoConfig();

    configurationSwitcher.keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::CUSTOM);
}

TEST_F(ConfigurationSwitcherTest, SelectUserConfigurationInCustomMode)
{
    initNominal();

    mockUserConfigurationNavigationFactory(uuid::UuidView(idUserCfg), storageContext);

    EXPECT_CALL(userConfigurationNavigation, enableCustom).Times(1).WillOnce(Return(Ret::SUCCESS));
    configurationSwitcher.keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::CUSTOM);
    Mock::VerifyAndClearExpectations(&userConfigurationNavigation);

    EXPECT_CALL(userConfigurationStorage, isExistingUserConfiguration(uuid::UuidView(idUserCfg)))
        .Times(1)
        .WillOnce(Return(true));

    EXPECT_CALL(userConfigurationNavigation, applyId(uuid::UuidView(idCfg2)))
        .Times(1)
        .WillOnce(Return(Ret::SUCCESS));
    EXPECT_CALL(userConfigurationNavigation, enableCustom).Times(1).WillOnce(Return(Ret::SUCCESS));

    Ret ret = configurationSwitcher.selectUserConfiguration(uuid::UuidView(idUserCfg),
                                                            uuid::UuidView(idCfg2));

    EXPECT_EQ(ret, Ret::SUCCESS);
}

TEST_F(ConfigurationSwitcherTest, Click)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation,
                applyNext(IUserConfigurationNavigation::SwitchDirection::FORWARD))
        .Times(1);
    configurationSwitcher.simpleClicked(ButtonID(BTN_DOWN));
}

TEST_F(ConfigurationSwitcherTest, ClickNoConfig)
{
    initNoConfig();

    configurationSwitcher.simpleClicked(ButtonID(BTN_DOWN));
}

TEST_F(ConfigurationSwitcherTest, ClickOtherButton)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation, applyNext).Times(0);
    configurationSwitcher.simpleClicked(ButtonID(BTN_UP));
}

TEST_F(ConfigurationSwitcherTest, DoubleClick)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation,
                applyNext(IUserConfigurationNavigation::SwitchDirection::BACKWARD))
        .Times(1);
    configurationSwitcher.doubleClicked(ButtonID(BTN_DOWN));
}

TEST_F(ConfigurationSwitcherTest, DoubleClickOtherButton)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation, applyNext).Times(0);
    configurationSwitcher.doubleClicked(ButtonID(BTN_UP));
}

TEST_F(ConfigurationSwitcherTest, SetNextConfiguration)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation,
                applyNext(IUserConfigurationNavigation::SwitchDirection::FORWARD))
        .Times(1);
    configurationSwitcher.setNextConfiguration();
}

TEST_F(ConfigurationSwitcherTest, CurrentConfigurationDeleted)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation, getCurrentUserConfigurationId())
        .Times(1)
        .WillOnce(Return(uuid::UuidView(idUserCfg)));

    mockGetUserConfigurationList(userConfigurationStorage, defaultUserConfigurationList);
    mockUserConfigurationNavigationFactory(uuid::UuidView(idUserCfg), storageContext);

    EXPECT_CALL(userConfigurationStorage, isExistingUserConfiguration(uuid::UuidView(idUserCfg)))
        .Times(1)
        .WillOnce(Return(true));
    EXPECT_CALL(userConfigurationNavigation, disableCustom).Times(1).WillOnce(Return(Ret::SUCCESS));
    EXPECT_CALL(userConfigurationNavigation, applyFirst).Times(1).WillOnce(Return(Ret::SUCCESS));

    configurationSwitcher.notification(UserConfigurationDeleteEvent{uuid::UuidView(idUserCfg)});
}

TEST_F(ConfigurationSwitcherTest, CurrentConfigurationUpdated)
{
    initNominal();

    EXPECT_CALL(userConfigurationNavigation, getCurrentUserConfigurationId())
        .Times(1)
        .WillOnce(Return(uuid::UuidView(idUserCfg)));

    mockGetUserConfigurationList(userConfigurationStorage, defaultUserConfigurationList);
    mockUserConfigurationNavigationFactory(uuid::UuidView(idUserCfg), storageContext);

    EXPECT_CALL(userConfigurationStorage, isExistingUserConfiguration(uuid::UuidView(idUserCfg)))
        .Times(1)
        .WillOnce(Return(true));
    EXPECT_CALL(userConfigurationNavigation, disableCustom).Times(1).WillOnce(Return(Ret::SUCCESS));
    EXPECT_CALL(userConfigurationNavigation, applyFirst).Times(1).WillOnce(Return(Ret::SUCCESS));

    configurationSwitcher.notification(UserConfigurationUpdateEvent{uuid::UuidView(idUserCfg)});
}

TEST_F(ConfigurationSwitcherTest, OtherConfigurationDeleted)
{
    etl::array<std::byte, uuid::SIZE> idOtherUserCfg{std::byte(0x00),
                                                     std::byte(0x02),
                                                     std::byte(0x00)};

    initNominal();

    EXPECT_CALL(userConfigurationNavigation, getCurrentUserConfigurationId())
        .Times(1)
        .WillOnce(Return(uuid::UuidView(idUserCfg)));

    EXPECT_CALL(userConfigurationNavigation, applyFirst()).Times(0);

    configurationSwitcher.notification(UserConfigurationDeleteEvent{uuid::UuidView(idOtherUserCfg)});
}
