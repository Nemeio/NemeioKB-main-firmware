/*
 * ITE8951RawBppFrameBuffer.hpp
 *
 *  Created on: 22 oct. 2021
 *      Author: thomas
 */

#ifndef DISPLAY_IT8951_IT8951RAWBPPFRAMEBUFFER_HPP_
#define DISPLAY_IT8951_IT8951RAWBPPFRAMEBUFFER_HPP_

#include <IT8951/IT8951ScreenController.hpp>
#include <IT8951/IT8951ScreenControllerTypes.hpp>
#include "IFrameBuffer.hpp"
#include "Module.hpp"

class IT8951RawBppFrameBuffer : public IFrameBuffer,
                                public Module<Identification::ModuleId::DISPLAY>
{
public:
    IT8951RawBppFrameBuffer(IT8951ScreenController& screenController,
                            IT8951DWord frameBufferAddress,
                            ScreenDefs::ScreenInfo screenInfo);
    virtual ~IT8951RawBppFrameBuffer() = default;

    bool clearContent() final;
    bool loadImage(Image& image) final;
    bool loadImage(ScreenDefs::Window window, Image& image) final;
    uint32_t getStartAddress() final;
    Image::eImageType getContentType() final;

private:
    static constexpr bool IS_DEFAULT_PAD_BIT_VALUE_WHITE = true;
    IT8951ScreenController& mScreenController;
    ScreenDefs::ScreenInfo mScreenInfo = {0, 0};
    Image::eImageType mContentType = Image::eImageType::NONE;
    uint32_t mFrameBufferAddress = 0;

    void computePaddingParameters(ScreenDefs::Window& window,
                                  uint8_t initialBitShift,
                                  uint8_t bpp,
                                  uint32_t& realLineLength,
                                  uint8_t& bitShift,
                                  uint32_t& lineCopyBuffSizeBytes,
                                  IT8951Word& areaWidthWithPadding);
    uint32_t computeLineCopyBufferSize(uint32_t realLineLength, uint8_t bitShift, uint8_t bpp);
    bool adaptAreaInfo1Bpp(ScreenDefs::Window& window, uint8_t& pixelShift);
    ScreenDefs::ScreenErrCode doSendConstImgData(const IT8951Word data[],
                                                 uint32_t dataLength,
                                                 IT8951Word* copyBuff,
                                                 uint32_t wordCount,
                                                 uint8_t pixelShift);
    ScreenDefs::ScreenErrCode bppToPixelFormat(uint8_t bpp, uint8_t& it8951PixelFormatCode);

    ScreenDefs::ScreenErrCode sendData(ScreenDefs::Window& window,
                                       IDataReader& dataReader,
                                       uint32_t lineLength,
                                       uint8_t bitShift,
                                       uint32_t lineCopyBuffSize);

    ScreenDefs::ScreenErrCode startLoadImageArea(ScreenDefs::Window& window, uint8_t mBpp);
    ScreenDefs::ScreenErrCode endLoadImageArea();

    bool loadImage1Bpp(ScreenDefs::Window window, Image& image, uint8_t initialBitShift);
    bool loadImageNBpp(ScreenDefs::Window window,
                       Image& image,
                       uint8_t initialBitShift,
                       uint8_t bpp);

    uint8_t imageTypeToBpp(Image::eImageType imageType);

    ScreenDefs::ScreenErrCode setImgBuffAddr(IT8951DWord imgBuffAddr);
};

#endif /* DISPLAY_IT8951_IT8951RAWBPPFRAMEBUFFER_HPP_ */
