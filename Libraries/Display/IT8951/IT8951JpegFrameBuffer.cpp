/*
 * IT8951JpegFrameBuffer.cpp
 *
 *  Created on: 17 nov. 2021
 *      Author: thomas
 */

#include <IT8951JpegFrameBuffer.hpp>
#include "DummyScreen.hpp"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "it8951_jpeg_fb"

IT8951JpegFrameBuffer::IT8951JpegFrameBuffer(IT8951ScreenController& screenController,
                                             IT8951DWord frameBufferAddress,
                                             ScreenDefs::ScreenInfo screenInfo)
    : mScreenController(screenController)
    , mFrameBufferAddress(reinterpret_cast<uint32_t>(frameBufferAddress))
    , mScreenInfo(screenInfo)
{
}

bool IT8951JpegFrameBuffer::clearContent()
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

bool IT8951JpegFrameBuffer::loadImage(Image& image)
{
    ScreenDefs::ScreenErrCode err;
    size_t receivedLen = 0;
    size_t maxReadSize = (image.getContent().getMaxReadSize()) / 2 * 2;
    size_t imageSize = 0;
    volatile size_t sendSize = 0;

    image.getContent().getSize(imageSize);
    if(imageSize % 2 != 0)
    { /* Image must be padded by desktop app */
        return false;
    }

    err = mScreenController.startMemBurst(mFrameBufferAddress, imageSize / 2);
    if(err == ScreenDefs::ScreenErrCode::SUCCESS)
    {
        err = mScreenController.startSendImgData();
    }

    if(err == ScreenDefs::ScreenErrCode::SUCCESS)
    {
        do
        {
            const uint8_t* srcBuff = image.getContent().read(maxReadSize, receivedLen);
            if(nullptr == srcBuff)
            {
                err = ScreenDefs::ScreenErrCode::READ_DATA_FAILED;
                break;
            }

            err = mScreenController.doSendJpegImgData((IT8951Word*) srcBuff, receivedLen / 2);

            if(ScreenDefs::ScreenErrCode::SUCCESS != err)
            {
                break;
            }

            sendSize += receivedLen;

        } while(sendSize < imageSize);
    }

    mScreenController.endSendImgData();
    mScreenController.endSendBurst();

    if(ScreenDefs::ScreenErrCode::SUCCESS != err)
    {
        LOG_ERR(MODULE,
                "Load image error "
                    << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                          static_cast<uint16_t>(err)));
    }

    return (err == ScreenDefs::ScreenErrCode::SUCCESS);
}

bool IT8951JpegFrameBuffer::loadImage(ScreenDefs::Window window, Image& image)
{
    return false;
}

uint32_t IT8951JpegFrameBuffer::getStartAddress()
{
    return mFrameBufferAddress;
}

Image::eImageType IT8951JpegFrameBuffer::getContentType()
{
    return Image::eImageType::JPEG;
}
