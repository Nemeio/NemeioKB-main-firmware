/*
 * IT8951Assembly.hpp
 *
 *  Created on: 2 nov. 2021
 *      Author: thomas
 */

#ifndef DISPLAY_IT8951_IT8951ASSEMBLY_HPP_
#define DISPLAY_IT8951_IT8951ASSEMBLY_HPP_

#include "IT8951FrameBuffers.hpp"
#include "IT8951FrameBuffers.hpp"
#include "IT8951ScreenController.hpp"
#include "ScreenFactory.hpp"
#include "Module.hpp"

class IT8951Assembly : public IScreenController, public Module<Identification::ModuleId::DISPLAY>
{
public:
    IT8951Assembly(iSPI& iSPI,
                   SingleChipSelector& chipSelector,
                   IGPIOController& gpioController,
                   IT8951ScreenController::IT8951IO& screenIO,
                   STM32Watchdog& watchdog);
    virtual ~IT8951Assembly() = default;

    /* IScreenController */
    ScreenDefs::ScreenErrCode init() final;
    etl::optional<etl::reference_wrapper<IScreen>> getScreen() final;

    ScreenDefs::ScreenErrCode getVersions(char* firmwareVersionBuffer,
                                          std::size_t firmwareVersionBufferLen,
                                          char* lutVersionBuffer,
                                          std::size_t lutVersionBufferLen) const final;

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

private:
    IT8951ScreenController mScreenController;
    etl::optional<etl::reference_wrapper<IScreen>> mScreen = etl::nullopt;

    IT8951FrameBuffers mFrameBuffers;
    ScreenFactory mScreenFactory;

    IFrameBuffer& getGeneratedFrameBufferFromIndex(uint8_t frameBufferIndex);
};

#endif /* DISPLAY_IT8951_IT8951ASSEMBLY_HPP_ */
