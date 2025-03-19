/*
 * DummyScreenController.hpp
 *
 *  Created on: 3 nov. 2021
 *      Author: thomas
 */

#ifndef NEMEIO_DISPLAY_DUMMYDISPLAY_DUMMYSCREENCONTROLLER_HPP_
#define NEMEIO_DISPLAY_DUMMYDISPLAY_DUMMYSCREENCONTROLLER_HPP_

#include "IScreenController.hpp"
#include "DummyFrameBuffer.hpp"

class DummyScreenController : public IScreenController
{
public:
    DummyScreenController() = default;
    virtual ~DummyScreenController() = default;

    ScreenDefs::ScreenErrCode init() final;
    etl::optional<etl::reference_wrapper<IScreen>> getScreen() final;

    IFrameBuffer& getFrameBuffer(uint8_t frameBufferIndex, Image::eImageType type) final;

    ScreenDefs::ScreenErrCode display(IScreen& screen,
                                      IFrameBuffer& frameBuffer,
                                      const DisplayOptions& displayOptions) final;
    ScreenDefs::ScreenErrCode display(IScreen& screen,
                                      uint8_t frameBufferIndex,
                                      const DisplayOptions& displayOptions) final;
    ScreenDefs::ScreenErrCode clearScreen(IScreen& screen) final;

    void setPower(bool powerOn) final;
    void holdReset() final;

    void forceCleanAfterNFastDisplay(uint8_t displayCount) final;
    void forceCleanAfterNBlackBackgroundDisplay(uint8_t displayCount) final;

    ScreenDefs::ScreenErrCode getVersions(char* firmwareVersionBuffer,
                                          std::size_t firmwareVersionBufferLen,
                                          char* lutVersionBuffer,
                                          std::size_t lutVersionBufferLen) const final;

private:
    DummyFrameBuffer mDummyFrameBuffer;
};

#endif /* NEMEIO_DISPLAY_DUMMYDISPLAY_DUMMYSCREENCONTROLLER_HPP_ */
