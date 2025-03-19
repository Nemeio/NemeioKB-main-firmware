#include "gtest/gtest.h"

#include "IT8951FirmwareUpdate.hpp"
#include "MockBlockDevice.hpp"
#include "MockUpdateListener.hpp"

using ::testing::_;
using ::testing::ElementsAreArray;
using ::testing::Eq;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::StrictMock;

class IT8951FirmwareUpdateTest : public ::testing::Test
{
protected:
    IT8951FirmwareUpdateTest()
        : it8951ControllerUpgrader(blockDevice)
    {
        ON_CALL(blockDevice, get_program_size).WillByDefault(Return(BD_PROGRAM_SIZE));
        ON_CALL(blockDevice, get_erase_size).WillByDefault(Return(BD_ERASE_SIZE));
        ON_CALL(blockDevice, size).WillByDefault(Return(BD_SIZE));
    };

    void initNominal()
    {
        EXPECT_CALL(blockDevice, init).Times(1).WillOnce(Return(BD_ERROR_OK));
        EXPECT_CALL(blockDevice, erase(_, EXPECTED_ERASE_SIZE))
            .Times(1)
            .WillOnce(Return(BD_ERROR_OK));

        EXPECT_EQ(it8951ControllerUpgrader.initUpdate(DEFAULT_UPGRADE_SIZE),
                  FirmwareUpdateDefs::ErrCode::SUCCESS);
    }

    static constexpr std::size_t BD_PROGRAM_SIZE = 256;
    static constexpr std::size_t BD_ERASE_SIZE = 4096;
    static constexpr std::size_t BD_SIZE = 4 * 1024 * 1024;
    static constexpr std::size_t DEFAULT_UPGRADE_SIZE = 577537;
    static constexpr std::size_t EXPECTED_ERASE_SIZE = DEFAULT_UPGRADE_SIZE / BD_ERASE_SIZE
                                                           * BD_ERASE_SIZE
                                                       + (DEFAULT_UPGRADE_SIZE % BD_ERASE_SIZE > 0
                                                              ? BD_ERASE_SIZE
                                                              : 0);
    static constexpr std::size_t EXPECTED_PROGRAM_COUNT = DEFAULT_UPGRADE_SIZE / BD_PROGRAM_SIZE
                                                          + (DEFAULT_UPGRADE_SIZE % BD_PROGRAM_SIZE
                                                                     > 0
                                                                 ? 1
                                                                 : 0);

    NiceMock<MockBlockDevice> blockDevice;
    IT8951FirmwareUpdate it8951ControllerUpgrader;
};

TEST_F(IT8951FirmwareUpdateTest, InitFailed)
{
    uint8_t dummyReadBuffer[BD_PROGRAM_SIZE] = {0};
    constexpr std::size_t UPGRADE_SIZE = 573952;

    EXPECT_CALL(blockDevice, init).Times(1).WillOnce(Return(BD_ERROR_DEVICE_ERROR));

    EXPECT_EQ(it8951ControllerUpgrader.initUpdate(UPGRADE_SIZE),
              FirmwareUpdateDefs::ErrCode::UNEXPECTED);
    EXPECT_EQ(it8951ControllerUpgrader.writeFirmware(dummyReadBuffer, sizeof(dummyReadBuffer)),
              FirmwareUpdateDefs::ErrCode::UNEXPECTED);
}

TEST_F(IT8951FirmwareUpdateTest, InitInvalidLength)
{
    constexpr std::size_t UPGRADE_SIZE = BD_SIZE + 1;

    EXPECT_EQ(it8951ControllerUpgrader.initUpdate(UPGRADE_SIZE),
              FirmwareUpdateDefs::ErrCode::UNEXPECTED);
}

TEST_F(IT8951FirmwareUpdateTest, UpgradeNominal)
{
    uint8_t dummyReadBuffer = 0;

    initNominal();

    EXPECT_CALL(blockDevice, program(_, _, BD_PROGRAM_SIZE))
        .Times(EXPECTED_PROGRAM_COUNT)
        .WillRepeatedly(Return(BD_ERROR_OK));

    for(uint32_t i = 0; i < DEFAULT_UPGRADE_SIZE; i++)
    {
        EXPECT_EQ(it8951ControllerUpgrader.writeFirmware(&dummyReadBuffer, 1),
                  FirmwareUpdateDefs::ErrCode::SUCCESS);
    }

    /* Flush remaining data in the buffer */
    EXPECT_EQ(it8951ControllerUpgrader.runUpdate(), FirmwareUpdateDefs::ErrCode::SUCCESS);
}

TEST_F(IT8951FirmwareUpdateTest, UpgradeProgramFailed)
{
    uint8_t dummyReadBuffer[BD_PROGRAM_SIZE] = {0};

    initNominal();

    EXPECT_CALL(blockDevice, program).Times(1).WillOnce(Return(BD_ERROR_DEVICE_ERROR));

    EXPECT_EQ(it8951ControllerUpgrader.writeFirmware(dummyReadBuffer, sizeof(dummyReadBuffer)),
              FirmwareUpdateDefs::ErrCode::WRITE);
}

TEST_F(IT8951FirmwareUpdateTest, DeinitNominal)
{
    initNominal();
    EXPECT_CALL(blockDevice, deinit).Times(1);

    EXPECT_EQ(it8951ControllerUpgrader.runUpdate(), FirmwareUpdateDefs::ErrCode::UNEXPECTED);
}
