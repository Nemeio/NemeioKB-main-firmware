#include "gtest/gtest.h"

#include "KeyProtocolManager.hpp"
#include "DummyRecursiveMutex.hpp"
#include "MockKeyProtocolListener.hpp"

using ::testing::NiceMock;
using ::testing::Return;

class KeyProtocolManagerTest : public ::testing::Test
{
protected:
    KeyProtocolManagerTest()
        : keyProtocolManager(mutex)
    {
        keyProtocolManager.registerKeyProtocolListener(keyProtocolListener);
    };

    DummyRecursiveMutex mutex;
    KeyProtocolManager keyProtocolManager;
    NiceMock<MockKeyProtocolListener> keyProtocolListener;
};

TEST_F(KeyProtocolManagerTest, Init)
{
    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::HID))
        .Times(1);
    keyProtocolManager.init();
}

TEST_F(KeyProtocolManagerTest, KeepAlive)
{
    keyProtocolManager.init();

    EXPECT_CALL(keyProtocolListener,
                keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::CUSTOM))
        .Times(1);
    keyProtocolManager.keepAlive();
    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged).Times(0);
    keyProtocolManager.keepAlive();
    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::HID))
        .Times(1);
    keyProtocolManager.keepAliveTimeout();
}

TEST_F(KeyProtocolManagerTest, SetProtocolModeInvalidValue)
{
    keyProtocolManager.init();

    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged).Times(0);
    keyProtocolManager.setProtocolMode(keyprotocoldefs::KeyProtocolMode::PAIRING);
    keyProtocolManager.setProtocolMode(keyprotocoldefs::KeyProtocolMode::UNDEFINED);
}

TEST_F(KeyProtocolManagerTest, SetProtocolModeInvalidState)
{
    keyProtocolManager.init();

    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged).Times(0);
    keyProtocolManager.setProtocolMode(keyprotocoldefs::KeyProtocolMode::CUSTOM);
}

TEST_F(KeyProtocolManagerTest, SetProtocolMode)
{
    keyProtocolManager.init();

    keyProtocolManager.keepAlive();

    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged).Times(0);
    keyProtocolManager.setProtocolMode(keyprotocoldefs::KeyProtocolMode::CUSTOM);
    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::HID))
        .Times(1);
    keyProtocolManager.setProtocolMode(keyprotocoldefs::KeyProtocolMode::HID);
    EXPECT_CALL(keyProtocolListener,
                keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::CUSTOM))
        .Times(1);
    keyProtocolManager.setProtocolMode(keyprotocoldefs::KeyProtocolMode::CUSTOM);

    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged).Times(0);
    keyProtocolManager.keepAlive();
}

TEST_F(KeyProtocolManagerTest, PairingDisconnected)
{
    std::string passkeyStr = "012345";
    BLEDefs::Passkey passkey(passkeyStr.begin(), passkeyStr.end());

    keyProtocolManager.init();

    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged).Times(0);
    keyProtocolManager.pairingStop();

    EXPECT_CALL(keyProtocolListener,
                keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::PAIRING))
        .Times(1);
    keyProtocolManager.pairingStart(passkey);
    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::HID))
        .Times(1);
    keyProtocolManager.pairingStop();
}

TEST_F(KeyProtocolManagerTest, PairingConnected)
{
    std::string passkeyStr = "012345";
    BLEDefs::Passkey passkey(passkeyStr.begin(), passkeyStr.end());

    keyProtocolManager.init();

    keyProtocolManager.keepAlive();

    EXPECT_CALL(keyProtocolListener,
                keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::PAIRING))
        .Times(1);
    keyProtocolManager.pairingStart(passkey);
    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::HID))
        .Times(1);
    keyProtocolManager.pairingStop();

    EXPECT_CALL(keyProtocolListener,
                keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::CUSTOM))
        .Times(1);
    keyProtocolManager.keepAlive();
}

TEST_F(KeyProtocolManagerTest, PairingConnectedAfterPairingStart)
{
    std::string passkeyStr = "012345";
    BLEDefs::Passkey passkey(passkeyStr.begin(), passkeyStr.end());

    keyProtocolManager.init();

    EXPECT_CALL(keyProtocolListener,
                keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::PAIRING))
        .Times(1);
    keyProtocolManager.pairingStart(passkey);
    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged).Times(0);
    keyProtocolManager.keepAlive();
    EXPECT_CALL(keyProtocolListener, keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::HID))
        .Times(1);
    keyProtocolManager.pairingStop();

    EXPECT_CALL(keyProtocolListener,
                keyProtocolModeChanged(keyprotocoldefs::KeyProtocolMode::CUSTOM))
        .Times(1);
    keyProtocolManager.keepAlive();
}