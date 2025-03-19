#include "TestBenchId.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "etl/string.h"

class TestBenchIdTest : public ::testing::Test
{
protected:
    TestBenchIdTest() = default;

    etl::string<TestBenchId::SIZE> stubTestBenchIdStr{"#ABCDEFGHIJKLMNOPQRSTUVWXYZ012345#"};
};

TEST_F(TestBenchIdTest, Default)
{
    etl::string<TestBenchId::SIZE> str{""};
    TestBenchId testBenchId;

    EXPECT_EQ(testBenchId.get(), str);
    EXPECT_FALSE(testBenchId.isValid());
}

TEST_F(TestBenchIdTest, GetSet)
{
    TestBenchId testBenchId;

    testBenchId.set(stubTestBenchIdStr);

    EXPECT_EQ(testBenchId.get(), stubTestBenchIdStr);
    EXPECT_TRUE(testBenchId.isValid());
}

TEST_F(TestBenchIdTest, SetShorter)
{
    TestBenchId testBenchId;

    etl::string<TestBenchId::SIZE - 1> stubTestBenchIdStrShorter{
        "#ABCDEFGHIJKLMNOPQRSTUVWXYZ01234#"};

    testBenchId.set(stubTestBenchIdStrShorter);

    EXPECT_NE(testBenchId.get(), stubTestBenchIdStrShorter);
    EXPECT_FALSE(testBenchId.isValid());
}

TEST_F(TestBenchIdTest, Equality)
{
    TestBenchId testBenchId1{stubTestBenchIdStr};
    TestBenchId testBenchId2{stubTestBenchIdStr};

    etl::string<TestBenchId::SIZE> stubTestBenchIdStr3{"#ABCDEFGHIJKLMNOPQRSTUVWXYZ012345!"};
    TestBenchId testBenchId3{stubTestBenchIdStr3};

    EXPECT_TRUE(testBenchId1 == testBenchId2);
    EXPECT_FALSE(testBenchId1 != testBenchId2);

    EXPECT_TRUE(testBenchId1 != testBenchId3);
    EXPECT_FALSE(testBenchId1 == testBenchId3);
}
