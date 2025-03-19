/*
 * IScreenController.hpp
 *
 *  Created on: 20 oct. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DISPLAY_ISCREENCONTROLLER_HPP_
#define NEMEIO_DISPLAY_ISCREENCONTROLLER_HPP_

#include "etl/optional.h"
#include "etl/functional.h"

#include "ScreenDefs.hpp"
#include "IFrameBuffer.hpp"
#include "IScreenVersionsReader.hpp"

class IScreen;

enum eDisplayMode
{
    FULL,
    FAST,
};

class IScreenController : public IScreenVersionsReader
{
public:
    IScreenController() = default;
    virtual ~IScreenController() = default;

    virtual ScreenDefs::ScreenErrCode init() = 0;
    virtual etl::optional<etl::reference_wrapper<IScreen>> getScreen() = 0;

    /* We need to specify image type, because clearScreen will be done before sending the image. So we need to know the type before to execute the action on the correct buffer */
    virtual IFrameBuffer& getFrameBuffer(uint8_t frameBufferIndex, Image::eImageType type) = 0;

    struct DisplayOptions
    {
        eDisplayMode displayMode;
        ScreenDefs::BackgroundColor backgroundColor;
    };
    virtual ScreenDefs::ScreenErrCode display(IScreen& screen,
                                              IFrameBuffer& frameBuffer,
                                              const DisplayOptions& displayOptions) = 0;
    virtual ScreenDefs::ScreenErrCode display(IScreen& screen,
                                              uint8_t frameBufferIndex,
                                              const DisplayOptions& displayOptions) = 0;
    virtual ScreenDefs::ScreenErrCode clearScreen(IScreen& screen) = 0;

    virtual void setPower(bool powerOn) = 0;
    virtual void holdReset() = 0;

    virtual void forceCleanAfterNFastDisplay(uint8_t displayCount) = 0;
    virtual void forceCleanAfterNBlackBackgroundDisplay(uint8_t displayCount) = 0;
};


#endif /* NEMEIO_DISPLAY_ISCREENCONTROLLER_HPP_ */
