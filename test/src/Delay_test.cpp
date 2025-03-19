#include "gtest/gtest.h"

#include "SystemTypes.hpp"

TEST(DelayTest, MicrosecZero)
{
    DelayUs delay(0);
    EXPECT_EQ(delay.valueUs, 0);
    EXPECT_FALSE(delay.isInfiniteDelay());
};

TEST(DelayTest, Microsec)
{
    DelayUs delay(10);
    EXPECT_EQ(delay.valueUs, 10);
    EXPECT_FALSE(delay.isInfiniteDelay());
};

TEST(DelayTest, MicrosecInfinite)
{
    DelayUs delay(DelayUs::INFINITE_DELAY);
    EXPECT_TRUE(delay.isInfiniteDelay());
};

TEST(DelayTest, MillisecZero)
{
    DelayMs delay(0);
    EXPECT_EQ(delay.getMs(), 0);
    EXPECT_FALSE(delay.isInfiniteDelay());
};

TEST(DelayTest, Millisec)
{
    DelayMs delay(10);
    EXPECT_EQ(delay.getMs(), 10);
    EXPECT_EQ(delay.valueUs, 10000);
    EXPECT_FALSE(delay.isInfiniteDelay());
};

TEST(DelayTest, MillisecInfinite)
{
    DelayMs delay(DelayMs::INFINITE_DELAY);
    EXPECT_TRUE(delay.isInfiniteDelay());
};

TEST(DelayTest, Seconds)
{
    DelaySec delay(10);
    EXPECT_EQ(delay.getMs(), 10000);
    EXPECT_EQ(delay.valueUs, 10000000);
    EXPECT_FALSE(delay.isInfiniteDelay());
};

TEST(DelayTest, SecondsInfinite)
{
    DelaySec delay(DelayMs::INFINITE_DELAY);
    EXPECT_TRUE(delay.isInfiniteDelay());
};

TEST(DelayTest, ComparisonEquality)
{
    DelayUs delay1(10);
    DelayUs delay2(20);
    EXPECT_TRUE(delay1 == delay1);
    EXPECT_FALSE(delay1 == delay2);
};

TEST(DelayTest, ComparisonEqualityInfinite)
{
    DelayUs delay1(10);
    DelayUs delay2(DelayUs::INFINITE_DELAY);
    EXPECT_FALSE(delay1 == delay2);
    EXPECT_TRUE(delay2 == delay2);

    DelayMs delay3(DelayMs::INFINITE_DELAY);
    EXPECT_TRUE(delay3 == delay2);
    EXPECT_TRUE(delay2 == delay3);
};

TEST(DelayTest, ComparisonDifferent)
{
    DelayUs delay1(10);
    DelayUs delay2(20);
    EXPECT_TRUE(delay1 != delay2);
    EXPECT_FALSE(delay1 != delay1);
};

TEST(DelayTest, ComparisonDifferentInfinite)
{
    DelayUs delay1(10);
    DelayUs delay2(DelayUs::INFINITE_DELAY);
    EXPECT_TRUE(delay1 != delay2);
    EXPECT_FALSE(delay2 != delay2);

    DelayMs delay3(DelayMs::INFINITE_DELAY);
    EXPECT_FALSE(delay3 != delay2);
    EXPECT_FALSE(delay2 != delay3);
};

TEST(DelayTest, ComparisonInferiorOrEqual)
{
    DelayUs delay1(10);
    DelayUs delay2(20);
    EXPECT_TRUE(delay1 <= delay2);
    EXPECT_FALSE(delay2 <= delay1);

    EXPECT_TRUE(delay2 <= delay2);
};

TEST(DelayTest, ComparisonInferiorOrEqualInfinite)
{
    DelayUs delay1(10);
    DelayUs delay2(DelayUs::INFINITE_DELAY);
    EXPECT_TRUE(delay1 <= delay2);
    EXPECT_FALSE(delay2 <= delay1);

    EXPECT_TRUE(delay2 <= delay2);

    DelayMs delay3 = DelayMs(DelayMs::INFINITE_DELAY);
    EXPECT_TRUE(delay3 <= delay2);
    EXPECT_TRUE(delay2 <= delay3);
};

TEST(DelayTest, MinMaxValues)
{
    EXPECT_EQ(INT32_MAX, DelayUs::getMaxUs());
    EXPECT_EQ(INT32_MAX / 1000, DelayMs::getMaxMs());
    EXPECT_EQ(INT32_MAX / 1000000, DelaySec::getMaxSec());

    EXPECT_EQ(INT32_MIN, DelayUs::getMinUs());
    EXPECT_EQ(INT32_MIN / 1000, DelayMs::getMinMs());
    EXPECT_EQ(INT32_MIN / 1000000, DelaySec::getMinSec());
};