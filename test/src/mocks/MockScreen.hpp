/*
 * MockAppCommTxListener.hpp
 *
 *  Created on: Dec 2, 2019
 *      Author: Labo
 */

#ifndef MOCKS_MOCKAPPCOMMTXLISTENER_HPP_
#define MOCKS_MOCKAPPCOMMTXLISTENER_HPP_

#include "gmock/gmock.h"
#include "IScreen.hpp"

class MockScreen : public IScreen
{
public:
    MOCK_METHOD(ScreenDefs::ScreenErrCode, init, (), (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode,
                draw,
                (const ScreenDefs::Window& window,
                 size_t dataLength,
                 IDataReader& dataReader,
                 ScreenDefs::PixelFormat pixelFormat,
                 uint8_t fb),
                (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode,
                refresh,
                (bool bCleanScreen, ScreenDefs::PixelFormat pixelFormat, uint8_t fb),
                (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode,
                refreshWindow,
                (const ScreenDefs::Window& window,
                 bool bInverted,
                 ScreenDefs::PixelFormat pixelFormat,
                 uint8_t fb),
                (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode,
                clearCompleteScreen,
                (bool bFastClear, bool bRefresh),
                (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode,
                clearWindow,
                (const ScreenDefs::Window& window),
                (override));
    MOCK_METHOD(ScreenDefs::ScreenBpp, getBpp, (), (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode, POWER_OFF, (), (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode, goToLowPowerMode, (), (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode, wakeUp, (), (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode,
                getVersions,
                (char* firmwareVersionBuffer,
                 std::size_t firmwareVersionBufferLen,
                 char* lutVersionBuffer,
                 std::size_t lutVersionBufferLen),
                (override));

    MOCK_METHOD(ScreenDefs::ScreenErrCode, onUpdateStarted, (), (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode, onUpdateEnded, (), (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode, holdReset, (), (override));
    MOCK_METHOD(ScreenDefs::ScreenErrCode,
                getMaxWindowSize,
                (ScreenDefs::Window & window),
                (override));
};

#endif /* MOCKS_MOCKAPPCOMMTXLISTENER_HPP_ */
