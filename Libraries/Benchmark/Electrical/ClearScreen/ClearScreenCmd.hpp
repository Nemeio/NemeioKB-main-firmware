#pragma once

#include "AppCommCmd.hpp"
#include "AssemblyHardware.h"

namespace benchmark
{
class ClearScreenCmd : public AppCommCmd
{
public:
    ClearScreenCmd(IAppCommTxListener& appCommTxListener, IScreen& screen);
    ~ClearScreenCmd() final = default;

private:
    IScreen& m_screen;

    void dataReceived(const uint8_t* pData, size_t dataLen) final;
};

} // namespace benchmark
