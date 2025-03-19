#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "KeypadScanCodeConverter.hpp"
#include "MockNemeioKeypadPressedListener.hpp"

using ::testing::Mock;
using ::testing::StrictMock;

class KeypadScanCodeConverterTest : public ::testing::Test
{
protected:
    KeypadScanCodeConverterTest() { converter.registerKeyListener(nemeioKeypadPressedListener); }

    void pressKey(ButtonID button) { scanCodes[button.y] &= ~(1 << button.x); }

    void unpressKey(ButtonID button) { scanCodes[button.y] |= (1 << button.x); }

    ButtonID KEY_5{5, 1};

    ButtonID KEY_A{1, 2};
    ButtonID KEY_Z{2, 2};
    ButtonID KEY_E{3, 2};

    ButtonID KEY_Q{1, 3};
    ButtonID KEY_S{2, 3};
    ButtonID KEY_F{4, 3};

    ButtonID KEY_V{5, 4};
    ButtonID KEY_B{6, 4};
    ButtonID KEY_N{7, 4};

    ButtonID KEY_CTRL_L{6, 6};
    ButtonID KEY_ALT_L{0, 6};

    StrictMock<MockNemeioKeypadPressedListener> nemeioKeypadPressedListener;
    KeypadScanCodeConverter converter;

    etl::vector<uint16_t, KeypadDefs::MAX_ROW>
        scanCodes{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
};

TEST_F(KeypadScanCodeConverterTest, NoKey)
{
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, SingleKey)
{
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_A);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_A);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, MultipleKeys)
{
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_A);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_A);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, MultipleKeysReleasedAtSameTime)
{
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_A);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_A);
    unpressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, MultipleKeysPressedAndReleasedAtSameTime)
{
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_A);
    pressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_A);
    unpressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, Press7)
{
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_E)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_F)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_V)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_B)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_N)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_A);
    pressKey(KEY_Z);
    pressKey(KEY_E);
    pressKey(KEY_F);
    pressKey(KEY_V);
    pressKey(KEY_B);
    pressKey(KEY_N);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_E)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_F)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_V)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_B)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_N)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_A);
    unpressKey(KEY_Z);
    unpressKey(KEY_E);
    unpressKey(KEY_F);
    unpressKey(KEY_V);
    unpressKey(KEY_B);
    unpressKey(KEY_N);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, Modifiers)
{
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_CTRL_L)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_ALT_L)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_B)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_CTRL_L);
    pressKey(KEY_ALT_L);
    pressKey(KEY_B); // Key mapped on the same column that CTRL_L but no ghosting should be detected
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, GhostingAllAtOnceMinimalConditions)
{
    pressKey(KEY_A);
    pressKey(KEY_S);
    pressKey(KEY_Z); // Cannot happen with hardware (Q will also be pressed)
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);
    unpressKey(KEY_A);
    unpressKey(KEY_S);
    unpressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, GhostingAllAtOnce)
{
    pressKey(KEY_A);
    pressKey(KEY_S);
    pressKey(KEY_Z);
    pressKey(KEY_Q);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);
    unpressKey(KEY_A);
    unpressKey(KEY_S);
    unpressKey(KEY_Z);
    unpressKey(KEY_Q);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, GhostingUnpressAll)
{
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_S)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_A);
    pressKey(KEY_S);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    pressKey(KEY_Z);
    pressKey(KEY_Q);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_S)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_A);
    unpressKey(KEY_S);
    unpressKey(KEY_Z);
    unpressKey(KEY_Q);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, GhostingUnpress1)
{
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_S)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    pressKey(KEY_A);
    pressKey(KEY_S);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    pressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_A)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_A);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_S)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, unpressedKey(KEY_Z)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_S);
    unpressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}

TEST_F(KeypadScanCodeConverterTest, GhostingIgnoreNewKey)
{
    pressKey(KEY_A);
    pressKey(KEY_S);
    pressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    pressKey(KEY_N);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    pressKey(KEY_5);
    converter.convertScanCodeToKeyUpdate(scanCodes);

    Mock::VerifyAndClearExpectations(&nemeioKeypadPressedListener);

    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_5)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, pressedKey(KEY_N)).Times(1);
    EXPECT_CALL(nemeioKeypadPressedListener, terminateUpdateKeys).Times(1);
    unpressKey(KEY_A);
    unpressKey(KEY_S);
    unpressKey(KEY_Z);
    converter.convertScanCodeToKeyUpdate(scanCodes);
}