/*
 * IT8951FrameBuffer.hpp
 *
 *  Created on: 4 nov. 2021
 *      Author: thomas
 */

#ifndef DISPLAY_IT8951_IT8951FRAMEBUFFERS_HPP_
#define DISPLAY_IT8951_IT8951FRAMEBUFFERS_HPP_

#include <IT8951/IT8951RawBppFrameBuffer.hpp>
#include "IT8951JpegFrameBuffer.hpp"
#include "etl/variant.h"
#include <array>
#include "DummyFrameBuffer.hpp"

class IT8951FrameBuffers
{
public:
    explicit IT8951FrameBuffers(IT8951ScreenController& screenController);
    virtual ~IT8951FrameBuffers() = default;

    IFrameBuffer& initFrameBuffer(IScreen& screen, uint8_t frameBufferIndex, Image::eImageType type);
    IFrameBuffer& getFrameBuffer(uint8_t frameBufferIndex);

private:
    IT8951ScreenController& mScreenController;

    /* We can't use jpeg and raw buffer at the same time */
    using IT8951FrameBuffer =
        etl::variant<DummyFrameBuffer, IT8951RawBppFrameBuffer, IT8951JpegFrameBuffer>;
    std::array<IT8951FrameBuffer, 6> mFrameBuffers;
};

#endif /* DISPLAY_IT8951_IT8951FRAMEBUFFERS_HPP_ */
