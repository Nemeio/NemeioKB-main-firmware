#include "gtest/gtest.h"

#include "MacConvertModifierStrategy.hpp"

class MacConvertModifierStrategyTest : public ::testing::Test
{
protected:
    MacConvertModifierStrategyTest() = default;

    MacConvertModifierStrategy convertModifierStrategy;
};

TEST_F(MacConvertModifierStrategyTest, Buttons)
{
    EXPECT_EQ(convertModifierStrategy.convertButtonToModifierFlags(BTN_CTRL_L_MAC_FN),
              LayoutModifiersDefs::LayoutModifierFlags(LayoutModifiersDefs::Flag::FN));

    EXPECT_EQ(convertModifierStrategy.convertButtonToModifierFlags(BTN_FN_MAC_CTRL_L),
              LayoutModifiersDefs::LayoutModifierFlags(LayoutModifiersDefs::Flag::CTRL_LEFT));

    EXPECT_EQ(convertModifierStrategy.convertButtonToModifierFlags(BTN_GUI_L_MAC_ALT_L),
              LayoutModifiersDefs::LayoutModifierFlags(LayoutModifiersDefs::Flag::ALT_LEFT));

    EXPECT_EQ(convertModifierStrategy.convertButtonToModifierFlags(BTN_ALT_L_MAC_GUI_L),
              LayoutModifiersDefs::LayoutModifierFlags(LayoutModifiersDefs::Flag::GUI_LEFT));

    EXPECT_EQ(convertModifierStrategy.convertButtonToModifierFlags(BTN_ALT_R_MAC_GUI_R),
              LayoutModifiersDefs::LayoutModifierFlags(LayoutModifiersDefs::Flag::GUI_RIGHT));

    EXPECT_EQ(convertModifierStrategy.convertButtonToModifierFlags(BTN_CTRL_R_MAC_ALT_R),
              LayoutModifiersDefs::LayoutModifierFlags(LayoutModifiersDefs::Flag::ALT_RIGHT));

    EXPECT_EQ(convertModifierStrategy.convertButtonToModifierFlags(BTN_ESCAPE),
              LayoutModifiersDefs::LayoutModifierFlags(0));
};