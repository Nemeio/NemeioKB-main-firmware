#include "MacAddress.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class MacAddressTest : public ::testing::Test
{
protected:
    MacAddressTest() = default;

    static constexpr size_t EXPECTED_MAC_ADDRESS_SIZE = 6;
    static constexpr char serialNumberRefValue[] = "ABCDEFG12345";
    etl::array<uint8_t, EXPECTED_MAC_ADDRESS_SIZE> stubMacAddress{0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
};

TEST_F(MacAddressTest, Get)
{
    MacAddress macAddress{stubMacAddress};

    etl::array_view<const uint8_t> macAdressArray = macAddress.get();

    EXPECT_EQ(macAdressArray, stubMacAddress);
}

TEST_F(MacAddressTest, Valid)
{
    MacAddress macAddress{stubMacAddress};

    EXPECT_TRUE(macAddress.isValid());
}

TEST_F(MacAddressTest, Invalid)
{
    MacAddress macAddress{etl::array<uint8_t, EXPECTED_MAC_ADDRESS_SIZE>{0}};

    EXPECT_FALSE(macAddress.isValid());
}

TEST_F(MacAddressTest, Equality)
{
    MacAddress macAddress1{stubMacAddress};
    MacAddress macAddress2{stubMacAddress};
    MacAddress macAddress3{etl::array<uint8_t, EXPECTED_MAC_ADDRESS_SIZE>{0}};

    EXPECT_TRUE(macAddress1 == macAddress2);
    EXPECT_FALSE(macAddress1 != macAddress2);

    EXPECT_TRUE(macAddress1 != macAddress3);
    EXPECT_FALSE(macAddress1 == macAddress3);
}
