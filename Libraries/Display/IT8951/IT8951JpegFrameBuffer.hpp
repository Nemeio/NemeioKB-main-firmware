/*
 * IT8951JpegFrameBuffer.hpp
 *
 *  Created on: 17 nov. 2021
 *      Author: thomas
 */

#ifndef DISPLAY_IT8951_IT8951JPEGFRAMEBUFFER_HPP_
#define DISPLAY_IT8951_IT8951JPEGFRAMEBUFFER_HPP_

#include "IFrameBuffer.hpp"
#include "IT8951ScreenController.hpp"
#include <array>
#include "Module.hpp"

class IT8951JpegFrameBuffer : public IFrameBuffer, public Module<Identification::ModuleId::DISPLAY>
{
public:
    IT8951JpegFrameBuffer(IT8951ScreenController& screenController,
                          IT8951DWord frameBufferAddress,
                          ScreenDefs::ScreenInfo screenInfo);
    virtual ~IT8951JpegFrameBuffer() = default;

    bool clearContent() final;
    bool loadImage(Image& image) final;
    bool loadImage(ScreenDefs::Window window, Image& image) final;
    uint32_t getStartAddress() final;
    Image::eImageType getContentType() final;

private:
    IT8951ScreenController& mScreenController;
    ScreenDefs::ScreenInfo mScreenInfo = {0, 0};
    Image::eImageType mContentType = Image::eImageType::NONE;
    uint32_t mFrameBufferAddress = 0;
};

#endif /* DISPLAY_IT8951_IT8951JPEGFRAMEBUFFER_HPP_ */
