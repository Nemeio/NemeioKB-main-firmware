#include "gtest/gtest.h"
#include "CborLayoutConfigurationCodec.hpp"

using namespace configuration;

class CborLayoutConfigurationCodecTest : public ::testing::Test
{
public:
    CborLayoutConfigurationCodecTest()
        : id({reinterpret_cast<std::byte*>(idU8), sizeof(idU8)})
        , layoutHash({reinterpret_cast<std::byte*>(layoutHashU8), sizeof(layoutHashU8)})
        , associatedHidLayoutConfigurationId(
              {reinterpret_cast<std::byte*>(associatedHidLayoutConfigurationIdU8),
               sizeof(associatedHidLayoutConfigurationIdU8)})
    {
    }

    uint8_t idU8[16]{0x8a,
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
    uuid::UuidView id;

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
    uuid::UuidView layoutHash;

    uint8_t associatedHidLayoutConfigurationIdU8[16]{0xab,
                                                     0xce,
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
                                                     0x0d,
                                                     0xef};
    uuid::UuidView associatedHidLayoutConfigurationId;

    CborLayoutConfigurationCodec codec;
};

TEST_F(CborLayoutConfigurationCodecTest, ValidInput)
{
    const uint8_t input[] = {0xa5, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                             0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0x50, 0xab,
                             0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3,
                             0x14, 0x12, 0x0d, 0xef, 0x03, 0x02, 0x04, 0x82, 0x83, 0x01, 0x02,
                             0x03, 0x83, 0x04, 0x05, 0x06, 0x05, 0x81, 0x81, 0x0a};

    etl::array<KeySequence, 2> expectedWinSas{KeySequence{1, 2, 3}, KeySequence{4, 5, 6}};
    etl::array<KeySequence, 1> expectedCapslock{KeySequence{10}};
    etl::optional<LayoutConfiguration> layoutConfiguration;
    OptionalReturn<LayoutConfiguration> retLayoutConfiguration =
        createOptionalReturn<LayoutConfiguration>(layoutConfiguration);

    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            id,
                            retLayoutConfiguration);

    EXPECT_TRUE(ret);
    ASSERT_TRUE(layoutConfiguration.has_value());

    ASSERT_TRUE(layoutConfiguration.value().getAssociatedHidLayoutConfigurationId().has_value());

    EXPECT_EQ(layoutConfiguration.value().getId(), id);
    EXPECT_EQ(layoutConfiguration.value().getLayoutHash(), layoutHash);
    EXPECT_EQ(layoutConfiguration.value().getAssociatedHidLayoutConfigurationId().value().get(),
              associatedHidLayoutConfigurationId);
    EXPECT_EQ(layoutConfiguration.value().getOperatingSystem(), OperatingSystem::MAC);
    EXPECT_EQ(layoutConfiguration.value().getWinSasSequence(), expectedWinSas);
    EXPECT_EQ(layoutConfiguration.value().getCapslockKeySequences(), expectedCapslock);

    // Test the validation only
    OptionalReturn<LayoutConfiguration> nullRetLayoutConfiguration(etl::nullopt);
    ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                       etl::nullopt,
                       nullRetLayoutConfiguration);
    EXPECT_TRUE(ret);
}

TEST_F(CborLayoutConfigurationCodecTest, IncompleteInput)
{
    const uint8_t input[] = {0xa4, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a,
                             0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02,
                             0x50, 0xab, 0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                             0x75, 0x00, 0xa3, 0x14, 0x12, 0x0d, 0xef, 0x03, 0x02, 0x04,
                             0x82, 0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05};

    etl::optional<LayoutConfiguration> layoutConfiguration;
    OptionalReturn<LayoutConfiguration> retLayoutConfiguration =
        createOptionalReturn<LayoutConfiguration>(layoutConfiguration);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            etl::nullopt,
                            retLayoutConfiguration);

    EXPECT_FALSE(ret);
    EXPECT_FALSE(layoutConfiguration.has_value());

    // Test the validation only
    OptionalReturn<LayoutConfiguration> nullRetLayoutConfiguration(etl::nullopt);
    ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                       etl::nullopt,
                       nullRetLayoutConfiguration);

    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutConfigurationCodecTest, MissingLayoutHash)
{
    const uint8_t input[] = {0xa3, 0x02, 0x50, 0xab, 0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                             0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x0d, 0xef, 0x03, 0x01, 0x04,
                             0x82, 0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06};
    OptionalReturn<LayoutConfiguration> nullRetLayoutConfiguration(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            etl::nullopt,
                            nullRetLayoutConfiguration);
    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutConfigurationCodecTest, MissingOperatingSystem)
{
    const uint8_t input[] = {0xa3, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                             0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0x50, 0xab, 0xce, 0x8c,
                             0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x0d,
                             0xef, 0x04, 0x82, 0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06};
    OptionalReturn<LayoutConfiguration> nullRetLayoutConfiguration(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            etl::nullopt,
                            nullRetLayoutConfiguration);
    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutConfigurationCodecTest, MinimalLayoutConfiguration)
{
    const uint8_t input[] = {0xa2, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                             0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x03, 0x01};

    etl::vector<KeySequence, 1> expectedWinSas{};

    etl::optional<LayoutConfiguration> layoutConfiguration;
    OptionalReturn<LayoutConfiguration> retLayoutConfiguration =
        createOptionalReturn<LayoutConfiguration>(layoutConfiguration);

    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            id,
                            retLayoutConfiguration);

    EXPECT_TRUE(ret);
    ASSERT_TRUE(layoutConfiguration.has_value());

    EXPECT_EQ(layoutConfiguration.value().getId(), id);
    EXPECT_EQ(layoutConfiguration.value().getLayoutHash(), layoutHash);
    EXPECT_FALSE(layoutConfiguration.value().getAssociatedHidLayoutConfigurationId().has_value());
    EXPECT_EQ(layoutConfiguration.value().getOperatingSystem(), OperatingSystem::WINDOWS);
    EXPECT_EQ(layoutConfiguration.value().getWinSasSequence(), expectedWinSas);
}

TEST_F(CborLayoutConfigurationCodecTest, AssociatedHidLayoutConfigurationIdInvalidType)
{
    const uint8_t input[] = {0xa3, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                             0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0x01, 0x03, 0x01};

    OptionalReturn<LayoutConfiguration> nullRetLayoutConfiguration(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            etl::nullopt,
                            nullRetLayoutConfiguration);
    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutConfigurationCodecTest, WinSasInvalidType)
{
    const uint8_t input[] = {0xa3, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                             0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x03, 0x01, 0x04, 0x05};

    OptionalReturn<LayoutConfiguration> nullRetLayoutConfiguration(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            etl::nullopt,
                            nullRetLayoutConfiguration);
    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutConfigurationCodecTest, AssociatedHidLayoutConfigurationIdNullType)
{
    const uint8_t input[] = {0xa4, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                             0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0xf6, 0x03,
                             0x02, 0x04, 0x82, 0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06};

    etl::array<KeySequence, 2> expectedWinSas{KeySequence{1, 2, 3}, KeySequence{4, 5, 6}};

    etl::optional<LayoutConfiguration> layoutConfiguration;
    OptionalReturn<LayoutConfiguration> retLayoutConfiguration =
        createOptionalReturn<LayoutConfiguration>(layoutConfiguration);

    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            id,
                            retLayoutConfiguration);

    EXPECT_TRUE(ret);
    ASSERT_TRUE(layoutConfiguration.has_value());

    EXPECT_EQ(layoutConfiguration.value().getId(), id);
    EXPECT_EQ(layoutConfiguration.value().getLayoutHash(), layoutHash);
    EXPECT_FALSE(layoutConfiguration.value().getAssociatedHidLayoutConfigurationId().has_value());
    EXPECT_EQ(layoutConfiguration.value().getOperatingSystem(), OperatingSystem::MAC);
    EXPECT_EQ(layoutConfiguration.value().getWinSasSequence(), expectedWinSas);
}

TEST_F(CborLayoutConfigurationCodecTest, InvalidOperatingSystem)
{
    // OS = 0
    const uint8_t inputA[] = {0xa4, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a,
                              0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02,
                              0x50, 0xab, 0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                              0x75, 0x00, 0xa3, 0x14, 0x12, 0x0d, 0xef, 0x03, 0x00, 0x04,
                              0x82, 0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06};
    // OS = 3
    const uint8_t inputB[] = {0xa4, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a,
                              0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02,
                              0x50, 0xab, 0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                              0x75, 0x00, 0xa3, 0x14, 0x12, 0x0d, 0xef, 0x03, 0x03, 0x04,
                              0x82, 0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06};
    OptionalReturn<LayoutConfiguration> nullRetLayoutConfigurationA(etl::nullopt);
    bool retA = codec.decode({reinterpret_cast<const std::byte*>(inputA), sizeof(inputA)},
                             etl::nullopt,
                             nullRetLayoutConfigurationA);
    EXPECT_FALSE(retA);

    OptionalReturn<LayoutConfiguration> nullRetLayoutConfigurationB(etl::nullopt);
    bool retB = codec.decode({reinterpret_cast<const std::byte*>(inputB), sizeof(inputB)},
                             etl::nullopt,
                             nullRetLayoutConfigurationB);
    EXPECT_FALSE(retB);
}

TEST_F(CborLayoutConfigurationCodecTest, InvalidLayoutHash)
{
    // Length 15
    const uint8_t inputA[] = {0xa4, 0x01, 0x4f, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                              0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0x50,
                              0xab, 0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75,
                              0x00, 0xa3, 0x14, 0x12, 0x0d, 0xef, 0x03, 0x02, 0x04, 0x82,
                              0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06};
    // Length 17
    const uint8_t inputB[] = {0xa4, 0x01, 0x51, 0xff, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72,
                              0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d,
                              0x02, 0x50, 0xab, 0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                              0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x0d, 0xef, 0x03, 0x02,
                              0x04, 0x82, 0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x066};
    OptionalReturn<LayoutConfiguration> nullRetLayoutConfigurationA(etl::nullopt);
    bool retA = codec.decode({reinterpret_cast<const std::byte*>(inputA), sizeof(inputA)},
                             etl::nullopt,
                             nullRetLayoutConfigurationA);
    EXPECT_FALSE(retA);

    OptionalReturn<LayoutConfiguration> nullRetLayoutConfigurationB(etl::nullopt);
    bool retB = codec.decode({reinterpret_cast<const std::byte*>(inputB), sizeof(inputB)},
                             etl::nullopt,
                             nullRetLayoutConfigurationB);
    EXPECT_FALSE(retB);
}

TEST_F(CborLayoutConfigurationCodecTest, InvalidAssociatedHidLayoutConfigurationId)
{
    // Length 15
    const uint8_t inputA[] = {0xa4, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a,
                              0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02,
                              0x4f, 0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75,
                              0x00, 0xa3, 0x14, 0x12, 0x0d, 0xef, 0x03, 0x02, 0x04, 0x82,
                              0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06};
    // Length 17
    const uint8_t inputB[] = {0xa4, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a,
                              0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02,
                              0x51, 0xff, 0xab, 0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                              0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x0d, 0xef, 0x03, 0x02,
                              0x04, 0x82, 0x83, 0x01, 0x02, 0x03, 0x83, 0x04, 0x05, 0x06};
    OptionalReturn<LayoutConfiguration> nullRetLayoutConfigurationA(etl::nullopt);
    bool retA = codec.decode({reinterpret_cast<const std::byte*>(inputA), sizeof(inputA)},
                             etl::nullopt,
                             nullRetLayoutConfigurationA);
    EXPECT_FALSE(retA);

    OptionalReturn<LayoutConfiguration> nullRetLayoutConfigurationB(etl::nullopt);
    bool retB = codec.decode({reinterpret_cast<const std::byte*>(inputB), sizeof(inputB)},
                             etl::nullopt,
                             nullRetLayoutConfigurationB);
    EXPECT_FALSE(retB);
}

TEST_F(CborLayoutConfigurationCodecTest, EmptySequences)
{
    const uint8_t input[] = {0xa5, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                             0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0x50, 0xab,
                             0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3,
                             0x14, 0x12, 0x0d, 0xef, 0x03, 0x02, 0x04, 0x80, 0x05, 0x80};
    etl::vector<KeySequence, 1> expectedWinSas{};
    etl::vector<KeySequence, 1> expectedCapslock{};

    etl::optional<LayoutConfiguration> layoutConfiguration;
    OptionalReturn<LayoutConfiguration> retLayoutConfiguration =
        createOptionalReturn<LayoutConfiguration>(layoutConfiguration);

    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            id,
                            retLayoutConfiguration);

    EXPECT_TRUE(ret);
    ASSERT_TRUE(layoutConfiguration.has_value());

    ASSERT_TRUE(layoutConfiguration.value().getAssociatedHidLayoutConfigurationId().has_value());

    EXPECT_EQ(layoutConfiguration.value().getId(), id);
    EXPECT_EQ(layoutConfiguration.value().getLayoutHash(), layoutHash);
    EXPECT_EQ(layoutConfiguration.value().getAssociatedHidLayoutConfigurationId().value().get(),
              associatedHidLayoutConfigurationId);
    EXPECT_EQ(layoutConfiguration.value().getOperatingSystem(), OperatingSystem::MAC);
    EXPECT_EQ(layoutConfiguration.value().getWinSasSequence(), expectedWinSas);
    EXPECT_EQ(layoutConfiguration.value().getCapslockKeySequences(), expectedCapslock);
}

TEST_F(CborLayoutConfigurationCodecTest, EmptyWinSasKeySequence)
{
    const uint8_t input[] = {0xa4, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                             0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0x50, 0xab, 0xce, 0x8c,
                             0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x0d,
                             0xef, 0x03, 0x02, 0x04, 0x82, 0x83, 0x01, 0x02, 0x03, 0x80};
    OptionalReturn<LayoutConfiguration> nullRetLayoutConfiguration(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            etl::nullopt,
                            nullRetLayoutConfiguration);
    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutConfigurationCodecTest, MaxWinSasSequences)
{
    const uint8_t input[] = {0xa4, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf,
                             0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0x50, 0xab, 0xce, 0x8c,
                             0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x0d,
                             0xef, 0x03, 0x02, 0x04, 0x8f, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81,
                             0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81,
                             0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01};

    etl::vector<KeySequence, LayoutConfiguration::MAX_WIN_SAS_SEQUENCES> expectedWinSas{};

    for(size_t i = 0; i < expectedWinSas.capacity(); ++i)
    {
        expectedWinSas.push_back(KeySequence{1});
    }

    etl::optional<LayoutConfiguration> layoutConfiguration;
    OptionalReturn<LayoutConfiguration> retLayoutConfiguration =
        createOptionalReturn<LayoutConfiguration>(layoutConfiguration);

    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            id,
                            retLayoutConfiguration);

    EXPECT_TRUE(ret);
    ASSERT_TRUE(layoutConfiguration.has_value());

    ASSERT_TRUE(layoutConfiguration.value().getAssociatedHidLayoutConfigurationId().has_value());

    EXPECT_EQ(layoutConfiguration.value().getId(), id);
    EXPECT_EQ(layoutConfiguration.value().getLayoutHash(), layoutHash);
    EXPECT_EQ(layoutConfiguration.value().getAssociatedHidLayoutConfigurationId().value().get(),
              associatedHidLayoutConfigurationId);
    EXPECT_EQ(layoutConfiguration.value().getOperatingSystem(), OperatingSystem::MAC);
    EXPECT_EQ(layoutConfiguration.value().getWinSasSequence(), expectedWinSas);
}

TEST_F(CborLayoutConfigurationCodecTest, TooMuchWinSasSequences)
{
    const uint8_t input[] = {0xa4, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                             0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0x50, 0xab,
                             0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3,
                             0x14, 0x12, 0x0d, 0xef, 0x03, 0x02, 0x04, 0x90, 0x81, 0x01, 0x81,
                             0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01,
                             0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01, 0x81,
                             0x01, 0x81, 0x01, 0x81, 0x01, 0x81, 0x01};

    OptionalReturn<LayoutConfiguration> nullRetLayoutConfiguration(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            etl::nullopt,
                            nullRetLayoutConfiguration);
    EXPECT_FALSE(ret);
}

TEST_F(CborLayoutConfigurationCodecTest, TooMuchCapslockSequences)
{
    const uint8_t input[] = {0xa5, 0x01, 0x50, 0x80, 0x7e, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39,
                             0xbf, 0x75, 0x00, 0xa3, 0x14, 0x12, 0x00, 0x0d, 0x02, 0x50, 0xab,
                             0xce, 0x8c, 0xb0, 0xe8, 0x72, 0x4a, 0x39, 0xbf, 0x75, 0x00, 0xa3,
                             0x14, 0x12, 0x0d, 0xef, 0x03, 0x02, 0x04, 0x82, 0x83, 0x01, 0x02,
                             0x03, 0x83, 0x04, 0x05, 0x06, 0x05, 0x82, 0x81, 0x0a, 0x81, 0x0b};

    OptionalReturn<LayoutConfiguration> nullRetLayoutConfiguration(etl::nullopt);
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)},
                            etl::nullopt,
                            nullRetLayoutConfiguration);
    EXPECT_FALSE(ret);
}