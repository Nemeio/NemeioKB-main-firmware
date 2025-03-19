/*
 * IT8951Assembly.cpp
 *
 *  Created on: 2 nov. 2021
 *      Author: thomas
 */

#include <IT8951/IT8951Assembly.hpp>
#include "DummyScreen.hpp"
#include "TraceLogger.hpp"
#include "TechnicalErrorDefs.hpp"

#define MODULE "it8951"

IT8951Assembly::IT8951Assembly(iSPI& iSPI,
                               SingleChipSelector& chipSelector,
                               IGPIOController& gpioController,
                               IT8951ScreenController::IT8951IO& screenIO,
                               STM32Watchdog& watchdog)
    : mScreenController(iSPI, chipSelector, gpioController, screenIO, watchdog)
    , mFrameBuffers(mScreenController)
{
}

IFrameBuffer& IT8951Assembly::getFrameBuffer(uint8_t frameBufferIndex, Image::eImageType type)
{
    DummyScreen dummy;
    std::reference_wrapper<IScreen> screen = dummy;
    if(mScreen.has_value())
    {
        screen = mScreen.value();
    }

    return mFrameBuffers.initFrameBuffer(screen, frameBufferIndex, type);
}

ScreenDefs::ScreenErrCode IT8951Assembly::init()
{
    ScreenDefs::ScreenErrCode ret = mScreenController.init();

    mScreen = mScreenFactory.makeScreen(*this, mScreenController.getScreenInfo());

    if(mScreen.has_value())
    {
        mScreenController.clearScreen(mScreen.value());
    }
    else
    {
        ret = ScreenDefs::ScreenErrCode::UNEXPECTED;
    }

    if(ret == ScreenDefs::ScreenErrCode::SUCCESS)
    {
        LOG_INFO(MODULE, "Init success");
    }
    else
    {
        LOG_ERR(MODULE,
                "Init error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                    static_cast<uint16_t>(ret)));
    }

    return ret;
}

etl::optional<etl::reference_wrapper<IScreen>> IT8951Assembly::getScreen()
{
    return mScreen;
}

ScreenDefs::ScreenErrCode IT8951Assembly::getVersions(char* firmwareVersionBuffer,
                                                      std::size_t firmwareVersionBufferLen,
                                                      char* lutVersionBuffer,
                                                      std::size_t lutVersionBufferLen) const
{
    return mScreenController.getVersions(firmwareVersionBuffer,
                                         firmwareVersionBufferLen,
                                         lutVersionBuffer,
                                         lutVersionBufferLen);
}

ScreenDefs::ScreenErrCode IT8951Assembly::display(IScreen& screen,
                                                  IFrameBuffer& frameBuffer,
                                                  const DisplayOptions& displayOptions)
{
    ScreenDefs::ScreenErrCode ret = mScreenController.display(screen, frameBuffer, displayOptions);

    if(ret != ScreenDefs::ScreenErrCode::SUCCESS)
    {
        LOG_ERR(MODULE,
                "Display error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                       static_cast<uint16_t>(ret)));
    }
    return ret;
}

ScreenDefs::ScreenErrCode IT8951Assembly::display(IScreen& screen,
                                                  uint8_t frameBufferIndex,
                                                  const DisplayOptions& displayOptions)
{
    ScreenDefs::ScreenErrCode ret = display(screen,
                                            mFrameBuffers.getFrameBuffer(frameBufferIndex),
                                            displayOptions);

    if(ret != ScreenDefs::ScreenErrCode::SUCCESS)
    {
        LOG_ERR(MODULE,
                "Display error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                       static_cast<uint16_t>(ret))
                                 << "on fb " << frameBufferIndex);
    }
    return ret;
}

ScreenDefs::ScreenErrCode IT8951Assembly::clearScreen(IScreen& screen)
{
    ScreenDefs::ScreenErrCode ret = mScreenController.clearScreen(mScreen.value());

    if(ret != ScreenDefs::ScreenErrCode::SUCCESS)
    {
        LOG_ERR(MODULE,
                "Clear error " << TechnicalErrorDefs::TechnicalError(getModuleId(),
                                                                     static_cast<uint16_t>(ret)));
    }
    return ret;
}

void IT8951Assembly::setPower(bool powerOn)
{
    mScreenController.setPower(powerOn);
}

void IT8951Assembly::holdReset()
{
    mScreenController.holdReset();
}

void IT8951Assembly::forceCleanAfterNFastDisplay(uint8_t displayCount)
{
    mScreenController.forceCleanAfterNFastDisplay(displayCount);
}

void IT8951Assembly::forceCleanAfterNBlackBackgroundDisplay(uint8_t displayCount)
{
    mScreenController.forceCleanAfterNBlackBackgroundDisplay(displayCount);
}