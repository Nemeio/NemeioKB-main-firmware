#include "gtest/gtest.h"
#include "CborUuidListCodec.hpp"

class CborUuidListCodecTest : public ::testing::Test
{
public:
    CborUuidListCodecTest()
        : layoutHash({reinterpret_cast<std::byte*>(layoutHashU8), sizeof(layoutHashU8)})
        , layoutHash2({reinterpret_cast<std::byte*>(layoutHash2U8), sizeof(layoutHash2U8)})
    {
    }

    uint8_t layoutHashU8[16]{0x80,
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
                             0x0d};
    uuid::Uuid layoutHash;

    uint8_t layoutHash2U8[16]{0x88,
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
                              0xdd};
    uuid::Uuid layoutHash2;

    CborUuidListCodec codec;
};

TEST_F(CborUuidListCodecTest, Nominal)
{
    const uint8_t expectedOutputBuffer[] = {0xa1, 0x01, 0x82, 0xa1, 0x01, 0x50, 0x80, 0x7e, 0x8c,
                                            0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3,
                                            0x14, 0x12, 0x00, 0x0d, 0xa1, 0x01, 0x50, 0x88, 0x7e,
                                            0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00,
                                            0xa3, 0x14, 0x12, 0x00, 0xdd};
    etl::array_view<const std::byte> expectedOutput{reinterpret_cast<const std::byte*>(
                                                        expectedOutputBuffer),
                                                    sizeof(expectedOutputBuffer)};
    etl::array<std::byte, 1024> outputBuffer;

    etl::array<uuid::Uuid, 2> list{layoutHash, layoutHash2};

    ssize_t retSize = codec.encode(list, outputBuffer);

    etl::array_view<const std::byte> output{outputBuffer.data(), retSize};

    EXPECT_GE(retSize, 0);
    EXPECT_EQ(output, expectedOutput);
}

TEST_F(CborUuidListCodecTest, EmptyList)
{
    const uint8_t expectedOutputBuffer[] = {0xa1, 0x01, 0x80};
    etl::array_view<const std::byte> expectedOutput{reinterpret_cast<const std::byte*>(
                                                        expectedOutputBuffer),
                                                    sizeof(expectedOutputBuffer)};
    etl::array<std::byte, 1024> outputBuffer;

    etl::vector<uuid::Uuid, 1> list;

    ssize_t retSize = codec.encode(list, outputBuffer);

    etl::array_view<const std::byte> output{outputBuffer.data(), retSize};

    EXPECT_GE(retSize, 0);
    EXPECT_EQ(output, expectedOutput);
}