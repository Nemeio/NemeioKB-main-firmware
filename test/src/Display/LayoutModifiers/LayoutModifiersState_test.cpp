#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "LayoutModifiersState.hpp"
#include "MockLayoutModifierListener.hpp"
#include "DummyRecursiveMutex.hpp"

using namespace configuration;

class LayoutModifiersStateTest : public Identifiable, public ::testing::Test
{
protected:
    LayoutModifiersStateTest()
        : layoutModifiersState(mutex, listener)
        , defaultLayoutConfiguration(uuid::UuidView{defaultUuid},
                                     uuid::UuidView{defaultUuid},
                                     etl::nullopt,
                                     OperatingSystem::WINDOWS,
                                     defaultKeySequence,
                                     defaultKeySequence){};

    void keyLockStatesChanged(const HIDDefs::KeyLockStates keyLockStates,
                              bool bExpectNotification,
                              LayoutModifiersDefs::LayoutModifierStateFlagsInt expectedFlags = 0)
    {
        if(bExpectNotification)
        {
            EXPECT_CALL(listener,
                        layoutModifiersStateChanged(
                            LayoutModifiersDefs::LayoutModifierFlags(expectedFlags)));
        }
        layoutModifiersState.keyLockStatesChanged(*this, keyLockStates);
    }

    void capslockChanged(bool bCapslock,
                         bool bExpectNotification,
                         LayoutModifiersDefs::LayoutModifierStateFlagsInt expectedFlags = 0)
    {
        HIDDefs::KeyLockStates keyLockStates;
        keyLockStates.capsLock = bCapslock;

        keyLockStatesChanged(keyLockStates, bExpectNotification, expectedFlags);
    }

    void pressedKey(ButtonID button,
                    const Configuration& defaultLayoutConfiguration,
                    bool bExpectNotification,
                    LayoutModifiersDefs::LayoutModifierStateFlagsInt expectedFlags = 0)
    {
        if(bExpectNotification)
        {
            EXPECT_CALL(listener,
                        layoutModifiersStateChanged(
                            LayoutModifiersDefs::LayoutModifierFlags(expectedFlags)));
        }
        layoutModifiersState.pressedKeyWithConfig(button, defaultLayoutConfiguration);
    }

    void unpressedKey(ButtonID button,
                      const Configuration& defaultLayoutConfiguration,
                      bool bExpectNotification,
                      LayoutModifiersDefs::LayoutModifierStateFlagsInt expectedFlags = 0)
    {
        if(bExpectNotification)
        {
            EXPECT_CALL(listener,
                        layoutModifiersStateChanged(
                            LayoutModifiersDefs::LayoutModifierFlags(expectedFlags)));
        }
        layoutModifiersState.unpressedKeyWithConfig(button, defaultLayoutConfiguration);
    }

    DummyRecursiveMutex mutex;
    MockLayoutModifierListener listener;
    LayoutModifiersState layoutModifiersState;
    uuid::Uuid defaultUuid{etl::array<std::byte, uuid::SIZE>{std::byte(0x00)}};
    etl::vector<KeySequence, 1> defaultKeySequence;
    configuration::LayoutConfiguration defaultLayoutConfiguration;
};

TEST_F(LayoutModifiersStateTest, KeyLockStates)
{
    capslockChanged(true, true, LayoutModifiersDefs::Flag::CAPSLOCK);
    capslockChanged(true, false);
    capslockChanged(false, true, 0);

    HIDDefs::KeyLockStates keyLockStates;
    keyLockStates.numLock = true;
    keyLockStatesChanged(keyLockStates, false);
};

TEST_F(LayoutModifiersStateTest, Buttons)
{
    pressedKey(BTN_ESCAPE, defaultLayoutConfiguration, false);

    pressedKey(BTN_CTRL_L_MAC_FN,
               defaultLayoutConfiguration,
               true,
               LayoutModifiersDefs::Flag::CTRL_LEFT);
    pressedKey(BTN_CTRL_L_MAC_FN, defaultLayoutConfiguration, false);
    unpressedKey(BTN_CTRL_L_MAC_FN, defaultLayoutConfiguration, true, 0);

    pressedKey(BTN_ALT_R_MAC_GUI_R,
               defaultLayoutConfiguration,
               true,
               LayoutModifiersDefs::Flag::ALT_RIGHT);
};

TEST_F(LayoutModifiersStateTest, CapsLockAndButtons)
{
    pressedKey(BTN_CTRL_L_MAC_FN,
               defaultLayoutConfiguration,
               true,
               LayoutModifiersDefs::Flag::CTRL_LEFT);
    pressedKey(BTN_ALT_R_MAC_GUI_R,
               defaultLayoutConfiguration,
               true,
               LayoutModifiersDefs::Flag::CTRL_LEFT | LayoutModifiersDefs::Flag::ALT_RIGHT);

    capslockChanged(true,
                    true,
                    LayoutModifiersDefs::Flag::CTRL_LEFT | LayoutModifiersDefs::Flag::ALT_RIGHT
                        | LayoutModifiersDefs::Flag::CAPSLOCK);
    unpressedKey(BTN_CTRL_L_MAC_FN,
                 defaultLayoutConfiguration,
                 true,
                 LayoutModifiersDefs::Flag::ALT_RIGHT | LayoutModifiersDefs::Flag::CAPSLOCK);
};

TEST_F(LayoutModifiersStateTest, KeypadActionsChanged)
{
    pressedKey(BTN_CTRL_L_MAC_FN,
               defaultLayoutConfiguration,
               true,
               LayoutModifiersDefs::Flag::CTRL_LEFT);

    EXPECT_CALL(listener, layoutModifiersStateChanged(LayoutModifiersDefs::LayoutModifierFlags(0)));
    layoutModifiersState.resetKeysWithConfig(defaultLayoutConfiguration);

    pressedKey(BTN_CTRL_L_MAC_FN,
               defaultLayoutConfiguration,
               true,
               LayoutModifiersDefs::Flag::CTRL_LEFT);
    pressedKey(BTN_ALT_R_MAC_GUI_R,
               defaultLayoutConfiguration,
               true,
               LayoutModifiersDefs::Flag::CTRL_LEFT | LayoutModifiersDefs::Flag::ALT_RIGHT);
    capslockChanged(true,
                    true,
                    LayoutModifiersDefs::Flag::CTRL_LEFT | LayoutModifiersDefs::Flag::ALT_RIGHT
                        | LayoutModifiersDefs::Flag::CAPSLOCK);

    EXPECT_CALL(listener,
                layoutModifiersStateChanged(
                    LayoutModifiersDefs::LayoutModifierFlags(LayoutModifiersDefs::Flag::CAPSLOCK)));
    layoutModifiersState.resetKeysWithConfig(defaultLayoutConfiguration);
};

TEST_F(LayoutModifiersStateTest, MacButtons)
{
    configuration::LayoutConfiguration macLayoutConfig(uuid::UuidView{defaultUuid},
                                                       uuid::UuidView{defaultUuid},
                                                       etl::nullopt,
                                                       configuration::OperatingSystem::MAC,
                                                       defaultKeySequence,
                                                       defaultKeySequence);

    pressedKey(BTN_ESCAPE, macLayoutConfig, false);

    pressedKey(BTN_CTRL_L_MAC_FN, macLayoutConfig, true, LayoutModifiersDefs::Flag::FN);
    pressedKey(BTN_CTRL_L_MAC_FN, macLayoutConfig, false);
    unpressedKey(BTN_CTRL_L_MAC_FN, macLayoutConfig, true, 0);

    pressedKey(BTN_ALT_R_MAC_GUI_R, macLayoutConfig, true, LayoutModifiersDefs::Flag::GUI_RIGHT);
};
