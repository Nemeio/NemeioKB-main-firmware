/*
 * embedded_ostream_test.cpp
 *
 *  Created on: Apr 13, 2021
 *      Author: Labo
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <string>
#include "embedded_ostream.hpp"

class EmbeddedOsstreamTest : public ::testing::Test
{
protected:
    EmbeddedOsstreamTest() = default;
};

TEST_F(EmbeddedOsstreamTest, BooleanTrueValue)
{
    embedded_ostream oss;

    oss << true;
    EXPECT_EQ(oss.str().data(), std::string("1"));
}

TEST_F(EmbeddedOsstreamTest, BooleanFalseValue)
{
    embedded_ostream oss;

    oss << false;
    EXPECT_EQ(oss.str().data(), std::string("0"));
}

TEST_F(EmbeddedOsstreamTest, IntegerNegativeValue)
{
    embedded_ostream oss;

    oss << -68000;
    EXPECT_EQ(oss.str().data(), std::string("-68000"));
}

TEST_F(EmbeddedOsstreamTest, IntegerPositiveValue)
{
    embedded_ostream oss;

    oss << 85000;
    EXPECT_EQ(oss.str().data(), std::string("85000"));
}

TEST_F(EmbeddedOsstreamTest, CharValue)
{
    embedded_ostream oss;

    oss << 'e';
    EXPECT_EQ(oss.str().data(), std::string("e"));
}

TEST_F(EmbeddedOsstreamTest, StringValue)
{
    embedded_ostream oss;

    oss << etl::string<4>("test");
    EXPECT_EQ(oss.str().data(), std::string("test"));
}

TEST_F(EmbeddedOsstreamTest, CharArrayValue)
{
    embedded_ostream oss;

    oss << "charArray";
    EXPECT_EQ(oss.str().data(), std::string("charArray"));
}

TEST_F(EmbeddedOsstreamTest, Clear)
{
    embedded_ostream oss;

    oss << "charArray";
    EXPECT_EQ(oss.str().data(), std::string("charArray"));
    oss.clear();
    EXPECT_EQ(oss.str().data(), std::string(""));
}

TEST_F(EmbeddedOsstreamTest, MaxSize)
{
    embedded_io::embedded_ostream_base<5> oss;

    oss << "charArray";
    EXPECT_EQ(oss.str().data(), std::string("charA"));
}

TEST_F(EmbeddedOsstreamTest, MaxSizeStreamMultiple)
{
    embedded_io::embedded_ostream_base<5> oss;

    oss << "char";
    EXPECT_EQ(oss.str().data(), std::string("char"));
    oss << "Array";
    EXPECT_EQ(oss.str().data(), std::string("charA"));
    oss << "123";
    EXPECT_EQ(oss.str().data(), std::string("charA"));
}
