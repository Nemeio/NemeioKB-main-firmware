/*
 * IT8951ScreenController.hpp
 *
 *  Created on: 21 oct. 2021 
 *      Author: thomas
 */

#ifndef NEMEIO_DISPLAY_SCREENCONTROLLER_IT8951SCREENCONTROLLER_HPP_
#define NEMEIO_DISPLAY_SCREENCONTROLLER_IT8951SCREENCONTROLLER_HPP_

#include "IT8951ScreenControllerCommands.hpp"
#include "IT8951ScreenControllerTypes.hpp"
#include <optional>
#include "IScreenController.hpp"
#include "iSPI.h"
#include "IScreen.hpp"
#include "SingleChipSelector.h"
#include "Signal.hpp"
#include "STM32Watchdog.hpp"
#include "etl/array_view.h"

#define SPI_MSB
//#define SPI_LSB

class IT8951ScreenController
{
public:
    struct IT8951IO
    {
        GPIOPin hRdyPin;
        GPIOPin onPin;
        GPIOPin rstNPin;

        IT8951IO(GPIOPin hRdyPin_, GPIOPin onPin_, GPIOPin rstNPin_)
            : hRdyPin(hRdyPin_)
            , onPin(onPin_)
            , rstNPin(rstNPin_)
        {
        }
    };

    IT8951ScreenController(iSPI& iSPI,
                           SingleChipSelector& chipSelector,
                           IGPIOController& gpioController,
                           IT8951IO& screenIO,
                           STM32Watchdog& watchdog);
    virtual ~IT8951ScreenController() = default;

    /* IScreenController */
    ScreenDefs::ScreenErrCode init();
    ScreenDefs::ScreenErrCode getVersions(char* firmwareVersionBuffer,
                                          std::size_t firmwareVersionBufferLen,
                                          char* lutVersionBuffer,
                                          std::size_t lutVersionBufferLen) const;

    ScreenDefs::ScreenErrCode display(IScreen& screen,
                                      IFrameBuffer& frameBuffer,
                                      const IScreenController::DisplayOptions& displayOptions =
                                          {eDisplayMode::FULL,
                                           ScreenDefs::BackgroundColor::UNKNOWN});
    ScreenDefs::ScreenErrCode clearScreen(IScreen& screen);

    void setPower(bool powerOn);

    ScreenDefs::ScreenErrCode wakeUp();
    ScreenDefs::ScreenErrCode goToLowPowerMode();

    /* IT8951 specific */
    ScreenDefs::ScreenErrCode setLimitedRefresh(bool enable);
    void forceCleanAfterNFastDisplay(uint8_t displayCount);
    void forceCleanAfterNBlackBackgroundDisplay(uint8_t displayCount);

    ScreenDefs::ScreenInfo getScreenInfo();
    IT8951DWord getRawBppFrameBuffAddrFromIndex(IScreen& screen, uint8_t frameBufferIndex);
    IT8951DWord getJpegFrameBuffAddrFromIndex(IScreen& screen, uint8_t frameBufferIndex);
    void holdReset();

    ScreenDefs::ScreenErrCode sendCommand(IT8951Word command);
    ScreenDefs::ScreenErrCode sendCommandWithArgs(IT8951Word command,
                                                  IT8951Word args[],
                                                  uint32_t argsWordCount);
    ScreenDefs::ScreenErrCode sendData(IT8951Word data[], uint32_t wordCount);
    ScreenDefs::ScreenErrCode sendImgData(IT8951Word data[], uint32_t wordCount);
    ScreenDefs::ScreenErrCode sendConstImgData(const IT8951Word data[],
                                               IT8951Word* copyBuff,
                                               uint32_t wordCount);
    ScreenDefs::ScreenErrCode startSendImgData();
    ScreenDefs::ScreenErrCode doSendJpegImgData(IT8951Word data[], uint32_t wordCount);
    ScreenDefs::ScreenErrCode doSendNBppImgData(IT8951Word data[], uint32_t wordCount);
    ScreenDefs::ScreenErrCode doSendConstImgData(const IT8951Word data[],
                                                 uint32_t dataLength,
                                                 IT8951Word* copyBuff,
                                                 uint32_t wordCount,
                                                 uint8_t pixelShift);
    ScreenDefs::ScreenErrCode endSendImgData();
    ScreenDefs::ScreenErrCode readData(IT8951Word data[], uint32_t wordCount);
    ScreenDefs::ScreenErrCode writeReg(IT8951Word regAddr, IT8951Word regValue);
    ScreenDefs::ScreenErrCode readReg(IT8951Word regAddr, IT8951Word& regValue);
    ScreenDefs::ScreenErrCode readRegs(IT8951Word regBaseAddr,
                                       etl::array_view<IT8951Word> readValues);
    ScreenDefs::ScreenErrCode enablePackedMode();

    ScreenDefs::ScreenErrCode startMemBurst(IT8951DWord memAddr, IT8951DWord writeSize);
    ScreenDefs::ScreenErrCode endSendBurst();
    ScreenDefs::ScreenErrCode clearContent(IScreen& screen,
                                           bool clearBufferContent,
                                           bool refreshScreen = false);

    ScreenDefs::ScreenErrCode waitForDisplayReady();

    /* Utils */
    inline IT8951Word reverseWord(IT8951Word word);

private:
    struct DeviceSystemInfo
    {
        uint16_t panelWidth;
        uint16_t panelHeight;
        uint32_t imgBufAddr;
        uint32_t jpegBufAddr;
        uint16_t FWVersionStr[USDEF_CMD_GET_DEV_INFO_FW_STR_WORDS];
        uint16_t LUTVersionStr[USDEF_CMD_GET_DEV_INFO_LUT_STR_WORDS];
    };

    bool m_bInitialized = false;

    iSPI& m_iSPI;
    const SPIConfig m_commandSpiConfig = {
        2500000,
        ISPI_DATASIZE_16BIT, // 16-bit data size
#ifdef SPI_LSB
        ISPI_FIRSTBIT_LSB,
#else
        ISPI_FIRSTBIT_MSB,
#endif
        ISPI_POLARITY_LOW, // CPOL = 0
        ISPI_PHASE_1EDGE   // CPHA = 0
    };

    const SPIConfig m_dataSpiConfig = {
        20000000,
        ISPI_DATASIZE_16BIT, // 16-bit data size
#ifdef SPI_LSB
        ISPI_FIRSTBIT_LSB,
#else
        ISPI_FIRSTBIT_MSB,
#endif
        ISPI_POLARITY_LOW, // CPOL = 0
        ISPI_PHASE_1EDGE   // CPHA = 0
    };

    SingleChipSelector& m_csNSelector;
    IGPIOController& m_gpioController;
    IT8951IO m_screenIO;
    Signal mHostReadySignal;
    STM32Watchdog& m_stm32Watchdog;


#ifdef NO_BOOTLOADER
    static constexpr bool FIRST_POWER_UP_BY_BOOTLOADER = false;
#else
    static constexpr bool FIRST_POWER_UP_BY_BOOTLOADER = true;
#endif
    bool m_bFirstPowerUpByBootloader = FIRST_POWER_UP_BY_BOOTLOADER;

    static constexpr int32_t HOST_READY_TIMEOUT_MS = 2000;

    static constexpr IT8951Word BLACK_1BPP_COLOR = 0x00;
    static constexpr IT8951Word WHITE_1BPP_COLOR = 0xFF;

    DeviceSystemInfo m_deviceSystemInfo;

    /* Hack for Holitech screen */
    uint8_t mCleanRefreshPeriod{1};
    uint8_t mFastRefreshCount{0};

    uint8_t mBlackBackgroundCleanRefreshPeriod{10};
    uint8_t mBlackBackgroundRefreshCount{0};

    ScreenDefs::ScreenErrCode getDeviceSystemInfo(DeviceSystemInfo& deviceSystemInfo);
    ScreenDefs::ScreenErrCode initTemperature();
    ScreenDefs::ScreenErrCode getTemperature(IT8951Word& userSetTemperature,
                                             IT8951Word& realTemperature);

    ScreenDefs::ScreenErrCode bppToPixelFormat(uint8_t bpp, uint8_t& it8951PixelFormatCode);

    ScreenDefs::ScreenErrCode sendDataWaitHRDY(IT8951Word data[], uint32_t wordCount);
    ScreenDefs::ScreenErrCode waitForHostReady();
    void hostReadyCallback();
    void resetHostReadyState();
    ScreenDefs::ScreenErrCode enable1BppMode();
    ScreenDefs::ScreenErrCode disable1BppMode();
    ScreenDefs::ScreenErrCode setBitmapColorTable(IT8951Word backgroundColor,
                                                  IT8951Word foregroundColor);

    ScreenDefs::ScreenErrCode decodeJpeg(IScreen& screen, IT8951DWord jpegFrameBuffAddr);
    ScreenDefs::ScreenErrCode displayJpeg(IScreen& screen,
                                          IT8951DWord frameBuffAddr,
                                          uint8_t displayMode);
    ScreenDefs::ScreenErrCode display1Bpp(IScreen& screen,
                                          IT8951DWord frameBuffAddr,
                                          uint8_t displayMode);
    ScreenDefs::ScreenErrCode displayNBpp(IScreen& screen,
                                          IT8951DWord frameBuffAddr,
                                          uint8_t displayMode);

    /* Transmission */
    ScreenDefs::ScreenErrCode spiTransmit(const uint8_t data[],
                                          uint32_t wordCount,
                                          bool bEndTransfer = true);
    ScreenDefs::ScreenErrCode spiReceive(uint8_t data[],
                                         uint32_t wordCount,
                                         bool bEndTransfer = true);
};

#endif /* NEMEIO_DISPLAY_SCREENCONTROLLER_IT8951SCREENCONTROLLER_HPP_ */
