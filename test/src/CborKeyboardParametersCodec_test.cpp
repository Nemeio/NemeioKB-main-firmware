#include "gtest/gtest.h"
#include "CborKeyboardParametersCodec.hpp"

class CborKeyboardParametersCodecTest : public ::testing::Test
{
public:
    CborKeyboardParametersCodecTest() = default;

    CborKeyboardParametersCodec codec;
};

TEST_F(CborKeyboardParametersCodecTest, ValidInput)
{
    const uint8_t input[] = {0xb3, 0x01, 0x19, 0x05, 0xdc, 0x02, 0x19, 0x09, 0xc4, 0x03, 0x19, 0x03,
                             0xe8, 0x04, 0x19, 0x07, 0xd0, 0x05, 0x19, 0x0b, 0xb8, 0x06, 0x18, 0x32,
                             0x07, 0x14, 0x08, 0x19, 0x01, 0xf4, 0x09, 0x19, 0x02, 0x58, 0x0a, 0x05,
                             0x0b, 0xf4, 0x0c, 0x19, 0x04, 0xb0, 0x0d, 0x19, 0x04, 0x4c, 0x0e, 0x0f,
                             0x0f, 0x19, 0x07, 0x6c, 0x10, 0x19, 0x07, 0x9e, 0x11, 0xf5, 0x12, 0x04,
                             0x13, 0x83, 0x18, 0x2d, 0x18, 0x32, 0x18, 0x37};
    etl::vector<uint8_t, KeyboardParameters::BRIGHTNESS_STEP_LIST_MAX_SIZE> brightnessStepList{45,
                                                                                               50,
                                                                                               55};
    KeyboardParameters expectedKeyboardParameters;

    expectedKeyboardParameters.setInactiveTime(1500);
    expectedKeyboardParameters.setSleepTime(2500);
    expectedKeyboardParameters.setInactiveTimeUSBDisconnected(1000);
    expectedKeyboardParameters.setSleepTimeUSBDisconnected(2000);
    expectedKeyboardParameters.setPowerOffTimeUSBDisconnected(3000);
    expectedKeyboardParameters.setLedPowerActiveLevel(50);
    expectedKeyboardParameters.setLedPowerInactiveLevel(20);
    expectedKeyboardParameters.setButtonLongPressDelay(500);
    expectedKeyboardParameters.setButtonRepeatLongPressDelay(600);
    expectedKeyboardParameters.setCleanRefreshPeriod(5);
    expectedKeyboardParameters.setDemoMode(false);
    expectedKeyboardParameters.setLowBatteryBlinkOnDelayMs(1200);
    expectedKeyboardParameters.setLowBatteryBlinkOffDelayMs(1100);
    expectedKeyboardParameters.setLowBatteryLevelThresholdPercent(15);
    expectedKeyboardParameters.setBleBlinkOnDelayMs(1900);
    expectedKeyboardParameters.setBleBlinkOffDelayMs(1950);
    expectedKeyboardParameters.setHighQualityModifier(true);
    expectedKeyboardParameters.setBlackBackgroundCleanRefreshPeriod(4);
    expectedKeyboardParameters.setBrightnessStepList(brightnessStepList);

    KeyboardParameters parameters;
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)}, parameters);

    EXPECT_TRUE(ret);
    EXPECT_EQ(parameters, expectedKeyboardParameters);
}

TEST_F(CborKeyboardParametersCodecTest, Encode)
{
    const uint8_t expectedOutput[] = {0xb3, 0x01, 0x0c, 0x02, 0x18, 0x3c, 0x03, 0x0c, 0x04,
                                      0x18, 0x3c, 0x05, 0x19, 0x01, 0x2c, 0x06, 0x18, 0x32,
                                      0x07, 0x18, 0x32, 0x08, 0x19, 0x05, 0xdc, 0x09, 0x18,
                                      0xfa, 0x0a, 0x0a, 0x0b, 0xf4, 0x0c, 0x18, 0x64, 0x0d,
                                      0x19, 0x03, 0x84, 0x0e, 0x14, 0x0f, 0x18, 0xc8, 0x10,
                                      0x19, 0x07, 0x08, 0x11, 0xf4, 0x12, 0x0a, 0x13, 0x89,
                                      0x14, 0x18, 0x1e, 0x18, 0x28, 0x18, 0x32, 0x18, 0x3c,
                                      0x18, 0x46, 0x18, 0x50, 0x18, 0x5a, 0x18, 0x64};
    KeyboardParameters parameters;
    etl::array<std::byte, 71> output;
    ssize_t size = codec.encode(parameters, output);

    EXPECT_GE(size, 0);
    EXPECT_EQ(etl::array_view<const std::byte>(output.begin(), size),
              etl::array_view<const std::byte>(reinterpret_cast<const std::byte*>(expectedOutput),
                                               sizeof(expectedOutput)));
}

TEST_F(CborKeyboardParametersCodecTest, EncodeTooSmallBuffer)
{
    const uint8_t expectedOutput[] = {0xb3, 0x01, 0x0c, 0x02, 0x18, 0x3c, 0x03, 0x0c, 0x04,
                                      0x18, 0x3c, 0x05, 0x19, 0x01, 0x2c, 0x06, 0x18, 0x32,
                                      0x07, 0x18, 0x32, 0x08, 0x19, 0x05, 0xdc, 0x09, 0x18,
                                      0xfa, 0x0a, 0x0a, 0x0b, 0xf4, 0x0c, 0x18, 0x64, 0x0d,
                                      0x19, 0x03, 0x84, 0x0e, 0x14, 0x0f, 0x18, 0xc8, 0x10,
                                      0x19, 0x07, 0x08, 0x11, 0xf4, 0x12, 0x0a, 0x13, 0x89,
                                      0x14, 0x18, 0x1e, 0x18, 0x28, 0x18, 0x32, 0x18, 0x3c,
                                      0x18, 0x46, 0x18, 0x50, 0x18, 0x5a, 0x18, 0x64};
    KeyboardParameters parameters;
    etl::array<std::byte, 70> output;
    ssize_t size = codec.encode(parameters, output);

    EXPECT_LT(size, 0);
}

TEST_F(CborKeyboardParametersCodecTest, InputIncomplete)
{
    const uint8_t input[] = {0x01, 0x19, 0x05, 0xdc, 0x02, 0x19, 0x09, 0xc4, 0x03, 0x19, 0x03, 0xe8,
                             0x04, 0x19, 0x07, 0xd0, 0x05, 0x19, 0x0b, 0xb8, 0x06, 0x18, 0x32, 0x07,
                             0x14, 0x08, 0x19, 0x01, 0xf4, 0x09, 0x19, 0x02, 0x58, 0x0a, 0x05, 0x0b,
                             0xf4, 0x0c, 0x19, 0x04, 0xb0, 0x0d, 0x19, 0x04, 0x4c, 0x0e, 0x0f, 0x0f,
                             0x19, 0x07, 0x6c, 0x10, 0x19, 0x07, 0x9e, 0x11, 0xf5, 0x12, 0x04, 0x13,
                             0x83, 0x18, 0x2d, 0x18, 0x32, 0x18, 0x37};
    KeyboardParameters parameters;
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)}, parameters);

    EXPECT_FALSE(ret);
}

TEST_F(CborKeyboardParametersCodecTest, ValidInputNoBrightnessStepList)
{
    const uint8_t input[] = {0xb2, 0x01, 0x19, 0x05, 0xdc, 0x02, 0x19, 0x09, 0xc4, 0x03,
                             0x19, 0x03, 0xe8, 0x04, 0x19, 0x07, 0xd0, 0x05, 0x19, 0x0b,
                             0xb8, 0x06, 0x18, 0x32, 0x07, 0x14, 0x08, 0x19, 0x01, 0xf4,
                             0x09, 0x19, 0x02, 0x58, 0x0a, 0x05, 0x0b, 0xf4, 0x0c, 0x19,
                             0x04, 0xb0, 0x0d, 0x19, 0x04, 0x4c, 0x0e, 0x0f, 0x0f, 0x19,
                             0x07, 0x6c, 0x10, 0x19, 0x07, 0x9e, 0x11, 0xf5, 0x12, 0x04};
    KeyboardParameters expectedKeyboardParameters;

    expectedKeyboardParameters.setInactiveTime(1500);
    expectedKeyboardParameters.setSleepTime(2500);
    expectedKeyboardParameters.setInactiveTimeUSBDisconnected(1000);
    expectedKeyboardParameters.setSleepTimeUSBDisconnected(2000);
    expectedKeyboardParameters.setPowerOffTimeUSBDisconnected(3000);
    expectedKeyboardParameters.setLedPowerActiveLevel(50);
    expectedKeyboardParameters.setLedPowerInactiveLevel(20);
    expectedKeyboardParameters.setButtonLongPressDelay(500);
    expectedKeyboardParameters.setButtonRepeatLongPressDelay(600);
    expectedKeyboardParameters.setCleanRefreshPeriod(5);
    expectedKeyboardParameters.setDemoMode(false);
    expectedKeyboardParameters.setLowBatteryBlinkOnDelayMs(1200);
    expectedKeyboardParameters.setLowBatteryBlinkOffDelayMs(1100);
    expectedKeyboardParameters.setLowBatteryLevelThresholdPercent(15);
    expectedKeyboardParameters.setBleBlinkOnDelayMs(1900);
    expectedKeyboardParameters.setBleBlinkOffDelayMs(1950);
    expectedKeyboardParameters.setHighQualityModifier(true);
    expectedKeyboardParameters.setBlackBackgroundCleanRefreshPeriod(4);

    KeyboardParameters parameters;
    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)}, parameters);

    EXPECT_TRUE(ret);
    EXPECT_EQ(parameters, expectedKeyboardParameters);
}

TEST_F(CborKeyboardParametersCodecTest, ValidInputInactiveTimeUSBDisconnectedOnly)
{
    const uint8_t input[] = {0xa1, 0x03, 0x19, 0x03, 0xe8};
    KeyboardParameters parameters;
    parameters.setInactiveTime(1500);

    KeyboardParameters expectedKeyboardParameters(parameters);
    expectedKeyboardParameters.setInactiveTimeUSBDisconnected(1000);

    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)}, parameters);

    EXPECT_TRUE(ret);
    EXPECT_EQ(parameters, expectedKeyboardParameters);
}

TEST_F(CborKeyboardParametersCodecTest, MaxBrightnessStepList)
{
    const uint8_t input[] =
        {0xa1, 0x13, 0x99, 0x01, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
         0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
         0x18, 0x18, 0x19, 0x18, 0x1a, 0x18, 0x1b, 0x18, 0x1c, 0x18, 0x1d, 0x18, 0x1e, 0x18, 0x1f,
         0x18, 0x20, 0x18, 0x21, 0x18, 0x22, 0x18, 0x23, 0x18, 0x24, 0x18, 0x25, 0x18, 0x26, 0x18,
         0x27, 0x18, 0x28, 0x18, 0x29, 0x18, 0x2a, 0x18, 0x2b, 0x18, 0x2c, 0x18, 0x2d, 0x18, 0x2e,
         0x18, 0x2f, 0x18, 0x30, 0x18, 0x31, 0x18, 0x32, 0x18, 0x33, 0x18, 0x34, 0x18, 0x35, 0x18,
         0x36, 0x18, 0x37, 0x18, 0x38, 0x18, 0x39, 0x18, 0x3a, 0x18, 0x3b, 0x18, 0x3c, 0x18, 0x3d,
         0x18, 0x3e, 0x18, 0x3f, 0x18, 0x40, 0x18, 0x41, 0x18, 0x42, 0x18, 0x43, 0x18, 0x44, 0x18,
         0x45, 0x18, 0x46, 0x18, 0x47, 0x18, 0x48, 0x18, 0x49, 0x18, 0x4a, 0x18, 0x4b, 0x18, 0x4c,
         0x18, 0x4d, 0x18, 0x4e, 0x18, 0x4f, 0x18, 0x50, 0x18, 0x51, 0x18, 0x52, 0x18, 0x53, 0x18,
         0x54, 0x18, 0x55, 0x18, 0x56, 0x18, 0x57, 0x18, 0x58, 0x18, 0x59, 0x18, 0x5a, 0x18, 0x5b,
         0x18, 0x5c, 0x18, 0x5d, 0x18, 0x5e, 0x18, 0x5f, 0x18, 0x60, 0x18, 0x61, 0x18, 0x62, 0x18,
         0x63, 0x18, 0x64, 0x18, 0x65, 0x18, 0x66, 0x18, 0x67, 0x18, 0x68, 0x18, 0x69, 0x18, 0x6a,
         0x18, 0x6b, 0x18, 0x6c, 0x18, 0x6d, 0x18, 0x6e, 0x18, 0x6f, 0x18, 0x70, 0x18, 0x71, 0x18,
         0x72, 0x18, 0x73, 0x18, 0x74, 0x18, 0x75, 0x18, 0x76, 0x18, 0x77, 0x18, 0x78, 0x18, 0x79,
         0x18, 0x7a, 0x18, 0x7b, 0x18, 0x7c, 0x18, 0x7d, 0x18, 0x7e, 0x18, 0x7f, 0x18, 0x80, 0x18,
         0x81, 0x18, 0x82, 0x18, 0x83, 0x18, 0x84, 0x18, 0x85, 0x18, 0x86, 0x18, 0x87, 0x18, 0x88,
         0x18, 0x89, 0x18, 0x8a, 0x18, 0x8b, 0x18, 0x8c, 0x18, 0x8d, 0x18, 0x8e, 0x18, 0x8f, 0x18,
         0x90, 0x18, 0x91, 0x18, 0x92, 0x18, 0x93, 0x18, 0x94, 0x18, 0x95, 0x18, 0x96, 0x18, 0x97,
         0x18, 0x98, 0x18, 0x99, 0x18, 0x9a, 0x18, 0x9b, 0x18, 0x9c, 0x18, 0x9d, 0x18, 0x9e, 0x18,
         0x9f, 0x18, 0xa0, 0x18, 0xa1, 0x18, 0xa2, 0x18, 0xa3, 0x18, 0xa4, 0x18, 0xa5, 0x18, 0xa6,
         0x18, 0xa7, 0x18, 0xa8, 0x18, 0xa9, 0x18, 0xaa, 0x18, 0xab, 0x18, 0xac, 0x18, 0xad, 0x18,
         0xae, 0x18, 0xaf, 0x18, 0xb0, 0x18, 0xb1, 0x18, 0xb2, 0x18, 0xb3, 0x18, 0xb4, 0x18, 0xb5,
         0x18, 0xb6, 0x18, 0xb7, 0x18, 0xb8, 0x18, 0xb9, 0x18, 0xba, 0x18, 0xbb, 0x18, 0xbc, 0x18,
         0xbd, 0x18, 0xbe, 0x18, 0xbf, 0x18, 0xc0, 0x18, 0xc1, 0x18, 0xc2, 0x18, 0xc3, 0x18, 0xc4,
         0x18, 0xc5, 0x18, 0xc6, 0x18, 0xc7, 0x18, 0xc8, 0x18, 0xc9, 0x18, 0xca, 0x18, 0xcb, 0x18,
         0xcc, 0x18, 0xcd, 0x18, 0xce, 0x18, 0xcf, 0x18, 0xd0, 0x18, 0xd1, 0x18, 0xd2, 0x18, 0xd3,
         0x18, 0xd4, 0x18, 0xd5, 0x18, 0xd6, 0x18, 0xd7, 0x18, 0xd8, 0x18, 0xd9, 0x18, 0xda, 0x18,
         0xdb, 0x18, 0xdc, 0x18, 0xdd, 0x18, 0xde, 0x18, 0xdf, 0x18, 0xe0, 0x18, 0xe1, 0x18, 0xe2,
         0x18, 0xe3, 0x18, 0xe4, 0x18, 0xe5, 0x18, 0xe6, 0x18, 0xe7, 0x18, 0xe8, 0x18, 0xe9, 0x18,
         0xea, 0x18, 0xeb, 0x18, 0xec, 0x18, 0xed, 0x18, 0xee, 0x18, 0xef, 0x18, 0xf0, 0x18, 0xf1,
         0x18, 0xf2, 0x18, 0xf3, 0x18, 0xf4, 0x18, 0xf5, 0x18, 0xf6, 0x18, 0xf7, 0x18, 0xf8, 0x18,
         0xf9, 0x18, 0xfa, 0x18, 0xfb, 0x18, 0xfc, 0x18, 0xfd, 0x18, 0xfe, 0x18, 0xff};

    KeyboardParameters parameters;

    etl::vector<uint8_t, KeyboardParameters::BRIGHTNESS_STEP_LIST_MAX_SIZE> brightnessStepList;
    for(size_t i = 0; i < brightnessStepList.capacity(); ++i)
    {
        brightnessStepList.push_back(i);
    }

    KeyboardParameters expectedKeyboardParameters(parameters);
    expectedKeyboardParameters.setBrightnessStepList(brightnessStepList);

    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)}, parameters);

    EXPECT_TRUE(ret);
    EXPECT_EQ(parameters, expectedKeyboardParameters);
}

TEST_F(CborKeyboardParametersCodecTest, TooLongBrightnessStepList)
{
    const uint8_t input[] =
        {0xa1, 0x13, 0x99, 0x01, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
         0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
         0x18, 0x18, 0x19, 0x18, 0x1a, 0x18, 0x1b, 0x18, 0x1c, 0x18, 0x1d, 0x18, 0x1e, 0x18, 0x1f,
         0x18, 0x20, 0x18, 0x21, 0x18, 0x22, 0x18, 0x23, 0x18, 0x24, 0x18, 0x25, 0x18, 0x26, 0x18,
         0x27, 0x18, 0x28, 0x18, 0x29, 0x18, 0x2a, 0x18, 0x2b, 0x18, 0x2c, 0x18, 0x2d, 0x18, 0x2e,
         0x18, 0x2f, 0x18, 0x30, 0x18, 0x31, 0x18, 0x32, 0x18, 0x33, 0x18, 0x34, 0x18, 0x35, 0x18,
         0x36, 0x18, 0x37, 0x18, 0x38, 0x18, 0x39, 0x18, 0x3a, 0x18, 0x3b, 0x18, 0x3c, 0x18, 0x3d,
         0x18, 0x3e, 0x18, 0x3f, 0x18, 0x40, 0x18, 0x41, 0x18, 0x42, 0x18, 0x43, 0x18, 0x44, 0x18,
         0x45, 0x18, 0x46, 0x18, 0x47, 0x18, 0x48, 0x18, 0x49, 0x18, 0x4a, 0x18, 0x4b, 0x18, 0x4c,
         0x18, 0x4d, 0x18, 0x4e, 0x18, 0x4f, 0x18, 0x50, 0x18, 0x51, 0x18, 0x52, 0x18, 0x53, 0x18,
         0x54, 0x18, 0x55, 0x18, 0x56, 0x18, 0x57, 0x18, 0x58, 0x18, 0x59, 0x18, 0x5a, 0x18, 0x5b,
         0x18, 0x5c, 0x18, 0x5d, 0x18, 0x5e, 0x18, 0x5f, 0x18, 0x60, 0x18, 0x61, 0x18, 0x62, 0x18,
         0x63, 0x18, 0x64, 0x18, 0x65, 0x18, 0x66, 0x18, 0x67, 0x18, 0x68, 0x18, 0x69, 0x18, 0x6a,
         0x18, 0x6b, 0x18, 0x6c, 0x18, 0x6d, 0x18, 0x6e, 0x18, 0x6f, 0x18, 0x70, 0x18, 0x71, 0x18,
         0x72, 0x18, 0x73, 0x18, 0x74, 0x18, 0x75, 0x18, 0x76, 0x18, 0x77, 0x18, 0x78, 0x18, 0x79,
         0x18, 0x7a, 0x18, 0x7b, 0x18, 0x7c, 0x18, 0x7d, 0x18, 0x7e, 0x18, 0x7f, 0x18, 0x80, 0x18,
         0x81, 0x18, 0x82, 0x18, 0x83, 0x18, 0x84, 0x18, 0x85, 0x18, 0x86, 0x18, 0x87, 0x18, 0x88,
         0x18, 0x89, 0x18, 0x8a, 0x18, 0x8b, 0x18, 0x8c, 0x18, 0x8d, 0x18, 0x8e, 0x18, 0x8f, 0x18,
         0x90, 0x18, 0x91, 0x18, 0x92, 0x18, 0x93, 0x18, 0x94, 0x18, 0x95, 0x18, 0x96, 0x18, 0x97,
         0x18, 0x98, 0x18, 0x99, 0x18, 0x9a, 0x18, 0x9b, 0x18, 0x9c, 0x18, 0x9d, 0x18, 0x9e, 0x18,
         0x9f, 0x18, 0xa0, 0x18, 0xa1, 0x18, 0xa2, 0x18, 0xa3, 0x18, 0xa4, 0x18, 0xa5, 0x18, 0xa6,
         0x18, 0xa7, 0x18, 0xa8, 0x18, 0xa9, 0x18, 0xaa, 0x18, 0xab, 0x18, 0xac, 0x18, 0xad, 0x18,
         0xae, 0x18, 0xaf, 0x18, 0xb0, 0x18, 0xb1, 0x18, 0xb2, 0x18, 0xb3, 0x18, 0xb4, 0x18, 0xb5,
         0x18, 0xb6, 0x18, 0xb7, 0x18, 0xb8, 0x18, 0xb9, 0x18, 0xba, 0x18, 0xbb, 0x18, 0xbc, 0x18,
         0xbd, 0x18, 0xbe, 0x18, 0xbf, 0x18, 0xc0, 0x18, 0xc1, 0x18, 0xc2, 0x18, 0xc3, 0x18, 0xc4,
         0x18, 0xc5, 0x18, 0xc6, 0x18, 0xc7, 0x18, 0xc8, 0x18, 0xc9, 0x18, 0xca, 0x18, 0xcb, 0x18,
         0xcc, 0x18, 0xcd, 0x18, 0xce, 0x18, 0xcf, 0x18, 0xd0, 0x18, 0xd1, 0x18, 0xd2, 0x18, 0xd3,
         0x18, 0xd4, 0x18, 0xd5, 0x18, 0xd6, 0x18, 0xd7, 0x18, 0xd8, 0x18, 0xd9, 0x18, 0xda, 0x18,
         0xdb, 0x18, 0xdc, 0x18, 0xdd, 0x18, 0xde, 0x18, 0xdf, 0x18, 0xe0, 0x18, 0xe1, 0x18, 0xe2,
         0x18, 0xe3, 0x18, 0xe4, 0x18, 0xe5, 0x18, 0xe6, 0x18, 0xe7, 0x18, 0xe8, 0x18, 0xe9, 0x18,
         0xea, 0x18, 0xeb, 0x18, 0xec, 0x18, 0xed, 0x18, 0xee, 0x18, 0xef, 0x18, 0xf0, 0x18, 0xf1,
         0x18, 0xf2, 0x18, 0xf3, 0x18, 0xf4, 0x18, 0xf5, 0x18, 0xf6, 0x18, 0xf7, 0x18, 0xf8, 0x18,
         0xf9, 0x18, 0xfa, 0x18, 0xfb, 0x18, 0xfc, 0x18, 0xfd, 0x18, 0xfe, 0x18, 0xff, 0x18, 0xff};
    KeyboardParameters parameters;

    bool ret = codec.decode({reinterpret_cast<const std::byte*>(input), sizeof(input)}, parameters);

    EXPECT_FALSE(ret);
}