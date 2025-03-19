#include "gtest/gtest.h"
#include "CborLayoutMetadataCodec.hpp"

using namespace configuration;
class CborLayoutMetadataCodecTest : public ::testing::Test
{
public:
    CborLayoutMetadataCodecTest() = default;

    CborLayoutMetadataCodec codec;
};

TEST_F(CborLayoutMetadataCodecTest, ValidInput)
{
    const uint8_t input[] = {0xa2, 0x01, 0x02, 0x02, 0x18, 0xFF};

    etl::optional<LayoutMetadata> layoutMetadata;
    OptionalReturn<LayoutMetadata> retLayoutMetadata = createOptionalReturn<LayoutMetadata>(
        layoutMetadata);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            retLayoutMetadata);

    EXPECT_TRUE(ret);
    ASSERT_TRUE(layoutMetadata.has_value());

    EXPECT_EQ(layoutMetadata.value().background, ScreenDefs::BackgroundColor::BLACK);
    EXPECT_EQ(layoutMetadata.value().format, ScreenDefs::PixelFormat::_JPEG);

    // Test the validation only
    OptionalReturn<LayoutMetadata> nullRetLayoutMetadata(etl::nullopt);
    ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                       nullRetLayoutMetadata);
    EXPECT_TRUE(ret);
}

TEST_F(CborLayoutMetadataCodecTest, ValidInput2)
{
    const uint8_t input[] = {0xa2, 0x01, 0x00, 0x02, 0x02};

    etl::optional<LayoutMetadata> layoutMetadata;
    OptionalReturn<LayoutMetadata> retLayoutMetadata = createOptionalReturn<LayoutMetadata>(
        layoutMetadata);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            retLayoutMetadata);

    EXPECT_TRUE(ret);
    ASSERT_TRUE(layoutMetadata.has_value());

    EXPECT_EQ(layoutMetadata.value().background, ScreenDefs::BackgroundColor::UNKNOWN);
    EXPECT_EQ(layoutMetadata.value().format, ScreenDefs::PixelFormat::_2BPP);
}

TEST_F(CborLayoutMetadataCodecTest, IncompleteInput)
{
    const uint8_t input[] = {0xa2, 0x01, 0x00, 0x02, 0x18};

    etl::optional<LayoutMetadata> layoutMetadata;
    OptionalReturn<LayoutMetadata> retLayoutMetadata = createOptionalReturn<LayoutMetadata>(
        layoutMetadata);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            retLayoutMetadata);

    EXPECT_FALSE(ret);
    EXPECT_FALSE(layoutMetadata.has_value());

    // Test the validation only
    OptionalReturn<LayoutMetadata> nullRetLayoutMetadata(etl::nullopt);
    ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                       nullRetLayoutMetadata);

    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutMetadataCodecTest, MissingBackground)
{
    const uint8_t input[] = {0xa1, 0x02, 0x18, 0xFF};

    OptionalReturn<LayoutMetadata> nullRetLayoutMetadata(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            nullRetLayoutMetadata);

    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutMetadataCodecTest, MissingFormat)
{
    const uint8_t input[] = {0xa1, 0x01, 0x00};

    OptionalReturn<LayoutMetadata> nullRetLayoutMetadata(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            nullRetLayoutMetadata);

    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutMetadataCodecTest, InvalidBackground)
{
    const uint8_t input[] = {0xa2, 0x01, 0x03, 0x02, 0x18, 0xFF};

    OptionalReturn<LayoutMetadata> nullRetLayoutMetadata(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            nullRetLayoutMetadata);

    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutMetadataCodecTest, InvalidFormat)
{
    const uint8_t input[] = {0xa2, 0x01, 0x02, 0x02, 0x18, 0xFE};

    OptionalReturn<LayoutMetadata> nullRetLayoutMetadata(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            nullRetLayoutMetadata);

    EXPECT_FALSE(ret);
}
