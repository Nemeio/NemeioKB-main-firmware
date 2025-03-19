#include "gtest/gtest.h"

#include "KeyMessageDispatcher.hpp"
#include "DummyRecursiveMutex.hpp"
#include "MockKeyMessageHandler.hpp"

using ::testing::InSequence;
using ::testing::Mock;
using ::testing::Return;
using ::testing::StrictMock;

class KeyMessageDispatcherTest : public ::testing::Test
{
protected:
    KeyMessageDispatcherTest()
        : keyMessageDispatcher(mutex)
        , cfgHID(uuid::UuidView{defaultUuid1},
                 uuid::UuidView{defaultUuid1},
                 etl::nullopt,
                 configuration::OperatingSystem::WINDOWS,
                 defaultKeySequence,
                 defaultKeySequence)
        , cfgHID2(uuid::UuidView{defaultUuid2},
                  uuid::UuidView{defaultUuid1},
                  etl::nullopt,
                  configuration::OperatingSystem::WINDOWS,
                  defaultKeySequence,
                  defaultKeySequence)
        , cfgCustom(uuid::UuidView{defaultUuid3},
                    uuid::UuidView{defaultUuid1},
                    uuid::UuidView{defaultUuid1},
                    configuration::OperatingSystem::WINDOWS,
                    defaultKeySequence,
                    defaultKeySequence)
    {
        keyMessageDispatcher.registerHIDKeyMessageHandler(hidKeyMessageHandler);
        keyMessageDispatcher.registerCustomProtocolKeyMessageHandler(customKeyMessageHandler);
        keyMessageDispatcher.registerPairingKeyMessageHandler(pairingKeyMessageHandler);
    };

    void keyHandlerStrategyTest(const Configuration& config,
                                keyprotocoldefs::KeyProtocolMode mode,
                                MockKeyMessageHandler& keyMessageHandler)
    {
        keyMessageDispatcher.configurationChanged(&config);
        keyMessageDispatcher.keyProtocolModeChanged(mode);

        EXPECT_CALL(keyMessageHandler, pressedKeyWithConfig(button, config)).Times(1);
        keyMessageDispatcher.pressedKey(button);
        Mock::VerifyAndClearExpectations(&keyMessageHandler);

        EXPECT_CALL(keyMessageHandler, unpressedKeyWithConfig(button, config)).Times(1);
        keyMessageDispatcher.unpressedKey(button);
        Mock::VerifyAndClearExpectations(&keyMessageHandler);

        EXPECT_CALL(keyMessageHandler, terminateUpdateKeysWithConfig(config)).Times(1);
        keyMessageDispatcher.terminateUpdateKeys();
        Mock::VerifyAndClearExpectations(&keyMessageHandler);
    }

    DummyRecursiveMutex mutex;
    KeyMessageDispatcher keyMessageDispatcher;
    StrictMock<MockKeyMessageHandler> hidKeyMessageHandler;
    StrictMock<MockKeyMessageHandler> customKeyMessageHandler;
    StrictMock<MockKeyMessageHandler> pairingKeyMessageHandler;

    ButtonID button{1, 2};
    uuid::Uuid defaultUuid1{etl::array<std::byte, uuid::SIZE>{std::byte(0x00)}};
    uuid::Uuid defaultUuid2{etl::array<std::byte, uuid::SIZE>{std::byte(0x01), std::byte(0x00)}};
    uuid::Uuid defaultUuid3{etl::array<std::byte, uuid::SIZE>{std::byte(0x02), std::byte(0x00)}};
    etl::vector<configuration::KeySequence, 1> defaultKeySequence;
    configuration::LayoutConfiguration cfgHID;
    configuration::LayoutConfiguration cfgHID2;
    configuration::LayoutConfiguration cfgCustom;
};

TEST_F(KeyMessageDispatcherTest, Init)
{
    keyMessageDispatcher.pressedKey(button);
    keyMessageDispatcher.unpressedKey(button);
    keyMessageDispatcher.terminateUpdateKeys();
}

TEST_F(KeyMessageDispatcherTest, NoModeSet)
{
    keyMessageDispatcher.configurationChanged(&cfgHID);

    keyMessageDispatcher.pressedKey(button);
    keyMessageDispatcher.unpressedKey(button);
    keyMessageDispatcher.terminateUpdateKeys();
}

TEST_F(KeyMessageDispatcherTest, NoConfigSet)
{
    keyMessageDispatcher.keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::HID);

    keyMessageDispatcher.pressedKey(button);
    keyMessageDispatcher.unpressedKey(button);
    keyMessageDispatcher.terminateUpdateKeys();
}

TEST_F(KeyMessageDispatcherTest, HIDModeHIDConfig)
{
    keyHandlerStrategyTest(cfgHID, keyprotocoldefs::KeyProtocolMode::HID, hidKeyMessageHandler);
}

TEST_F(KeyMessageDispatcherTest, HIDModeCustomConfig)
{
    keyHandlerStrategyTest(cfgCustom, keyprotocoldefs::KeyProtocolMode::HID, hidKeyMessageHandler);
}

TEST_F(KeyMessageDispatcherTest, CustomModeHIDConfig)
{
    keyHandlerStrategyTest(cfgHID, keyprotocoldefs::KeyProtocolMode::CUSTOM, hidKeyMessageHandler);
}

TEST_F(KeyMessageDispatcherTest, CustomModeCustomConfig)
{
    keyHandlerStrategyTest(cfgCustom,
                           keyprotocoldefs::KeyProtocolMode::CUSTOM,
                           customKeyMessageHandler);
}

TEST_F(KeyMessageDispatcherTest, PairingModeHIDConfig)
{
    keyHandlerStrategyTest(cfgHID,
                           keyprotocoldefs::KeyProtocolMode::PAIRING,
                           pairingKeyMessageHandler);
}

TEST_F(KeyMessageDispatcherTest, PairingModeCustomConfig)
{
    keyHandlerStrategyTest(cfgCustom,
                           keyprotocoldefs::KeyProtocolMode::PAIRING,
                           pairingKeyMessageHandler);
}

TEST_F(KeyMessageDispatcherTest, ConfigChange)
{
    // Previous configuration must be properly reset before configuration change

    keyMessageDispatcher.configurationChanged(&cfgHID);
    keyMessageDispatcher.keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::CUSTOM);

    {
        InSequence s;

        EXPECT_CALL(hidKeyMessageHandler, resetKeysWithConfig(cfgHID)).Times(1);
        EXPECT_CALL(hidKeyMessageHandler, terminateUpdateKeysWithConfig(cfgHID)).Times(1);

        keyMessageDispatcher.configurationChanged(&cfgHID2);
    }

    Mock::VerifyAndClearExpectations(&hidKeyMessageHandler);

    {
        InSequence s;

        EXPECT_CALL(hidKeyMessageHandler, resetKeysWithConfig(cfgHID2)).Times(1);
        EXPECT_CALL(hidKeyMessageHandler, terminateUpdateKeysWithConfig(cfgHID2)).Times(1);

        keyMessageDispatcher.configurationChanged(&cfgCustom);
    }
}

TEST_F(KeyMessageDispatcherTest, ModeChange)
{
    // Current configuration must be properly reset before mode change

    keyMessageDispatcher.configurationChanged(&cfgHID);
    keyMessageDispatcher.keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::HID);

    {
        InSequence s;

        EXPECT_CALL(hidKeyMessageHandler, resetKeysWithConfig(cfgHID)).Times(1);
        EXPECT_CALL(hidKeyMessageHandler, terminateUpdateKeysWithConfig(cfgHID)).Times(1);

        keyMessageDispatcher.keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::PAIRING);
    }
}
