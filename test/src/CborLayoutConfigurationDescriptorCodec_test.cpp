#include "gtest/gtest.h"
#include "CborLayoutConfigurationDescriptorCodec.hpp"

using namespace configuration;
class CborLayoutConfigurationDescriptorCodecTest : public ::testing::Test
{
public:
    CborLayoutConfigurationDescriptorCodecTest()
        : idLayoutConfiguration({reinterpret_cast<std::byte*>(idLayoutConfigurationU8),
                                 sizeof(idLayoutConfigurationU8)})
        , layoutConfigurationHash({reinterpret_cast<std::byte*>(layoutConfigurationHashU8),
                                   sizeof(layoutConfigurationHashU8)})
        , layoutHash({reinterpret_cast<std::byte*>(layoutHashU8), sizeof(layoutHashU8)})
    {
    }

    uint8_t idLayoutConfigurationU8[16]{0x8a,
                                        0xc0,
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
                                        0x64,
                                        0xe9};
    uuid::UuidView idLayoutConfiguration;

    uint8_t layoutConfigurationHashU8[16]{0x88,
                                          0x7e,
                                          0x8c,
                                          0xb0,
                                          0xe8,
                                          0x72,
                                          0x4a,
                                          0x39,
                                          0xbf,
                                          0x75,
                                          0x00,
                                          0xa3,
                                          0x14,
                                          0x12,
                                          0x00,
                                          0x99};
    uuid::UuidView layoutConfigurationHash;

    uint8_t layoutHashU8[16]{0xAA,
                             0x7e,
                             0x8c,
                             0xb0,
                             0xe8,
                             0x72,
                             0x4a,
                             0x39,
                             0xbf,
                             0x75,
                             0x00,
                             0xa3,
                             0x14,
                             0x12,
                             0x00,
                             0xBB};
    uuid::UuidView layoutHash;

    CborLayoutConfigurationDescriptorCodec codec;
};

TEST_F(CborLayoutConfigurationDescriptorCodecTest, ValidInput)
{
    const uint8_t input[] = {0xa3, 0x01, 0x50, 0x8a, 0xc0, 0x1e, 0x5f, 0x62, 0x4e, 0x40, 0xdf,
                             0x99, 0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9, 0x02, 0x50, 0x88,
                             0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3,
                             0x14, 0x12, 0x00, 0x99, 0x03, 0x50, 0xAA, 0x7e, 0x8c, 0xb0, 0xe8,
                             0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0xBB};

    etl::optional<LayoutConfigurationDescriptor> layoutConfigurationDescriptor;
    OptionalReturn<LayoutConfigurationDescriptor> retLayoutConfigurationDescriptor =
        createOptionalReturn<LayoutConfigurationDescriptor>(layoutConfigurationDescriptor);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            retLayoutConfigurationDescriptor);

    EXPECT_TRUE(ret);
    ASSERT_TRUE(layoutConfigurationDescriptor.has_value());

    EXPECT_EQ(layoutConfigurationDescriptor.value().id, idLayoutConfiguration);
    EXPECT_EQ(layoutConfigurationDescriptor.value().layoutConfigurationHash,
              layoutConfigurationHash);
    EXPECT_EQ(layoutConfigurationDescriptor.value().layoutHash, layoutHash);

    // Test the validation only
    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptor(etl::nullopt);
    ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                       nullRetLayoutConfigurationDescriptor);
    EXPECT_TRUE(ret);
}

TEST_F(CborLayoutConfigurationDescriptorCodecTest, InvalidInput)
{
    const uint8_t input[] = {0xa3, 0x01, 0x50, 0x8a, 0xc0, 0x1e, 0x5f, 0x62, 0x4e, 0x40, 0xdf,
                             0x99, 0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9, 0x02, 0x50, 0x88,
                             0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3,
                             0x14, 0x12, 0x00, 0x99, 0x03, 0x50, 0xAA, 0x7e, 0x8c, 0xb0, 0xe8,
                             0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00};

    etl::optional<LayoutConfigurationDescriptor> layoutConfigurationDescriptor;
    OptionalReturn<LayoutConfigurationDescriptor> retLayoutConfigurationDescriptor =
        createOptionalReturn<LayoutConfigurationDescriptor>(layoutConfigurationDescriptor);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            retLayoutConfigurationDescriptor);

    EXPECT_FALSE(ret);
    EXPECT_FALSE(layoutConfigurationDescriptor.has_value());

    // Test the validation only
    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptor(etl::nullopt);
    ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                       nullRetLayoutConfigurationDescriptor);

    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutConfigurationDescriptorCodecTest, MissingId)
{
    const uint8_t input[] = {0xa2, 0x02, 0x50, 0x88, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a,
                             0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x99, 0x03,
                             0x50, 0xAA, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                             0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0xBB};

    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptor(etl::nullopt);
    bool retA = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                             nullRetLayoutConfigurationDescriptor);

    EXPECT_FALSE(retA);
}

TEST_F(CborLayoutConfigurationDescriptorCodecTest, MissingLayoutConfigurationHash)
{
    const uint8_t input[] = {0xa2, 0x01, 0x50, 0x8a, 0xc0, 0x1e, 0x5f, 0x62, 0x4e, 0x40,
                             0xdf, 0x99, 0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9, 0x03,
                             0x50, 0xAA, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                             0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0xBB};

    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptor(etl::nullopt);
    bool retA = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                             nullRetLayoutConfigurationDescriptor);

    EXPECT_FALSE(retA);
}

TEST_F(CborLayoutConfigurationDescriptorCodecTest, MissingLayoutHash)
{
    const uint8_t input[] = {0xa2, 0x01, 0x50, 0x8a, 0xc0, 0x1e, 0x5f, 0x62, 0x4e, 0x40,
                             0xdf, 0x99, 0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9, 0x02,
                             0x50, 0x88, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                             0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x99};

    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptor(etl::nullopt);
    bool retA = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                             nullRetLayoutConfigurationDescriptor);

    EXPECT_FALSE(retA);
}

TEST_F(CborLayoutConfigurationDescriptorCodecTest, InvalidId)
{
    // Length 15
    const uint8_t inputA[] = {0xa3, 0x01, 0x4f, 0xc0, 0x1e, 0x5f, 0x62, 0x4e, 0x40, 0xdf, 0x99,
                              0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9, 0x02, 0x50, 0x88, 0x7e,
                              0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14,
                              0x12, 0x00, 0x99, 0x03, 0x50, 0xAA, 0x7e, 0x8c, 0xb0, 0xe8, 0x72,
                              0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0xBB};
    // Length 17
    const uint8_t inputB[] = {0xa3, 0x01, 0x51, 0x55, 0x8a, 0xc0, 0x1e, 0x5f, 0x62, 0x4e,
                              0x40, 0xdf, 0x99, 0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9,
                              0x02, 0x50, 0x88, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                              0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x99, 0x03, 0x50,
                              0xAA, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75,
                              0x00, 0xa3, 0x14, 0x12, 0x00, 0xBB};

    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptorA(
        etl::nullopt);
    bool retA = codec.decode({reinterpret_cast<const std::byte*>(inputA), sizeof(inputA)},
                             nullRetLayoutConfigurationDescriptorA);

    EXPECT_FALSE(retA);

    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptorB(
        etl::nullopt);
    bool retB = codec.decode({reinterpret_cast<const std::byte*>(inputB), sizeof(inputB)},
                             nullRetLayoutConfigurationDescriptorB);

    EXPECT_FALSE(retB);
}

TEST_F(CborLayoutConfigurationDescriptorCodecTest, InvalidLayoutConfigurationHash)
{
    // Length 15
    const uint8_t inputA[] = {0xa3, 0x01, 0x50, 0x8a, 0xc0, 0x1e, 0x5f, 0x62, 0x4e, 0x40, 0xdf,
                              0x99, 0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9, 0x02, 0x4f, 0x7e,
                              0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14,
                              0x12, 0x00, 0x99, 0x03, 0x50, 0xAA, 0x7e, 0x8c, 0xb0, 0xe8, 0x72,
                              0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0xBB};
    // Length 17
    const uint8_t inputB[] = {0xa3, 0x01, 0x50, 0x8a, 0xc0, 0x1e, 0x5f, 0x62, 0x4e, 0x40,
                              0xdf, 0x99, 0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9, 0x02,
                              0x51, 0x55, 0x88, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                              0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x99, 0x03, 0x50,
                              0xAA, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75,
                              0x00, 0xa3, 0x14, 0x12, 0x00, 0xBB};

    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptorA(
        etl::nullopt);
    bool retA = codec.decode({reinterpret_cast<const std::byte*>(inputA), sizeof(inputA)},
                             nullRetLayoutConfigurationDescriptorA);

    EXPECT_FALSE(retA);

    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptorB(
        etl::nullopt);
    bool retB = codec.decode({reinterpret_cast<const std::byte*>(inputB), sizeof(inputB)},
                             nullRetLayoutConfigurationDescriptorB);

    EXPECT_FALSE(retB);
}

TEST_F(CborLayoutConfigurationDescriptorCodecTest, InvalidLayoutHash)
{
    // Length 15
    const uint8_t inputA[] = {0xa3, 0x01, 0x50, 0x8a, 0xc0, 0x1e, 0x5f, 0x62, 0x4e, 0x40, 0xdf,
                              0x99, 0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9, 0x02, 0x50, 0x88,
                              0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3,
                              0x14, 0x12, 0x00, 0x99, 0x03, 0x4f, 0x7e, 0x8c, 0xb0, 0xe8, 0x72,
                              0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0xBB};
    // Length 17
    const uint8_t inputB[] = {0xa3, 0x01, 0x50, 0x8a, 0xc0, 0x1e, 0x5f, 0x62, 0x4e, 0x40,
                              0xdf, 0x99, 0x65, 0xe2, 0x04, 0xc9, 0xa9, 0x64, 0xe9, 0x02,
                              0x50, 0x88, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                              0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x99, 0x03, 0x51, 0xFF,
                              0xAA, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75,
                              0x00, 0xa3, 0x14, 0x12, 0x00, 0xBB};

    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptorA(
        etl::nullopt);
    bool retA = codec.decode({reinterpret_cast<const std::byte*>(inputA), sizeof(inputA)},
                             nullRetLayoutConfigurationDescriptorA);

    EXPECT_FALSE(retA);

    OptionalReturn<LayoutConfigurationDescriptor> nullRetLayoutConfigurationDescriptorB(
        etl::nullopt);
    bool retB = codec.decode({reinterpret_cast<const std::byte*>(inputB), sizeof(inputB)},
                             nullRetLayoutConfigurationDescriptorB);

    EXPECT_FALSE(retB);
}