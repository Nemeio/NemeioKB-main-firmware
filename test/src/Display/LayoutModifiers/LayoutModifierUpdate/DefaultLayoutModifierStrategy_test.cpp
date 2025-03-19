#include "LayoutModifierStrategy_test.hpp"

#include "DefaultLayoutModifierStrategy.hpp"

class DefaultLayoutModifierStrategyTest
    : public DisplayLayoutModifierStrategyTest<DefaultLayoutModifierStrategy>
{
};

TEST_F(DefaultLayoutModifierStrategyTest, Flags)
{
    testConvertToFbNum(0, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::CTRL_LEFT, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::CTRL_RIGHT, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::GUI_LEFT, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::GUI_RIGHT, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::ALT_LEFT, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);

    testConvertToFbNum(Flag::SHIFT_LEFT | Flag::ALT_LEFT,
                       LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::SHIFT_LEFT | Flag::CAPSLOCK,
                       LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::CTRL_LEFT | Flag::CAPSLOCK,
                       LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::SHIFT_LEFT | Flag::FN, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);

    testConvertToFbNum(Flag::ALT_RIGHT, LayoutModifiersDefs::FrameBufferIndex::ALTGR);
    testConvertToFbNum(Flag::CTRL_LEFT | Flag::ALT_LEFT,
                       LayoutModifiersDefs::FrameBufferIndex::ALTGR);
    testConvertToFbNum(Flag::CTRL_RIGHT | Flag::ALT_LEFT,
                       LayoutModifiersDefs::FrameBufferIndex::ALTGR);
    testConvertToFbNum(Flag::CTRL_LEFT | Flag::ALT_RIGHT,
                       LayoutModifiersDefs::FrameBufferIndex::ALTGR);
    testConvertToFbNum(Flag::ALT_LEFT | Flag::ALT_RIGHT,
                       LayoutModifiersDefs::FrameBufferIndex::ALTGR);

    testConvertToFbNum(Flag::SHIFT_LEFT, LayoutModifiersDefs::FrameBufferIndex::SHIFT);
    testConvertToFbNum(Flag::SHIFT_RIGHT, LayoutModifiersDefs::FrameBufferIndex::SHIFT);

    testConvertToFbNum(Flag::SHIFT_LEFT | Flag::ALT_RIGHT,
                       LayoutModifiersDefs::FrameBufferIndex::SHIFT_ALTGR);
    testConvertToFbNum(Flag::SHIFT_RIGHT | Flag::ALT_RIGHT,
                       LayoutModifiersDefs::FrameBufferIndex::SHIFT_ALTGR);
    testConvertToFbNum(Flag::SHIFT_RIGHT | Flag::CTRL_LEFT | Flag::ALT_LEFT,
                       LayoutModifiersDefs::FrameBufferIndex::SHIFT_ALTGR);

    testConvertToFbNum(Flag::CAPSLOCK, LayoutModifiersDefs::FrameBufferIndex::CAPSLOCK);

    testConvertToFbNum(Flag::FN, LayoutModifiersDefs::FrameBufferIndex::FN);
};

TEST_F(DefaultLayoutModifierStrategyTest, Update)
{
    testUpdateModifiers(0, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testUpdateModifiers(Flag::ALT_RIGHT, LayoutModifiersDefs::FrameBufferIndex::ALTGR);
}

TEST_F(DefaultLayoutModifierStrategyTest, DrawStrategyTest)
{
    static constexpr uint8_t NB_IMAGES = 6;
    testDrawStrategyNbImages(NB_IMAGES);
}
