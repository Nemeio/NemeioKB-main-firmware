/*
 * IT8951FrameBuffer.cpp
 *
 *  Created on: 4 nov. 2021
 *      Author: thomas
 */

#include "IT8951FrameBuffers.hpp"
#include "utils.hpp"

IT8951FrameBuffers::IT8951FrameBuffers(IT8951ScreenController& screenController)
    : mScreenController(screenController)
{
}

IFrameBuffer& IT8951FrameBuffers::getFrameBuffer(uint8_t frameBufferIndex)
{
    ASSERT(frameBufferIndex < mFrameBuffers.size());
    return castToBase<IFrameBuffer>(mFrameBuffers[frameBufferIndex]);
}


IFrameBuffer& IT8951FrameBuffers::initFrameBuffer(IScreen& screen,
                                                  uint8_t frameBufferIndex,
                                                  Image::eImageType type)
{
    ASSERT(frameBufferIndex < mFrameBuffers.size());

    switch(type)
    {
    case Image::eImageType::_1BPP:
    case Image::eImageType::_2BPP:
    case Image::eImageType::_4BPP:
        mFrameBuffers[frameBufferIndex].emplace<IT8951RawBppFrameBuffer>(
            mScreenController,
            mScreenController.getRawBppFrameBuffAddrFromIndex(screen, frameBufferIndex),
            screen.getScreenInfo());
        break;
    case Image::eImageType::JPEG:
        mFrameBuffers[frameBufferIndex].emplace<IT8951JpegFrameBuffer>(
            mScreenController,
            mScreenController.getJpegFrameBuffAddrFromIndex(screen, frameBufferIndex),
            screen.getScreenInfo());
        break;
    default:
        ASSERT(false);
        break;
    }

    return getFrameBuffer(frameBufferIndex);
}
