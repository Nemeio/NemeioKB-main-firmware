/*
 * DummyScreenController.cpp
 *
 *  Created on: 3 nov. 2021
 *      Author: thomas
 */

#include "DummyScreenController.hpp"
#include <cstring>

ScreenDefs::ScreenErrCode DummyScreenController::init()
{
    return ScreenDefs::ScreenErrCode::UNEXPECTED;
}

etl::optional<etl::reference_wrapper<IScreen>> DummyScreenController::getScreen()
{
    return etl::nullopt;
}

IFrameBuffer& DummyScreenController::getFrameBuffer(uint8_t frameBufferIndex, Image::eImageType type)
{
    return mDummyFrameBuffer;
}

ScreenDefs::ScreenErrCode DummyScreenController::display(IScreen& screen,
                                                         IFrameBuffer& frameBuffer,
                                                         const DisplayOptions& displayOptions)
{
    return ScreenDefs::ScreenErrCode::SUCCESS;
}

ScreenDefs::ScreenErrCode DummyScreenController::display(IScreen& screen,
                                                         uint8_t frameBufferIndex,
                                                         const DisplayOptions& displayOptions)
{
    return ScreenDefs::ScreenErrCode::SUCCESS;
}

ScreenDefs::ScreenErrCode DummyScreenController::clearScreen(IScreen& screen)
{
    return ScreenDefs::ScreenErrCode::SUCCESS;
}

void DummyScreenController::setPower(bool powerOn)
{
}

void DummyScreenController::holdReset()
{
}

void DummyScreenController::forceCleanAfterNFastDisplay(uint8_t displayCount)
{
}

void DummyScreenController::forceCleanAfterNBlackBackgroundDisplay(uint8_t displayCount)
{
}

ScreenDefs::ScreenErrCode DummyScreenController::getVersions(char* firmwareVersionBuffer,
                                                             std::size_t firmwareVersionBufferLen,
                                                             char* lutVersionBuffer,
                                                             std::size_t lutVersionBufferLen) const
{
    memset(firmwareVersionBuffer, 0, firmwareVersionBufferLen);
    memset(lutVersionBuffer, 0, lutVersionBufferLen);
    return ScreenDefs::ScreenErrCode::SUCCESS;
}
