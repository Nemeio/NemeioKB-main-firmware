#pragma once

#include "etl/map.h"
#include "KeypadDefs.hpp"

struct ButtonID
{
    constexpr ButtonID(uint8_t px, uint8_t py)
        : x(px)
        , y(py)
    {
    }

    bool operator==(const ButtonID& button) const;

    bool toIndex(uint8_t& index) const;

    uint8_t x;
    uint8_t y;

    static constexpr uint8_t INDEX_COUNT = 82;
    static constexpr uint8_t KEYBOARD_BUTTON_MAX_INDEX = 79;
    static etl::map<uint8_t, uint8_t, INDEX_COUNT> ScanCodeToKeyIndex;
};