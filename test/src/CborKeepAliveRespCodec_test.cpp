#include "gtest/gtest.h"
#include "CborKeepAliveRespCodec.hpp"
#include "MockKeyboardStatus.hpp"

using ::testing::Return;

class CborKeepAliveRespCodecTest : public ::testing::Test
{
public:
    CborKeepAliveRespCodecTest() = default;

    CborKeepAliveRespCodec codec;
    MockKeyboardStatus status;
};

TEST_F(CborKeepAliveRespCodecTest, Encode)
{
    const uint8_t expectedOutput[] = {0xa1, 0x01, 0xa2, 0x01, 0x18, 0x30, 0x02, 0xf5};

    etl::array<std::byte, 8> output;

    EXPECT_CALL(status, getLogSize).Times(1).WillOnce(Return(48));
    EXPECT_CALL(status, isErrorLogged).Times(1).WillOnce(Return(true));
    ssize_t size = codec.encode(status, output);

    EXPECT_GE(size, 0);
    EXPECT_EQ(etl::array_view<const std::byte>(output.begin(), size),
              etl::array_view<const std::byte>(reinterpret_cast<const std::byte*>(expectedOutput),
                                               sizeof(expectedOutput)));
}

TEST_F(CborKeepAliveRespCodecTest, EncodeTooSmallBuffer)
{
    etl::array<std::byte, 5> output;

    EXPECT_CALL(status, getLogSize).Times(1).WillOnce(Return(48));
    EXPECT_CALL(status, isErrorLogged).Times(1).WillOnce(Return(false));
    ssize_t size = codec.encode(status, output);

    EXPECT_LT(size, 0);
}