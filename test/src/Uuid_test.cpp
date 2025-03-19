#include "gtest/gtest.h"

#include "Uuid.hpp"

TEST(UuidTest, UuidToStr)
{
    uint8_t idU8[16]{0xfe,
                     0xab,
                     0x1e,
                     0x5f,
                     0x62,
                     0x4e,
                     0x40,
                     0xdf,
                     0x99,
                     0x65,
                     0xe2,
                     0x04,
                     0xc9,
                     0xa9,
                     0x44,
                     0x55};

    uuid::Uuid uuid({reinterpret_cast<std::byte*>(idU8), sizeof(idU8)});

    embedded_ostream os;
    os << uuid;

    std::string expectedStr("feab1e5f624e40df9965e204c9a94455");

    EXPECT_EQ(os.str().c_str(), expectedStr);

    uuid::UuidView view(uuid);
    embedded_ostream osView;
    osView << view;
    EXPECT_EQ(osView.str().c_str(), expectedStr);
};