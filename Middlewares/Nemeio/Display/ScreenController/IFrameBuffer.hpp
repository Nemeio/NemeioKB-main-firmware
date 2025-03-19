/*
 * IFrameBuffer.hpp
 *
 *  Created on: 21 oct. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DISPLAY_SCREENCONTROLLER_IFRAMEBUFFER_HPP_
#define NEMEIO_DISPLAY_SCREENCONTROLLER_IFRAMEBUFFER_HPP_

#include "Image.hpp"
#include "ScreenDefs.hpp"

class IFrameBuffer
{
public:
    IFrameBuffer() = default;
    virtual ~IFrameBuffer() = default;

    virtual bool clearContent() = 0;
    virtual bool loadImage(Image& image) = 0;
    virtual bool loadImage(ScreenDefs::Window window, Image& image) = 0;
    virtual uint32_t getStartAddress() = 0;

    virtual Image::eImageType getContentType() = 0;
};

#endif /* NEMEIO_DISPLAY_SCREENCONTROLLER_IFRAMEBUFFER_HPP_ */
