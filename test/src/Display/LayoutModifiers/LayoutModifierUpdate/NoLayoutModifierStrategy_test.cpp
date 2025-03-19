#include "LayoutModifierStrategy_test.hpp"

#include "NoLayoutModifierStrategy.hpp"

class NoLayoutModifierStrategyTest : public LayoutModifierStrategyTest<NoLayoutModifierStrategy>
{
};

TEST_F(NoLayoutModifierStrategyTest, Flags)
{
    testConvertToFbNum(0, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::ALT_RIGHT, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
    testConvertToFbNum(Flag::SHIFT_LEFT, LayoutModifiersDefs::FrameBufferIndex::DEFAULT);
};

TEST_F(NoLayoutModifierStrategyTest, Update)
{
    bool bDrawCall = false;

    layoutModifierStrategy.updateModifiers(0, [&](uint8_t fb) { bDrawCall = true; });

    EXPECT_FALSE(bDrawCall);
}

TEST_F(NoLayoutModifierStrategyTest, DrawStrategyTest)
{
    static constexpr uint8_t NB_IMAGES = 1;
    testDrawStrategyNbImages(NB_IMAGES);
}
