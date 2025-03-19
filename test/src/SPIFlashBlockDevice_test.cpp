#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "gtest_helpers.hpp"

#include "FlashBlockDevice.hpp"
#include "SFlashIOCommonInterface.hpp"
#include "SPISFlashCommunicationInterface.hpp"
#include "SFlashConfigurationCommonInterface.hpp"
#include "MX25LFlash.hpp"
#include "MockGPIOController.hpp"
#include "MockSPI.hpp"
#include "MockSystem.hpp"
#include "DummyRecursiveMutex.hpp"
#include <array>

using ::testing::_;
using ::testing::Args;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::ElementsAreArray;
using ::testing::InSequence;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgPointee;

class flashBlockDeviceTest : public ::testing::Test
{
protected:
    flashBlockDeviceTest()
        : chipSelector(gpioController, DUMMY_CS_PIN)
        , spiInterface(spi, chipSelector, system)
        , flash(FLASH_TEST_INFO, spiInterface)
        , flashBlockDevice(flash){};

    void initNominal() { EXPECT_EQ(flash.init(), true); }

    void eraseSectorNominal(bd_addr_t address, bd_size_t size)
    {
        std::array<uint8_t, 1> opCodeArrayWriteEnable = {
            static_cast<uint8_t>(OpCode::WRITE_ENABLE_WREN)};
        std::array<uint8_t, 1> opCodeArraySectorErase = {
            static_cast<uint8_t>(FLASH_TEST_LUT.eraseCommand)};
        std::array<uint8_t, 1> opCodeArrayStatusRegister = {
            static_cast<uint8_t>(OpCode::READ_STATUS_REGISTER_RDSR)};

        {
            InSequence s;
            for(int i = 0; i < size / SECTOR_SIZE_BYTES; ++i)
            {
                EXPECT_CALL(spi, transmit(_, _, _, true))
                    .With(Args<0, 1>(ElementsAreArray(opCodeArrayWriteEnable)))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, transmit(_, _, _, false))
                    .With(Args<0, 1>(ElementsAreArray(opCodeArrayStatusRegister)))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, receive)
                    .Times(2)
                    .WillRepeatedly(DoAll(SetArgPointee<0>(STATUS_REG_WRITE_ENABLE),
                                          Return(ISPI_ERROR_SUCCESS)));

                EXPECT_CALL(spi, transmit(_, _, _, false))
                    .With(Args<0, 1>(ElementsAreArray(opCodeArraySectorErase)))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, transmit(_, _, _, true))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, transmit(_, _, _, false))
                    .With(Args<0, 1>(ElementsAreArray(opCodeArrayStatusRegister)))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, receive)
                    .Times(2)
                    .WillRepeatedly(DoAll(SetArgPointee<0>(STATUS_REG_NO_WRITE_IN_PROGRESS),
                                          Return(ISPI_ERROR_SUCCESS)));
            }
        }

        EXPECT_EQ(flashBlockDevice.erase(address, size), BD_ERROR_OK);
    }

    void programNominal(uint8_t* buffer, bd_addr_t address, bd_size_t size)
    {
        std::array<uint8_t, 1> opCodeArrayWriteEnable = {
            static_cast<uint8_t>(OpCode::WRITE_ENABLE_WREN)};
        std::array<uint8_t, 1> opCodeArrayPageProgram = {
            static_cast<uint8_t>(FLASH_TEST_LUT.writePageCommand)};
        std::array<uint8_t, 1> opCodeArrayStatusRegister = {
            static_cast<uint8_t>(OpCode::READ_STATUS_REGISTER_RDSR)};

        {
            InSequence s;

            for(int i = 0; i < size / PAGE_SIZE_BYTES; ++i)
            {
                EXPECT_CALL(spi, transmit(_, _, _, true))
                    .With(Args<0, 1>(ElementsAreArray(opCodeArrayWriteEnable)))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, transmit(_, _, _, false))
                    .With(Args<0, 1>(ElementsAreArray(opCodeArrayStatusRegister)))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, receive)
                    .Times(2)
                    .WillRepeatedly(DoAll(SetArgPointee<0>(STATUS_REG_WRITE_ENABLE),
                                          Return(ISPI_ERROR_SUCCESS)));

                EXPECT_CALL(spi, transmit(_, _, _, false))
                    .With(Args<0, 1>(ElementsAreArray(opCodeArrayPageProgram)))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, transmit(_, _, _, false))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, transmit(_, PAGE_SIZE_BYTES, _, true))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, transmit(_, _, _, false))
                    .With(Args<0, 1>(ElementsAreArray(opCodeArrayStatusRegister)))
                    .Times(1)
                    .WillOnce(Return(ISPI_ERROR_SUCCESS));

                EXPECT_CALL(spi, receive)
                    .Times(2)
                    .WillRepeatedly(DoAll(SetArgPointee<0>(STATUS_REG_NO_WRITE_IN_PROGRESS),
                                          Return(ISPI_ERROR_SUCCESS)));
            }
        }


        EXPECT_EQ(flashBlockDevice.program(buffer, address, size), BD_ERROR_OK);
    }

    void waitEndSequence()
    {
        EXPECT_CALL(spi, transmit(NULL, 0, _, true)).Times(1).WillOnce(Return(ISPI_ERROR_SUCCESS));
    }

    NiceMock<MockGPIOController> gpioController;
    SingleChipSelector chipSelector;
    NiceMock<MockSPI> spi;
    DummyRecursiveMutex mutex;
    NiceMock<MockSystem> system;
    SPISFlashCommunicationInterface spiInterface;
    MX25LFlash flash; /* Use ITE SPI flash */
    FlashBlockDevice flashBlockDevice;

    static constexpr FlashInfo FLASH_TEST_INFO = {256, 256, 4 * 1024, MO_TO_BYTES(16)};
    static constexpr FlashIOLut FLASH_TEST_LUT = {
        .readCommand = static_cast<uint8_t>(OpCode::READ_HIGHER_SPEED_FAST_READ),
        .writePageCommand = static_cast<uint8_t>(OpCode::PAGE_PROGRAM_PP),
        .eraseCommand = static_cast<uint8_t>(OpCode::SECTOR_ERASE_SE),
        .readCommandDummy = 1,
    };

    static constexpr GPIOPin DUMMY_CS_PIN = PIN_0;

    static constexpr uint32_t PAGE_SIZE_BYTES = FLASH_TEST_INFO.mPageSize;
    static constexpr uint32_t SECTOR_SIZE_BYTES = FLASH_TEST_INFO.mEraseSize;
    static constexpr uint8_t NB_READ_DUMMY_CYCLES = FLASH_TEST_LUT.readCommandDummy;

    static constexpr uint8_t STATUS_REG_NO_WRITE_IN_PROGRESS = 0x00;
    static constexpr uint8_t STATUS_REG_WRITE_IN_PROGRESS = 0x01;
    static constexpr uint8_t STATUS_REG_WRITE_DISABLE = 0x00;
    static constexpr uint8_t STATUS_REG_WRITE_ENABLE = 0x02;
};

TEST_F(flashBlockDeviceTest, InitNominal)
{
    initNominal();
}

TEST_F(flashBlockDeviceTest, DeinitNominal)
{
    EXPECT_EQ(flash.deinit(), true);
}

TEST_F(flashBlockDeviceTest, ReadNominal)
{
    initNominal();

    uint8_t dummyBuffer[SECTOR_SIZE_BYTES] = {0};
    bd_addr_t address = 0x1C800; //SECTOR_SIZE_BYTES * 10;
    std::array<uint8_t, 3> addressArray = {static_cast<uint8_t>((address & 0xFF0000) >> 16),
                                           static_cast<uint8_t>((address & 0xFF00) >> 8),
                                           static_cast<uint8_t>((address & 0xFF))};

    {
        InSequence s;
        std::array<uint8_t, 1> opCodeArray = {static_cast<uint8_t>(FLASH_TEST_LUT.readCommand)};

        EXPECT_CALL(spi, transmit(_, _, _, false))
            .With(Args<0, 1>(ElementsAreArray(opCodeArray)))
            .Times(1)
            .WillOnce(Return(ISPI_ERROR_SUCCESS));

        EXPECT_CALL(spi, transmit(_, _, _, false))
            .With(Args<0, 1>(ElementsAreArray(addressArray)))
            .Times(1)
            .WillOnce(Return(ISPI_ERROR_SUCCESS));

        EXPECT_CALL(spi, transmit(_, sizeof(uint8_t), _, false))
            .Times(NB_READ_DUMMY_CYCLES)
            .WillRepeatedly(Return(ISPI_ERROR_SUCCESS));

        EXPECT_CALL(spi, receive(dummyBuffer, sizeof(dummyBuffer), _, true, false))
            .Times(1)
            .WillOnce(Return(ISPI_ERROR_SUCCESS));
    }

    EXPECT_EQ(flashBlockDevice.read(dummyBuffer, address, sizeof(dummyBuffer)), BD_ERROR_OK);
}

TEST_F(flashBlockDeviceTest, ReadNotMultipleOfPage)
{
    initNominal();

    uint8_t dummyBuffer[SECTOR_SIZE_BYTES] = {0};
    bd_addr_t address = 0x010203;

    EXPECT_ASSERTION_FAILED(flashBlockDevice.read(dummyBuffer, address, sizeof(dummyBuffer)));
}

TEST_F(flashBlockDeviceTest, ReadSPIError)
{
    initNominal();

    uint8_t dummyBuffer[SECTOR_SIZE_BYTES] = {0};
    bd_addr_t address = 0x1C800;

    EXPECT_CALL(spi, transmit).Times(1).WillOnce(Return(ISPI_ERROR_UNKNOWN));

    waitEndSequence();

    EXPECT_EQ(flashBlockDevice.read(dummyBuffer, address, sizeof(dummyBuffer)),
              BD_ERROR_DEVICE_ERROR);
}

TEST_F(flashBlockDeviceTest, EraseNominal)
{
    initNominal();

    bd_addr_t address = 0x1000;

    eraseSectorNominal(address, 4096 * 3);
}

TEST_F(flashBlockDeviceTest, EraseWriteEnableTimeout)
{
    initNominal();

    bd_addr_t address = 0x1000;

    std::array<uint8_t, 1> opCodeArrayWriteEnable = {
        static_cast<uint8_t>(OpCode::WRITE_ENABLE_WREN)};
    std::array<uint8_t, 1> opCodeArrayStatusRegister = {
        static_cast<uint8_t>(OpCode::READ_STATUS_REGISTER_RDSR)};


    EXPECT_CALL(spi, transmit(_, _, _, true))
        .With(Args<0, 1>(ElementsAreArray(opCodeArrayWriteEnable)))
        .WillRepeatedly(Return(ISPI_ERROR_SUCCESS));

    EXPECT_CALL(spi, transmit(_, _, _, false))
        .With(Args<0, 1>(ElementsAreArray(opCodeArrayStatusRegister)))
        .WillRepeatedly(Return(ISPI_ERROR_SUCCESS));

    EXPECT_CALL(spi, receive)
        .Times(AtLeast(2))
        .WillRepeatedly(
            DoAll(SetArgPointee<0>(STATUS_REG_WRITE_DISABLE), Return(ISPI_ERROR_SUCCESS)));


    EXPECT_EQ(flashBlockDevice.erase(address, SECTOR_SIZE_BYTES), BD_ERROR_DEVICE_ERROR);
}

TEST_F(flashBlockDeviceTest, EraseWaitMemReadyTimeout)
{
    initNominal();

    bd_addr_t address = 0x1000;

    std::array<uint8_t, 1> opCodeArrayWriteEnable = {
        static_cast<uint8_t>(OpCode::WRITE_ENABLE_WREN)};
    std::array<uint8_t, 1> opCodeArraySectorErase = {static_cast<uint8_t>(OpCode::SECTOR_ERASE_SE)};
    std::array<uint8_t, 1> opCodeArrayStatusRegister = {
        static_cast<uint8_t>(OpCode::READ_STATUS_REGISTER_RDSR)};

    {
        InSequence s;

        EXPECT_CALL(spi, transmit(_, _, _, true))
            .With(Args<0, 1>(ElementsAreArray(opCodeArrayWriteEnable)))
            .Times(1)
            .WillOnce(Return(ISPI_ERROR_SUCCESS));

        EXPECT_CALL(spi, transmit(_, _, _, false))
            .With(Args<0, 1>(ElementsAreArray(opCodeArrayStatusRegister)))
            .Times(1)
            .WillOnce(Return(ISPI_ERROR_SUCCESS));

        EXPECT_CALL(spi, receive)
            .Times(2)
            .WillRepeatedly(
                DoAll(SetArgPointee<0>(STATUS_REG_WRITE_ENABLE), Return(ISPI_ERROR_SUCCESS)));

        EXPECT_CALL(spi, transmit(_, _, _, false))
            .With(Args<0, 1>(ElementsAreArray(opCodeArraySectorErase)))
            .Times(1)
            .WillOnce(Return(ISPI_ERROR_SUCCESS));

        EXPECT_CALL(spi, transmit(_, _, _, true)).Times(1).WillOnce(Return(ISPI_ERROR_SUCCESS));

        EXPECT_CALL(spi, transmit(_, _, _, false))
            .With(Args<0, 1>(ElementsAreArray(opCodeArrayStatusRegister)))
            .Times(1)
            .WillOnce(Return(ISPI_ERROR_SUCCESS));

        EXPECT_CALL(spi, receive)
            .Times(AtLeast(3))
            .WillRepeatedly(
                DoAll(SetArgPointee<0>(STATUS_REG_WRITE_IN_PROGRESS), Return(ISPI_ERROR_SUCCESS)));
    }

    EXPECT_EQ(flashBlockDevice.erase(address, SECTOR_SIZE_BYTES), BD_ERROR_DEVICE_ERROR);
}

TEST_F(flashBlockDeviceTest, ProgramNominal)
{
    initNominal();

    constexpr bd_addr_t SIZE = PAGE_SIZE_BYTES * 7;
    uint8_t dummyBuffer[SIZE] = {0};
    bd_addr_t address = 0x1000;

    programNominal(dummyBuffer, address, SIZE);
}
