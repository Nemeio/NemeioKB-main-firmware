/*
 * ScreenDefs.hpp
 *
 *  Created on: Nov 4, 2019
 *      Author: Labo
 */

#ifndef DISPLAY_SCREEN_SCREENDEFS_HPP_
#define DISPLAY_SCREEN_SCREENDEFS_HPP_

#include <cstdint>

namespace ScreenDefs
{
enum class ScreenErrCode : uint8_t
{
    SUCCESS = 0,
    UNEXPECTED,
    NOT_RESPONDING,
    BUSY,
    INVALID_PARAMETER,
    INCOMPATIBLE_HARDWARE,
    READ_DATA_FAILED,
    INVALID_STATE,
    UPGRADE_FAILED
};

struct ScreenInfo
{
    uint16_t width;
    uint16_t height;

    bool operator==(const ScreenInfo& screen) const
    {
        return (screen.width == width && screen.height == height);
    }
};

enum class PixelFormat : uint8_t
{
    _UNKNOWN = 0,
    _1BPP = 1,
    _2BPP = 2,
    _4BPP = 4,
    _8BPP = 8,
    _JPEG = 255,
};

struct Window
{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;

    Window(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h)
        : x(_x)
        , y(_y)
        , w(_w)
        , h(_h){};

    Window()
        : x(0)
        , y(0)
        , w(0)
        , h(0){};
};

struct DrawArgs
{
    ScreenDefs::Window window;
    const uint8_t* pData;
    std::size_t len;

    DrawArgs(const ScreenDefs::Window& _window, const uint8_t* _pData, std::size_t _len)
        : window(_window)
        , pData(_pData)
        , len(_len)
    {
    }

    DrawArgs()
        : window()
        , pData(nullptr)
        , len(0)
    {
    }
};

enum class BackgroundColor
{
    UNKNOWN = 0,
    WHITE,
    BLACK,
    MAX
};
} // namespace ScreenDefs

#endif /* DISPLAY_SCREEN_SCREENDEFS_HPP_ */
