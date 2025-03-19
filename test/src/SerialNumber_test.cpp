/*
 * AbstractSerialNumber_test.cpp
 *
 *  Created on: Jan 14, 2020
 *      Author: Labo
 */

#include "SerialNumber.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class SerialNumberTest : public ::testing::Test
{
protected:
    SerialNumberTest() = default;

    static constexpr char serialNumberRefValue[] = "ABCDEFG12345";
    etl::string<SERIAL_NUMBER_MAX_SIZE> stubSerialNumber{"ABCDEFG12345"};
};

TEST_F(SerialNumberTest, GetString)
{
    SerialNumber serialNumber{stubSerialNumber};

    size_t retSize = 0;
    etl::string_view serialNumberStr = serialNumber.getString();

    EXPECT_STREQ(serialNumberStr.data(), serialNumberRefValue);
}

TEST_F(SerialNumberTest, ConstructFromLongerString)
{
    SerialNumber longerSerialNumber{"ABCDEFG123456"};

    // Since data will be truncated, strings will be identical
    EXPECT_STREQ(longerSerialNumber.getString().data(), serialNumberRefValue);
}

TEST_F(SerialNumberTest, ConstructFromShorterString)
{
    SerialNumber shorterSerialNumber{"ABCDEFG1234"};

    EXPECT_STREQ(shorterSerialNumber.getString().data(), "ABCDEFG1234");
}

TEST_F(SerialNumberTest, Valid)
{
    SerialNumber serialNumber{stubSerialNumber};
    EXPECT_TRUE(serialNumber.isValid());
}

TEST_F(SerialNumberTest, Invalid)
{
    SerialNumber shorterSerialNumber{"ABCDEFG1234"};
    EXPECT_FALSE(shorterSerialNumber.isValid());
}

TEST_F(SerialNumberTest, Equality)
{
    SerialNumber serialNumber1{stubSerialNumber};
    SerialNumber serialNumber2{stubSerialNumber};
    SerialNumber serialNumber3{"ABCDEFG12346"};

    EXPECT_TRUE(serialNumber1 == serialNumber2);
    EXPECT_FALSE(serialNumber1 != serialNumber2);

    EXPECT_TRUE(serialNumber1 != serialNumber3);
    EXPECT_FALSE(serialNumber1 == serialNumber3);
}
