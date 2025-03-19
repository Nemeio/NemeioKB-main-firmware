/*
 * DummyFrameBuffer.hpp
 *
 *  Created on: 3 nov. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DISPLAY_DUMMYDISPLAY_DUMMYFRAMEBUFFER_HPP_
#define NEMEIO_DISPLAY_DUMMYDISPLAY_DUMMYFRAMEBUFFER_HPP_

#include "IFrameBuffer.hpp"

class DummyFrameBuffer : public IFrameBuffer
{
public:
    DummyFrameBuffer() = default;
    virtual ~DummyFrameBuffer() = default;

    bool clearContent() final;
    bool loadImage(Image& image) final;
    bool loadImage(ScreenDefs::Window window, Image& image) final;
    uint32_t getStartAddress() final;

    Image::eImageType getContentType() final;
};

#endif /* NEMEIO_DISPLAY_DUMMYDISPLAY_DUMMYFRAMEBUFFER_HPP_ */
