#pragma once

#include "AssemblyHardware.h"
#include "RectangleDataReader.hpp"
#include "ScreenDefs.hpp"

namespace benchmark
{
class CheckerBoard
{
public:
    explicit CheckerBoard(AssemblyHardware& hardwareAssembly);
    void draw(bool isFirstColorWhite);

private:
    static const etl::array<etl::pair<Color, Color>, 2> k_colors;

    static const etl::array<ScreenDefs::Window, 40> k_rectangles;

    IScreen& m_screen;
    AbstractWatchdog& m_watchdog;
};

} // namespace benchmark