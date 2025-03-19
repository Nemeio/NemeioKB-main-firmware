/*
 * IT8951ScreenController.cpp
 *
 *  Created on: 21 oct. 2021
 *      Author: thomas
 */
#include "IT8951ScreenController.hpp"
#include <functional>

#include "timing_macros.h"
#include "TraceLogger.hpp"

#define MODULE "it8951"

constexpr uint8_t MAX_GET_DEV_INFO_ATTEMPTS = 5;
constexpr uint16_t PARAMETERS_REGISTER_BASE = 0x1264;
constexpr uint8_t PARAMETERS_REGISTER_WORDS_COUNT = 22;

#define SWAP_WORD(x) (uint16_t)(((x & 0xff00) >> 8) | ((x & 0x00ff) << 8))

#ifdef SPI_MSB
#define WORD_ADAPT(x) (x) //SWAP_WORD(x)
#define WORD_ADAPT_BPP_IMG(x) reverseWord(x)
#define WORD_ADAPT_JPEG_IMG(x) (x)
#define STR_ADAPT(x) SWAP_WORD(x)
#elif defined SPI_LSB
#define WORD_ADAPT(x) reverseWord(x)
#define WORD_ADAPT_BPP_IMG(x) (x)
#define WORD_ADAPT_JPEG_IMG(x) reverseWord(x)
#define STR_ADAPT(x) SWAP_WORD(x)
#else
#define WORD_ADAPT(x) (x)
#define STR_ADAPT(x) (x)
#endif

static const unsigned char BitReverseTable256[] =
    {0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70,
     0xF0, 0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8,
     0x78, 0xF8, 0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34,
     0xB4, 0x74, 0xF4, 0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC,
     0x3C, 0xBC, 0x7C, 0xFC, 0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52,
     0xD2, 0x32, 0xB2, 0x72, 0xF2, 0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A,
     0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA, 0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16,
     0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
     0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61,
     0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1, 0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9,
     0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 0x05, 0x85, 0x45, 0xC5, 0x25,
     0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5, 0x0D, 0x8D, 0x4D, 0xCD,
     0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, 0x03, 0x83, 0x43,
     0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 0x0B, 0x8B,
     0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB, 0x07,
     0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
     0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F,
     0xFF};


IT8951ScreenController::IT8951ScreenController(iSPI& iSPI,
                                               SingleChipSelector& chipSelector,
                                               IGPIOController& gpioController,
                                               IT8951IO& screenIO,
                                               STM32Watchdog& watchdog)
    : m_iSPI(iSPI)
    , m_csNSelector(chipSelector)
    , m_gpioController(gpioController)
    , m_screenIO(screenIO)
    , m_stm32Watchdog(watchdog)
{
}

ScreenDefs::ScreenErrCode IT8951ScreenController::init()
{
    ScreenDefs::ScreenErrCode ret = ScreenDefs::ScreenErrCode::UNEXPECTED;

    m_iSPI.init(m_commandSpiConfig);

    // Configure HOST_HREADY pin
    GPIOInConfig gpioInConfig;

    gpioInConfig.mode = IGPIO_INMODE_EXTINT;
    gpioInConfig.pull = IGPIO_PULL_DOWN;
    gpioInConfig.itMode = IGPIO_ITMODE_RISING;
    gpioInConfig.itCallback = std::bind(&IT8951ScreenController::hostReadyCallback, this);

    m_gpioController.configureInput(m_screenIO.hRdyPin, gpioInConfig);

    GPIOOutConfig gpioOutConfig;

    // Configure EPDC_ON pin
    gpioOutConfig.outType = IGPIO_OUTTYPE_PUSHPULL;
    gpioOutConfig.pull = IGPIO_NO_PULL;

    m_gpioController.configureOutput(m_screenIO.onPin, gpioOutConfig);

    // Configure EPDC_RST# pin
    gpioOutConfig.outType = IGPIO_OUTTYPE_PUSHPULL;
    gpioOutConfig.pull = IGPIO_NO_PULL;

    m_gpioController.configureOutput(m_screenIO.rstNPin, gpioOutConfig);


    if(!m_bFirstPowerUpByBootloader)
    {
        setPower(true);
    }

    m_bFirstPowerUpByBootloader = false;

    ret = getDeviceSystemInfo(m_deviceSystemInfo);

    if(ScreenDefs::ScreenErrCode::SUCCESS == ret)
    {
        IT8951Word up1sr2 = 0;
        ret = readReg(0x002C, up1sr2);
        if(ScreenDefs::ScreenErrCode::SUCCESS == ret)
        {
            ret = writeReg(0x002C, up1sr2 & ~(1 << 7));
        }
    }

    /* No pixel updates when pixel is not changed */
    /* This is used to avoid "white flash" when switching to two black background picture */
    if(ScreenDefs::ScreenErrCode::SUCCESS == ret)
    {
        IT8951Word up1sr = 0;
        ret = readReg(UP1SR + 2, up1sr);
        if(ScreenDefs::ScreenErrCode::SUCCESS == ret)
        {
            ret = writeReg(UP1SR + 2, up1sr | (1 << 3));
        }
    }

    if(ScreenDefs::ScreenErrCode::SUCCESS == ret)
    {
        ret = enablePackedMode();
    }

    if(ScreenDefs::ScreenErrCode::SUCCESS == ret)
    {
        m_bInitialized = true;
    }
    else
    {
        memset(&m_deviceSystemInfo, 0, sizeof(m_deviceSystemInfo));
    }

    TRACE_FUNC_EXIT();

    m_iSPI.deinit();
    m_iSPI.init(m_dataSpiConfig);

    return ret;
}


ScreenDefs::ScreenErrCode IT8951ScreenController::setLimitedRefresh(bool enable)
{
    IT8951Word up1sr = 0;
    ScreenDefs::ScreenErrCode ret = readReg(UP1SR + 2, up1sr);

    if(ScreenDefs::ScreenErrCode::SUCCESS == ret)
    {
        IT8951Word writeValue;

        if(enable)
        {
            writeValue = up1sr | (1 << 3);
        }
        else
        {
            writeValue = up1sr & ~(1 << 3);
        }

        if(writeValue != up1sr)
        {
            ret = writeReg(UP1SR + 2, writeValue);
        }
    }

    return ret;
}

ScreenDefs::ScreenInfo IT8951ScreenController::getScreenInfo()
{
    ScreenDefs::ScreenInfo infos = {
        .width = m_deviceSystemInfo.panelWidth,
        .height = m_deviceSystemInfo.panelHeight,
    };

    return infos;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::getVersions(char* firmwareVersionBuffer,
                                                              std::size_t firmwareVersionBufferLen,
                                                              char* lutVersionBuffer,
                                                              std::size_t lutVersionBufferLen) const
{
    if(!m_bInitialized)
    {
        return ScreenDefs::ScreenErrCode::INVALID_STATE;
    }

    strncpyNullTerminated(firmwareVersionBuffer,
                          reinterpret_cast<const char*>(m_deviceSystemInfo.FWVersionStr),
                          firmwareVersionBufferLen);
    strncpyNullTerminated(lutVersionBuffer,
                          reinterpret_cast<const char*>(m_deviceSystemInfo.LUTVersionStr),
                          lutVersionBufferLen);

    return ScreenDefs::ScreenErrCode::SUCCESS;
}

void IT8951ScreenController::hostReadyCallback()
{
    mHostReadySignal.setFromISR();
}

void IT8951ScreenController::resetHostReadyState()
{
    mHostReadySignal.clear();
}

ScreenDefs::ScreenErrCode IT8951ScreenController::getDeviceSystemInfo(
    DeviceSystemInfo& deviceSystemInfo)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    uint32_t deviceInfoRequestCount = 0;
    bool receiveError;

    etl::array<IT8951Word, PARAMETERS_REGISTER_WORDS_COUNT> IT8951RawDeviceSystemInfo;

    do
    {
        receiveError = false;
        ++deviceInfoRequestCount;

        m_stm32Watchdog.refresh();

        err = readRegs(PARAMETERS_REGISTER_BASE, IT8951RawDeviceSystemInfo);

        if(err != ScreenDefs::ScreenErrCode::SUCCESS)
        {
            receiveError = true;
            continue;
        }

        deviceSystemInfo.panelWidth = IT8951RawDeviceSystemInfo[0];
        deviceSystemInfo.panelHeight = IT8951RawDeviceSystemInfo[1];
        deviceSystemInfo.imgBufAddr = IT8951RawDeviceSystemInfo[2]
                                      | (IT8951RawDeviceSystemInfo[3] << 16);
        deviceSystemInfo.jpegBufAddr = IT8951RawDeviceSystemInfo[4]
                                       | (IT8951RawDeviceSystemInfo[5] << 16);

        for(uint8_t i = 0; i < USDEF_CMD_GET_DEV_INFO_FW_STR_WORDS; i++)
        {
            deviceSystemInfo.FWVersionStr[i] = STR_ADAPT(IT8951RawDeviceSystemInfo[6 + i]);
        }

        for(uint8_t i = 0; i < USDEF_CMD_GET_DEV_INFO_LUT_STR_WORDS; i++)
        {
            deviceSystemInfo.LUTVersionStr[i] = STR_ADAPT(IT8951RawDeviceSystemInfo[14 + i]);
        }

        LOG_INFO(MODULE, "Device Info: (" << deviceInfoRequestCount << " tries)");
        LOG_INFO(MODULE,
                 "panel " << deviceSystemInfo.panelWidth << "x" << deviceSystemInfo.panelHeight);
        LOG_INFO(MODULE, "imgBuff " << etl::showbase << etl::hex << deviceSystemInfo.imgBufAddr);
        LOG_INFO(MODULE, "jpegBuff " << etl::showbase << etl::hex << deviceSystemInfo.jpegBufAddr);
        LOG_INFO(MODULE,
                 "FW version " << reinterpret_cast<const char*>(deviceSystemInfo.FWVersionStr));
        LOG_INFO(MODULE,
                 "LUT version " << reinterpret_cast<const char*>(deviceSystemInfo.LUTVersionStr));

    } while(receiveError && (deviceInfoRequestCount < MAX_GET_DEV_INFO_ATTEMPTS));

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::getTemperature(IT8951Word& userSetTemperature,
                                                                 IT8951Word& realTemperature)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    userSetTemperature = 0;
    realTemperature = 0;

    IT8951Word command = FSET_TEMP_CMD_ARG_GET;
    err = sendCommandWithArgs(IT8951_TCON_FSET_TEMP_CMD,
                              &command,
                              sizeof(command) / sizeof(IT8951Word));

    if(ScreenDefs::ScreenErrCode::SUCCESS == err)
    {
        err = readData(&realTemperature, sizeof(realTemperature) / sizeof(IT8951Word));
    }

    if(ScreenDefs::ScreenErrCode::SUCCESS == err)
    {
        err = readData(&userSetTemperature, sizeof(userSetTemperature) / sizeof(IT8951Word));
    }

    return err;
}


ScreenDefs::ScreenErrCode IT8951ScreenController::initTemperature()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    uint16_t userTemp = 0;
    uint16_t realTemp = 0;

    err = getTemperature(userTemp, realTemp);

    printf("Display User temperature = %d, Real temperature = %d\n", userTemp, realTemp);

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::waitForHostReady()
{
    resetHostReadyState();

    if(m_gpioController.read(m_screenIO.hRdyPin) != IGPIO_STATE_HIGH)
    {
        if(!mHostReadySignal.wait(DelayMs(HOST_READY_TIMEOUT_MS)))
        {
            return ScreenDefs::ScreenErrCode::BUSY;
        }
    }

    return ScreenDefs::ScreenErrCode::SUCCESS;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::waitForDisplayReady()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;
    uint16_t timeout = 3000;
    IT8951Word lutafsr = 0;

    // Wait for LUT ready
    err = readReg(LUTAFSR_REG_ADDR, lutafsr);
    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    while((LUTAFSR_NOT_BUSY != lutafsr) && (timeout > 0))
    {
        osDelay(1);
        err = readReg(LUTAFSR_REG_ADDR, lutafsr);
        if(ScreenDefs::ScreenErrCode::SUCCESS != err)
        {
            break;
        }
        timeout--;
    }

    if((0 == timeout) && (LUTAFSR_NOT_BUSY != lutafsr))
    {
        err = ScreenDefs::ScreenErrCode::BUSY;
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::readData(IT8951Word data[], uint32_t wordCount)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    IT8951Word preamble = IT8951_PREAMBLE_WORD_READ;

    preamble = WORD_ADAPT(preamble);

    err = spiTransmit((uint8_t*) &preamble, 1, false);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return ScreenDefs::ScreenErrCode::NOT_RESPONDING;
    }

    // The first received word is a dummy word
    IT8951Word dummy = 0;

    err = spiReceive((uint8_t*) &dummy, 1, false);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return ScreenDefs::ScreenErrCode::NOT_RESPONDING;
    }

    for(uint16_t i = 0; i < wordCount; ++i)
    {
        err = spiReceive((uint8_t*) (data + i), 1, (wordCount - 1 == i));
    }

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return ScreenDefs::ScreenErrCode::NOT_RESPONDING;
    }

    // Convert from Big-Endian to Little-Endian
    for(uint32_t i = 0; i < wordCount; i++)
    {
        data[i] = WORD_ADAPT(data[i]);
    }


    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::spiTransmit(const uint8_t data[],
                                                              uint32_t wordCount,
                                                              bool bEndTransfer)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;
    ISPIErrCode spiErr = ISPI_ERROR_SUCCESS;

    err = waitForHostReady();

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    spiErr = m_iSPI.transmit(data, wordCount, m_csNSelector, bEndTransfer);

    if(ISPI_ERROR_SUCCESS != spiErr)
    {
        m_iSPI.endTransfer(m_csNSelector);
        return ScreenDefs::ScreenErrCode::NOT_RESPONDING;
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::spiReceive(uint8_t data[],
                                                             uint32_t wordCount,
                                                             bool bEndTransfer)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;
    ISPIErrCode spiErr = ISPI_ERROR_SUCCESS;


    err = waitForHostReady();

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    spiErr = m_iSPI.receive(data, wordCount, m_csNSelector, bEndTransfer, false);

    if(ISPI_ERROR_SUCCESS != spiErr)
    {
        m_iSPI.endTransfer(m_csNSelector);
        return ScreenDefs::ScreenErrCode::NOT_RESPONDING;
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::sendData(IT8951Word data[], uint32_t wordCount)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    IT8951Word preamble = IT8951_PREAMBLE_WORD_WRITE;

    preamble = WORD_ADAPT(preamble);

    err = spiTransmit((uint8_t*) &preamble, 1, false);
    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        m_iSPI.endTransfer(m_csNSelector);
        return ScreenDefs::ScreenErrCode::NOT_RESPONDING;
    }

    // Convert from Little-Endian to Big-Endian
    for(uint32_t i = 0; i < wordCount; i++)
    {
        data[i] = WORD_ADAPT(data[i]);
    }

    err = spiTransmit((uint8_t*) data, wordCount, true);
    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::writeReg(IT8951Word regAddr, IT8951Word regValue)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    err = sendCommand(IT8951_TCON_REG_WR_CMD);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    err = sendData(&regAddr, sizeof(regAddr) / sizeof(IT8951Word));

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    return sendData(&regValue, sizeof(regValue) / sizeof(IT8951Word));
}

ScreenDefs::ScreenErrCode IT8951ScreenController::readReg(IT8951Word regAddr, IT8951Word& regValue)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;
    IT8951Word readValue = 0;

    err = sendCommand(IT8951_TCON_REG_RD_CMD);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    err = sendData(&regAddr, sizeof(regAddr) / sizeof(IT8951Word));

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    err = readData(&readValue, sizeof(readValue) / sizeof(IT8951Word));

    if(ScreenDefs::ScreenErrCode::SUCCESS == err)
    {
        regValue = readValue;
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::readRegs(IT8951Word regBaseAddr,
                                                           etl::array_view<IT8951Word> readValues)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    IT8951Word regAddr = regBaseAddr;
    for(uint16_t i = 0; i < readValues.size(); i++, regAddr += 2)
    {
        err = readReg(regAddr, readValues[i]);
        if(ScreenDefs::ScreenErrCode::SUCCESS != err)
        {
            return err;
        }
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::sendCommand(IT8951Word command)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    IT8951Word preamble = IT8951_PREAMBLE_WORD_COMMAND;

    preamble = WORD_ADAPT(preamble);

    err = spiTransmit((uint8_t*) &preamble, 1, false);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        m_iSPI.endTransfer(m_csNSelector);
        return err;
    }

    command = WORD_ADAPT(command);

    err = spiTransmit((uint8_t*) &command, 1, true);

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::sendCommandWithArgs(IT8951Word command,
                                                                      IT8951Word args[],
                                                                      uint32_t argsWordCount)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    err = sendCommand(command);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    err = sendDataWaitHRDY(args, argsWordCount);

    return err;
}

IT8951DWord IT8951ScreenController::getRawBppFrameBuffAddrFromIndex(IScreen& screen,
                                                                    uint8_t frameBufferIndex)
{
    const uint32_t frameBufferSize = (screen.getScreenInfo().width * screen.getScreenInfo().height)
                                     + 8;

    if(frameBufferIndex > 0)
    {
        // Second frame buffer is reserved by IT8951 firmware, so start by next one
        frameBufferIndex++;
    }

    return m_deviceSystemInfo.imgBufAddr + (frameBufferIndex * frameBufferSize);
}

IT8951DWord IT8951ScreenController::getJpegFrameBuffAddrFromIndex(IScreen& screen,
                                                                  uint8_t frameBufferIndex)
{
    const uint32_t frameBufferSize = 0x40000;

    return m_deviceSystemInfo.jpegBufAddr + frameBufferIndex * frameBufferSize;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::display(
    IScreen& screen,
    IFrameBuffer& frameBuffer,
    const IScreenController::DisplayOptions& displayOptions)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::INVALID_PARAMETER;
    bool isLimitedRefreshEnabled = true;
    eDisplayMode displayMode = displayOptions.displayMode;

    if(displayMode == eDisplayMode::FAST)
    {
        ++mFastRefreshCount;
        if(mFastRefreshCount >= mCleanRefreshPeriod)
        {
            mFastRefreshCount = 0;
            displayMode = eDisplayMode::FULL;
        }
    }
    else
    {
        mFastRefreshCount = 0;
    }

    if(displayOptions.backgroundColor == ScreenDefs::BackgroundColor::BLACK)
    {
        ++mBlackBackgroundRefreshCount;
        if(mBlackBackgroundRefreshCount >= mBlackBackgroundCleanRefreshPeriod)
        {
            mBlackBackgroundRefreshCount = 0;
            mFastRefreshCount = 0;

            isLimitedRefreshEnabled = false;
            displayMode = eDisplayMode::FULL;
        }
    }
    else
    {
        mBlackBackgroundRefreshCount = 0;
    }

    setLimitedRefresh(isLimitedRefreshEnabled);

    uint8_t mode = DISPLAY_MODE_CLEAR;
    switch(frameBuffer.getContentType())
    {
    case Image::eImageType::_1BPP:
        mode = (displayMode == eDisplayMode::FAST ? DISPLAY_MODE_A2 : DISPLAY_MODE_GC16);
        err = display1Bpp(screen, frameBuffer.getStartAddress(), mode);
        break;
    case Image::eImageType::_2BPP:
        mode = (displayMode == eDisplayMode::FAST ? DISPLAY_MODE_A2 : DISPLAY_MODE_GC16);
        err = displayNBpp(screen, frameBuffer.getStartAddress(), mode);
        break;
    case Image::eImageType::JPEG:
        mode = (displayMode == eDisplayMode::FAST ? DISPLAY_MODE_DU4 : DISPLAY_MODE_GC16);
        err = displayJpeg(screen, frameBuffer.getStartAddress(), mode);

    default:
        break;
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::displayJpeg(IScreen& screen,
                                                              IT8951DWord frameBuffAddr,
                                                              uint8_t displayMode)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::UNEXPECTED;

    err = decodeJpeg(screen, frameBuffAddr);
    if(err == ScreenDefs::ScreenErrCode::SUCCESS)
    {
        err = displayNBpp(screen, getRawBppFrameBuffAddrFromIndex(screen, 0), displayMode);
    }

    return err;
}


ScreenDefs::ScreenErrCode IT8951ScreenController::displayNBpp(IScreen& screen,
                                                              IT8951DWord frameBuffAddr,
                                                              uint8_t displayMode)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    PACK_ALIGN((struct displayAreaParams
                {
                    IT8951Word x;
                    IT8951Word y;
                    IT8951Word width;
                    IT8951Word height;
                    IT8951Word displayMode;
                    IT8951Word frameBuffAddrL;
                    IT8951Word frameBuffAddrH;
                }),
               sizeof(IT8951Word));

    // The buffer address is 26-bits
    const IT8951Word frameBuffAddrH = (frameBuffAddr >> 16) & (0x0000FFFF);
    const IT8951Word frameBuffAddrL = frameBuffAddr & (0x0000FFFF);
    const IT8951Word x = 0;
    const IT8951Word y = 0;

    displayAreaParams params = {
        .x = x,
        .y = y,
        .width = screen.getScreenInfo().width,
        .height = screen.getScreenInfo().height,
        .displayMode = displayMode,
        .frameBuffAddrL = frameBuffAddrL,
        .frameBuffAddrH = frameBuffAddrH,
    };

    err = sendCommandWithArgs(USDEF_CMD_DPY_AREA_CMD_FB_ADDR,
                              reinterpret_cast<IT8951Word*>(&params),
                              sizeof(params) / sizeof(IT8951Word));
    if(ScreenDefs::ScreenErrCode::SUCCESS == err)
    {
        err = waitForDisplayReady();
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::display1Bpp(IScreen& screen,
                                                              IT8951DWord frameBuffAddr,
                                                              uint8_t displayMode)
{
    ScreenDefs::ScreenErrCode ret = ScreenDefs::ScreenErrCode::SUCCESS;

    const bool bInverted = false;
    const IT8951Word backGroundColor = bInverted ? WHITE_1BPP_COLOR : BLACK_1BPP_COLOR;
    const IT8951Word foreGroundColor = bInverted ? BLACK_1BPP_COLOR : WHITE_1BPP_COLOR;

    ret = enable1BppMode();

    if(ScreenDefs::ScreenErrCode::SUCCESS == ret)
    {
        setBitmapColorTable(backGroundColor, foreGroundColor);
    }

    if(ScreenDefs::ScreenErrCode::SUCCESS == ret)
    {
        ret = displayNBpp(screen, frameBuffAddr, displayMode);
    }

    /* Always disable 1bpp even when previous command failed */
    ret = disable1BppMode();

    return ret;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::enable1BppMode()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    // Enable 1 bpp mode: UP1SR[18] = 1 (i.e. (UP1SR + 2)[2] = 1)
    IT8951Word up1sr2 = 0;

    err = readReg(UP1SR2_REG_ADDR, up1sr2);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    up1sr2 |= (UP1SR2_1BPP_ENABLED << UP1SR2_1BPP_IDX2);

    err = writeReg(UP1SR2_REG_ADDR, up1sr2);

    readReg(UP1SR2_REG_ADDR, up1sr2);

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::disable1BppMode()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    // Disable 1 bpp mode: UP1SR[18] = 0 (i.e. (UP1SR + 2)[2] = 0)
    IT8951Word up1sr2 = 0;

    err = readReg(UP1SR2_REG_ADDR, up1sr2);
    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    up1sr2 &= ~(UP1SR2_1BPP_ENABLED << UP1SR2_1BPP_IDX2);

    err = writeReg(UP1SR2_REG_ADDR, up1sr2);
    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::decodeJpeg(IScreen& screen,
                                                             IT8951DWord jpegFrameBuffAddr)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    PACK_ALIGN((struct decodeJpegParams
                {
                    IT8951Word frameBuffAddrL;
                    IT8951Word frameBuffAddrH;
                    IT8951Word x;
                    IT8951Word y;
                    IT8951Word width;
                    IT8951Word height;
                    IT8951Word sizeBuffAddrL;
                    IT8951Word sizeBuffAddrH;
                }),
               sizeof(IT8951Word));

    const IT8951DWord sizeBuffer = screen.getScreenInfo().width * screen.getScreenInfo().height;
    const IT8951Word x = 0;
    const IT8951Word y = 0;

    // The buffer address is 26-bits
    IT8951Word frameBuffAddrH = (jpegFrameBuffAddr >> 16) & (0x0000FFFF);
    IT8951Word frameBuffAddrL = jpegFrameBuffAddr & (0x0000FFFF);
    IT8951Word sizeBuffAddrH = (sizeBuffer >> 16) & (0x0000FFFF);
    IT8951Word sizeBuffAddrL = sizeBuffer & (0x0000FFFF);

    decodeJpegParams params = {
        .frameBuffAddrL = frameBuffAddrL,
        .frameBuffAddrH = frameBuffAddrH,
        .x = x,
        .y = y,
        .width = screen.getScreenInfo().width,
        .height = screen.getScreenInfo().height,
        .sizeBuffAddrL = sizeBuffAddrL,
        .sizeBuffAddrH = sizeBuffAddrH,
    };

    err = sendCommandWithArgs(USDEF_CMD_GET_JPGDEC_CMD,
                              reinterpret_cast<IT8951Word*>(&params),
                              sizeof(params) / sizeof(IT8951Word));
    if(err == ScreenDefs::ScreenErrCode::SUCCESS)
    {
        err = waitForDisplayReady();
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::setBitmapColorTable(IT8951Word backgroundColor,
                                                                      IT8951Word foregroundColor)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;
    err = writeReg(BGVR_REG_ADDR,
                   (backgroundColor << BGVR_BG_IDX) | (foregroundColor << BGVR_FG_IDX));

    IT8951Word table = 0;
    readReg(BGVR_REG_ADDR, table);

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::clearScreen(IScreen& screen)
{
    // Disable reduced pixel update mode to make sure the screen is properly cleaned
    ScreenDefs::ScreenErrCode ret = setLimitedRefresh(false);

    // If the screen malfunctions, screen commands will take some time to
    // return and will fail with ScreenDefs::ScreenErrCode::BUSY.
    // This may lead to watchdog resets, so do not continue if setLimitedRefresh
    // fails.
    if(ret == ScreenDefs::ScreenErrCode::SUCCESS)
    {
        ret = clearContent(screen, true, true);
        setLimitedRefresh(true);
    }

    // As the screen is now clean, reset the refresh counters
    mFastRefreshCount = 0;
    mBlackBackgroundRefreshCount = 0;

    return ret;
}


ScreenDefs::ScreenErrCode IT8951ScreenController::wakeUp()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::UNEXPECTED;

    if(!m_bInitialized)
    {
        return ScreenDefs::ScreenErrCode::UNEXPECTED;
    }

    err = sendCommand(IT8951_TCON_SYS_RUN_CMD);
    if(ScreenDefs::ScreenErrCode::SUCCESS == err)
    {
        err = waitForHostReady();
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::goToLowPowerMode()
{
    if(!m_bInitialized)
    {
        return ScreenDefs::ScreenErrCode::UNEXPECTED;
    }

    return sendCommand(IT8951_TCON_SLEEP_CMD);
}

void IT8951ScreenController::holdReset()
{
    m_gpioController.clear(m_screenIO.rstNPin);

    m_iSPI.deinit();

    m_bInitialized = false;
    m_bFirstPowerUpByBootloader = false;
}

void IT8951ScreenController::setPower(bool powerOn)
{
    if(powerOn)
    {
        m_gpioController.clear(m_screenIO.rstNPin);
        osDelay(10);
        m_gpioController.set(m_screenIO.onPin);
        osDelay(10);

        m_gpioController.set(m_screenIO.rstNPin);
        osDelay(100);
    }
    else
    {
        holdReset();

        m_gpioController.clear(m_screenIO.rstNPin);
        m_gpioController.clear(m_screenIO.onPin);
    }
}

ScreenDefs::ScreenErrCode IT8951ScreenController::startSendImgData()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    IT8951Word preamble = IT8951_PREAMBLE_WORD_WRITE;

    preamble = WORD_ADAPT(preamble);

    err = spiTransmit((uint8_t*) &preamble, 1, false);

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::startMemBurst(IT8951DWord memAddr,
                                                                IT8951DWord writeSize)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    std::vector<IT8951Word> cmdArgs;

    cmdArgs.push_back(memAddr & 0x0000FFFF);           // addr[15:0]
    cmdArgs.push_back((memAddr >> 16) & 0x0000FFFF);   // addr[25:16]
    cmdArgs.push_back(writeSize & 0x0000FFFF);         // writeSize[15:0]
    cmdArgs.push_back((writeSize >> 16) & 0x0000FFFF); // writeSize[25:16]

    err = sendCommandWithArgs(IT8951_TCON_MEM_BST_WR_CMD, cmdArgs.data(), cmdArgs.size());

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::endSendBurst()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    err = sendCommand(IT8951_TCON_MEM_BST_END_CMD);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        return err;
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::enablePackedMode()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    err = writeReg(I80CPCR_REG_ADDR, I80CPCR_ENABLE_PACKED);
    IT8951Word readCPCR = 0;
    err = readReg(I80CPCR_REG_ADDR, readCPCR);

    return err;
}


IT8951Word IT8951ScreenController::reverseWord(IT8951Word word)
{
    return (BitReverseTable256[word & 0xff] << 8) | (BitReverseTable256[(word >> 8) & 0xff]);
}

ScreenDefs::ScreenErrCode IT8951ScreenController::sendDataWaitHRDY(IT8951Word data[],
                                                                   uint32_t wordCount)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    IT8951Word preamble = IT8951_PREAMBLE_WORD_WRITE;

    preamble = WORD_ADAPT(preamble);

    // Convert from Little-Endian to Big-Endian
    for(uint32_t i = 0; i < wordCount; i++)
    {
        data[i] = WORD_ADAPT(data[i]);
    }

    for(uint32_t i = 0; i < wordCount; i++)
    {
        err = spiTransmit((uint8_t*) &preamble, 1, false);

        if(ScreenDefs::ScreenErrCode::SUCCESS != err)
        {
            return ScreenDefs::ScreenErrCode::NOT_RESPONDING;
        }
        err = spiTransmit((uint8_t*) &(data[i]), 1, true);
        if(ScreenDefs::ScreenErrCode::SUCCESS != err)
        {
            break;
            ;
        }
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::doSendNBppImgData(IT8951Word data[],
                                                                    uint32_t wordCount)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    // Convert from Little-Endian to Big-Endian
    for(uint32_t i = 0; i < wordCount; i++)
    {
        data[i] = WORD_ADAPT_BPP_IMG(data[i]);
    }

    err = spiTransmit((uint8_t*) data, wordCount, false);

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::doSendJpegImgData(IT8951Word data[],
                                                                    uint32_t wordCount)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    // Convert from Little-Endian to Big-Endian
    for(uint32_t i = 0; i < wordCount; i++)
    {
        data[i] = WORD_ADAPT_JPEG_IMG(data[i]);
    }

    err = spiTransmit((uint8_t*) data, wordCount, false);

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::endSendImgData()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    if(ISPI_ERROR_SUCCESS != m_iSPI.endTransfer(m_csNSelector))
    {
        err = ScreenDefs::ScreenErrCode::NOT_RESPONDING;
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951ScreenController::clearContent(IScreen& screen,
                                                               bool clearBufferContent,
                                                               bool refreshScreen)
{
    ScreenDefs::ScreenErrCode ret = ScreenDefs::ScreenErrCode::UNEXPECTED;
    PACK_ALIGN((struct clearContentParams
                {
                    IT8951Word x;
                    IT8951Word y;
                    IT8951Word width;
                    IT8951Word height;
                    IT8951Word displayMode;
                    IT8951Word color;
                }),
               sizeof(IT8951Word));

    const bool WRITE_BUFFER = clearBufferContent;
    const bool REFRESH_SCREEN = refreshScreen;
    const IT8951Word WHITE_COLOR = 0x00FF;
    const ScreenDefs::Window window = {0,
                                       0,
                                       screen.getScreenInfo().width,
                                       screen.getScreenInfo().height};

    IT8951Word arg4DisplayMode = 0;
    arg4DisplayMode |= DISPLAY_MODE_CLEAR & 0x0F;
    arg4DisplayMode |= (WRITE_BUFFER ? FILL_RECT_CMD_DMODE_SET_FB_EN : FILL_RECT_CMD_DMODE_SET_FB_DIS)
                       << FILL_RECT_CMD_DMODE_SET_FB_IDX;
    arg4DisplayMode |=
        (REFRESH_SCREEN ? FILL_RECT_CMD_DMODE_REFRESH_EN : FILL_RECT_CMD_DMODE_REFRESH_DIS)
        << FILL_RECT_CMD_DMODE_REFRESH_IDX;

    clearContentParams params = {
        .x = window.x,
        .y = window.y,
        .width = window.w,
        .height = window.h,
        .displayMode = arg4DisplayMode,
        .color = WHITE_COLOR,
    };

    ret = sendCommandWithArgs(IT8951_TCON_FILL_RECT_CMD,
                              reinterpret_cast<IT8951Word*>(&params),
                              sizeof(params) / sizeof(IT8951Word));

    waitForDisplayReady();

    return ret;
}

void IT8951ScreenController::forceCleanAfterNFastDisplay(uint8_t displayCount)
{
    mCleanRefreshPeriod = displayCount;
}

void IT8951ScreenController::forceCleanAfterNBlackBackgroundDisplay(uint8_t displayCount)
{
    mBlackBackgroundCleanRefreshPeriod = displayCount;
}
