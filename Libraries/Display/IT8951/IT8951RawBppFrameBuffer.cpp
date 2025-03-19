/*
 * ITE8951RawBppFrameBuffer.cpp
 *
 *  Created on: 22 oct. 2021
 *      Author: thomas
 */

#include <IT8951/IT8951RawBppFrameBuffer.hpp>
#include <IT8951/IT8951ScreenControllerCommands.hpp>
#include "DummyScreen.hpp"
#include "utils.hpp"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "it8951_raw_fb"

IT8951RawBppFrameBuffer::IT8951RawBppFrameBuffer(IT8951ScreenController& screenController,
                                                 IT8951DWord frameBufferAddress,
                                                 ScreenDefs::ScreenInfo screenInfo)
    : mScreenController(screenController)
    , mFrameBufferAddress(reinterpret_cast<uint32_t>(frameBufferAddress))
    , mScreenInfo(screenInfo)
{
}

bool IT8951RawBppFrameBuffer::clearContent()
{
    DummyScreen dummyScreen(mScreenInfo);
    ScreenDefs::ScreenErrCode err = mScreenController.clearContent(dummyScreen, true);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        LOG_ERR(MODULE,
                "Clear content error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(err)));
    }

    return (err == ScreenDefs::ScreenErrCode::SUCCESS);
}

bool IT8951RawBppFrameBuffer::loadImage(Image& image)
{
    ScreenDefs::Window window = {0, 0, mScreenInfo.width, mScreenInfo.height};

    return loadImage(window, image);
}

uint32_t IT8951RawBppFrameBuffer::getStartAddress()
{
    return mFrameBufferAddress;
}

Image::eImageType IT8951RawBppFrameBuffer::getContentType()
{
    return mContentType;
}

bool IT8951RawBppFrameBuffer::loadImageNBpp(ScreenDefs::Window window,
                                            Image& image,
                                            uint8_t initialBitShift,
                                            uint8_t bpp)
{
    uint8_t bitShift = 0;
    uint32_t realLineLength = 0;
    uint32_t lineCopyBuffSizeBytes = 0;
    IT8951Word areaWidthWithPadding = 0;

    mScreenController.waitForDisplayReady();

    computePaddingParameters(window,
                             initialBitShift,
                             bpp,
                             realLineLength,
                             bitShift,
                             lineCopyBuffSizeBytes,
                             areaWidthWithPadding);
    window.w = areaWidthWithPadding;

    setImgBuffAddr(mFrameBufferAddress);

    ScreenDefs::ScreenErrCode err = sendData(window,
                                             image.getContent(),
                                             realLineLength,
                                             bitShift,
                                             lineCopyBuffSizeBytes);

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        LOG_ERR(MODULE,
                "Load image error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(err)));
    }

    return (err == ScreenDefs::ScreenErrCode::SUCCESS);
}

bool IT8951RawBppFrameBuffer::loadImage(ScreenDefs::Window window, Image& image)
{
    bool ret = false;
    uint8_t initialBitShift = 0;

    size_t imageSize;
    // Check if we are not out of screen dimensions or data buffer
    if(!image.getContent().getSize(imageSize) || window.x >= mScreenInfo.width
       || window.x + window.h > mScreenInfo.width || window.y >= mScreenInfo.height
       || window.y + window.h > mScreenInfo.height || (imageSize * 8) < (window.w * window.h))
    {
        return false;
    }
    /* TODO Use screenErrorFilter to catch screen issue */
    mContentType = image.getType();
    uint8_t bpp = imageTypeToBpp(mContentType);
    if(bpp == 1)
    {
        ret = adaptAreaInfo1Bpp(window, initialBitShift)
              && loadImageNBpp(window, image, initialBitShift, 8); /* Force 8bpp */
    }
    else
    {
        ret = loadImageNBpp(window, image, initialBitShift, bpp);
    }

    return ret;
}

ScreenDefs::ScreenErrCode IT8951RawBppFrameBuffer::doSendConstImgData(const IT8951Word data[],
                                                                      uint32_t dataLength,
                                                                      IT8951Word* copyBuff,
                                                                      uint32_t wordCount,
                                                                      uint8_t pixelShift)
{
    ASSERT(copyBuff != NULL);

    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    uint32_t buffSizeBytes = wordCount * sizeof(IT8951Word);

    memset(copyBuff, 0x00, buffSizeBytes);
    bitRightShiftInByteArray(reinterpret_cast<const uint8_t*>(data),
                             dataLength,
                             pixelShift,
                             reinterpret_cast<uint8_t*>(copyBuff),
                             buffSizeBytes,
                             IS_DEFAULT_PAD_BIT_VALUE_WHITE);

    err = mScreenController.doSendNBppImgData(copyBuff, wordCount);

    return err;
}

ScreenDefs::ScreenErrCode IT8951RawBppFrameBuffer::bppToPixelFormat(uint8_t bpp,
                                                                    uint8_t& it8951PixelFormatCode)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    switch(bpp)
    {
    case 2:
        it8951PixelFormatCode = IT8951_2BPP;
        break;
    case 4:
        it8951PixelFormatCode = IT8951_4BPP;
        break;
    case 1: /* Force 8Bpp for 1Bpp */
    case 8:
        it8951PixelFormatCode = IT8951_8BPP;
        break;
    default:
        err = ScreenDefs::ScreenErrCode::INVALID_PARAMETER;
        break;
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951RawBppFrameBuffer::startLoadImageArea(ScreenDefs::Window& window,
                                                                      uint8_t mBpp)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;
    PACK_ALIGN((struct loadImageAreaParams
                {
                    IT8951Word memoryConverterSettings;
                    IT8951Word x;
                    IT8951Word y;
                    IT8951Word width;
                    IT8951Word height;
                }),
               sizeof(IT8951Word));

    const uint8_t endianess = IT8951_LDIMG_B_ENDIAN;
    uint8_t pixelFormat = 0;

    err = bppToPixelFormat(mBpp, pixelFormat);

    loadImageAreaParams params = {
        .memoryConverterSettings = static_cast<IT8951Word>(endianess << 8 | pixelFormat << 4
                                                           | IT8951_ROTATE_0),
        .x = window.x,
        .y = window.y,
        .width = window.w,
        .height = window.h,
    };

    static_assert(sizeof(params) % 2 == 0);

    if(err == ScreenDefs::ScreenErrCode::SUCCESS)
    {
        err = mScreenController.sendCommandWithArgs(IT8951_TCON_LD_IMG_AREA_CMD,
                                                    reinterpret_cast<IT8951Word*>(&params),
                                                    sizeof(params) / sizeof(IT8951Word));
    }

    return err;
}

ScreenDefs::ScreenErrCode IT8951RawBppFrameBuffer::endLoadImageArea()
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;

    err = mScreenController.sendCommand(IT8951_TCON_LD_IMG_END_CMD);

    return err;
}

ScreenDefs::ScreenErrCode IT8951RawBppFrameBuffer::sendData(ScreenDefs::Window& window,
                                                            IDataReader& dataReader,
                                                            uint32_t lineLength,
                                                            uint8_t bitShift,
                                                            uint32_t lineCopyBuffSize)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::UNEXPECTED;

    volatile uint32_t frameBuffOffset = 0;
    IT8951Word* copyLineBuff = NULL;

    err = startLoadImageArea(window, imageTypeToBpp(mContentType));

    // Send image line after line (like demo code)
    copyLineBuff = reinterpret_cast<IT8951Word*>(pvPortMalloc(lineCopyBuffSize));

    mScreenController.startSendImgData();
    for(IT8951Word line = 0; line < window.h; line++)
    {
        uint32_t wordCount = lineCopyBuffSize / sizeof(IT8951Word);

        size_t receivedLen = 0;
        const uint8_t* srcBuff = dataReader.read(lineLength, receivedLen);
        if(nullptr == srcBuff || receivedLen != lineLength)
        {
            err = ScreenDefs::ScreenErrCode::READ_DATA_FAILED;
            break;
        }

        err = doSendConstImgData((IT8951Word*) srcBuff,
                                 lineLength,
                                 copyLineBuff,
                                 wordCount,
                                 bitShift);

        if(ScreenDefs::ScreenErrCode::SUCCESS != err)
        {
            break;
        }

        frameBuffOffset += lineLength;
    }


    mScreenController.endSendImgData();

    vPortFree(copyLineBuff);

    endLoadImageArea();

    return err;
}

void IT8951RawBppFrameBuffer::computePaddingParameters(ScreenDefs::Window& window,
                                                       uint8_t initialBitShift,
                                                       uint8_t bpp,
                                                       uint32_t& realLineLength,
                                                       uint8_t& bitShift,
                                                       uint32_t& lineCopyBuffSizeBytes,
                                                       IT8951Word& areaWidthWithPadding)
{
    ASSERT(initialBitShift < NB_BITS_IT8951_WORD);
    ASSERT(bpp > 0);

    uint8_t paddingBitsAtBufferStart = (window.x % (NB_BITS_IT8951_WORD / bpp)) * bpp;
    bitShift = initialBitShift + paddingBitsAtBufferStart;

    realLineLength = window.w * bpp / BITS_PER_BYTE;

    lineCopyBuffSizeBytes = computeLineCopyBufferSize(realLineLength, bitShift, bpp);

    // If the data had an initial bit shift (e.g. for 1bpp), the width must include this padding
    uint8_t initPaddingLengthBytes = divRoundUp(initialBitShift, BITS_PER_BYTE);
    areaWidthWithPadding = window.w + initPaddingLengthBytes;
}

bool IT8951RawBppFrameBuffer::adaptAreaInfo1Bpp(ScreenDefs::Window& window, uint8_t& pixelShift)
{
    if(window.w % BITS_PER_BYTE != 0)
    {
        return false;
    }

    // If x is not aligned on 8, padding pixels must be added
    // These pixels will be written to the screen
    pixelShift = window.x % BITS_PER_BYTE;

    // There is no 1BPP mode for IT8951 load image command: use 8BPP mode and divide the transfer size by 8
    window.x = window.x / 8;
    window.y = window.y;
    window.w = window.w / 8;
    window.h = window.h;

    return true;
}

uint32_t IT8951RawBppFrameBuffer::computeLineCopyBufferSize(uint32_t realLineLength,
                                                            uint8_t bitShift,
                                                            uint8_t bpp)
{
    ASSERT(bitShift < NB_BITS_IT8951_WORD);

    uint8_t paddingLengthBytes = divRoundUp(bitShift, BITS_PER_BYTE);
    uint32_t lineCopyBuffSizeBytes = realLineLength + paddingLengthBytes;

    // Buffer length must be aligned on IT8951 word size
    lineCopyBuffSizeBytes = divRoundUp(lineCopyBuffSizeBytes, sizeof(IT8951Word))
                            * sizeof(IT8951Word);

    return lineCopyBuffSizeBytes;
}

ScreenDefs::ScreenErrCode IT8951RawBppFrameBuffer::setImgBuffAddr(IT8951DWord imgBuffAddr)
{
    ScreenDefs::ScreenErrCode err = ScreenDefs::ScreenErrCode::SUCCESS;
    // The buffer address is 26-bits
    IT8951Word imgBuffAddrH = (imgBuffAddr >> 16) & (0x0000FFFF);
    IT8951Word imgBuffAddrL = imgBuffAddr & (0x0000FFFF);

    if(ScreenDefs::ScreenErrCode::SUCCESS == err)
    {
        err = mScreenController.writeReg(LISAR2_REG_ADDR, imgBuffAddrH);
    }


    if(ScreenDefs::ScreenErrCode::SUCCESS == err)
    {
        err = mScreenController.writeReg(LISAR_REG_ADDR, imgBuffAddrL);
    }

    return err;
}


uint8_t IT8951RawBppFrameBuffer::imageTypeToBpp(Image::eImageType imageType)
{
    uint8_t bpp = 0;

    switch(imageType)
    {
    case Image::eImageType::_1BPP:
        bpp = 1;
        break;
    case Image::eImageType::_2BPP:
        bpp = 2;
        break;
    default:
        break;
    }

    return bpp;
}
