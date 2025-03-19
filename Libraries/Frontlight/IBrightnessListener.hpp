#pragma once

#include <cstdint>

class IBrightnessListener
{
public:
    virtual void onBrightnessChanged(uint8_t brightnessPercent) = 0;
};