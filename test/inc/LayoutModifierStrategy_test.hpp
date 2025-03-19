#ifndef LAYOUTMODIFIERSTRATEGY_TEST_HPP
#define LAYOUTMODIFIERSTRATEGY_TEST_HPP

#include "gtest/gtest.h"

#include "ILayoutModifierStrategy.hpp"
#include "IScreen.hpp"

using namespace LayoutModifiersDefs;

template<typename LayoutModifierStrategy>
class LayoutModifierStrategyTest : public ::testing::Test
{
protected:
    LayoutModifierStrategyTest()
        : layoutModifierStrategy(){};

    void testConvertToFbNum(LayoutModifierStateFlagsInt flags,
                            LayoutModifiersDefs::FrameBufferIndex expectedFb)
    {
        EXPECT_EQ(layoutModifierStrategy.convertFlagsToFbNum(flags), expectedFb);
    }

    void testDrawStrategyNbImages(uint8_t expectedNbImages)
    {
        EXPECT_EQ(layoutModifierStrategy.getNbImages(), expectedNbImages);
    }

    LayoutModifierStrategy layoutModifierStrategy;
};

template<typename LayoutModifierStrategy>
class DisplayLayoutModifierStrategyTest : public LayoutModifierStrategyTest<LayoutModifierStrategy>
{
protected:
    DisplayLayoutModifierStrategyTest() = default;

    void testUpdateModifiers(LayoutModifierStateFlagsInt flags,
                             LayoutModifiersDefs::FrameBufferIndex expectedFb)
    {
        bool bDrawCall = false;
        uint8_t drawFb = static_cast<uint8_t>(LayoutModifiersDefs::FrameBufferIndex::DEFAULT);

        this->layoutModifierStrategy.updateModifiers(flags,
                                                     [&](uint8_t fb)
                                                     {
                                                         bDrawCall = true;
                                                         drawFb = fb;
                                                     });

        EXPECT_TRUE(bDrawCall);
        EXPECT_EQ(drawFb, static_cast<uint8_t>(expectedFb));
    }
};

#endif
