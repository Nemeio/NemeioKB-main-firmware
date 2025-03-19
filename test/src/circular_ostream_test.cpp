/*
 * circular_ostream_test_test.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: Labo
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <string>
#include <string_view>
#include "circular_ostream.hpp"

static constexpr size_t DEFAULT_BUFFER_SIZE = 1024;
using circular_ostream_test =
    embedded_io::circular_ostream<150, embedded_io::CircularBufferStreamOutput<DEFAULT_BUFFER_SIZE>>;

class CircularOsstreamTest : public ::testing::Test
{
protected:
    CircularOsstreamTest() = default;
};

TEST_F(CircularOsstreamTest, BooleanTrueValue)
{
    circular_ostream_test oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << true;
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("1"));
}

TEST_F(CircularOsstreamTest, BooleanFalseValue)
{
    circular_ostream_test oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << false;
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("0"));
}

TEST_F(CircularOsstreamTest, IntegerNegativeValue)
{
    circular_ostream_test oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << -68000;
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("-68000"));
}

TEST_F(CircularOsstreamTest, IntegerPositiveValue)
{
    circular_ostream_test oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << 85000;
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("85000"));
}

TEST_F(CircularOsstreamTest, CharValue)
{
    circular_ostream_test oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << 'e';
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("e"));
}

TEST_F(CircularOsstreamTest, StringValue)
{
    circular_ostream_test oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << etl::string<4>("test");
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("test"));
}

TEST_F(CircularOsstreamTest, CharArrayValue)
{
    circular_ostream_test oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << "charArray";
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("charArray"));
}

TEST_F(CircularOsstreamTest, Clear)
{
    circular_ostream_test oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << "charArray";
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("charArray"));
    oss.clear();
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string(""));
}

TEST_F(CircularOsstreamTest, StreamMultiple)
{
    circular_ostream_test oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << "This ";
    oss << "is ";
    oss << "a test";
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("This is a test"));
}

TEST_F(CircularOsstreamTest, MaxTempStringSize)
{
    embedded_io::circular_ostream<5, embedded_io::CircularBufferStreamOutput<DEFAULT_BUFFER_SIZE>>
        oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << "charArray";
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string("charA"));
}

TEST_F(CircularOsstreamTest, MaxBufferSize)
{
    embedded_io::circular_ostream<150, embedded_io::CircularBufferStreamOutput<10>> oss;
    etl::string<DEFAULT_BUFFER_SIZE> str;

    oss << "This is a test";
    oss.to_string(str);
    EXPECT_EQ(str.c_str(), std::string(" is a test"));
}

TEST_F(CircularOsstreamTest, PopTo)
{
    circular_ostream_test oss;

    etl::array<char, 30> data;
    std::string str("This is a test");
    size_t expectedSize = str.size();

    oss << str.c_str();
    EXPECT_EQ(oss.pop_to(data), expectedSize);
    EXPECT_EQ(std::string(data.data(), data.data() + expectedSize), str);

    EXPECT_EQ(oss.pop_to(data), 0);
}

TEST_F(CircularOsstreamTest, PopToMultiple)
{
    circular_ostream_test oss;

    etl::array<char, 13> data;
    std::string str("This is a test");
    size_t expectedSize = data.size();

    oss << str.c_str();

    EXPECT_EQ(oss.pop_to(data), expectedSize);
    EXPECT_EQ(std::string(data.data(), data.data() + expectedSize), "This is a tes");


    expectedSize = 1;
    EXPECT_EQ(oss.pop_to(data), expectedSize);
    EXPECT_EQ(std::string(data.data(), data.data() + expectedSize), "t");
}
