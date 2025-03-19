#include "gtest/gtest.h"

#include "utils.hpp"

TEST(UtilsTest, ByteToHex1)
{
    auto str = byteToHexStr(std::byte(0x01));
    EXPECT_STREQ(str.data(), "01");
};

TEST(UtilsTest, ByteToHex2)
{
    auto str = byteToHexStr(std::byte(0xEF));
    EXPECT_STREQ(str.data(), "ef");
};