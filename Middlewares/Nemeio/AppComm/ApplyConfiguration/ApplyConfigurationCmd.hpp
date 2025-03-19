#pragma once

#include "AppCommCmd.hpp"
#include "IConfigurationSwitcher.hpp"

enum class AppErrCode;

class ApplyConfigurationCmd : public AppCommCmd
{
public:
    ApplyConfigurationCmd(IAppCommTxListener& appCommTxListener,
                          IConfigurationSwitcher& configurationSwitcher);
    virtual ~ApplyConfigurationCmd() = default;

    virtual void dataReceived(const uint8_t* pData, size_t dataLen);

private:
    IConfigurationSwitcher& m_configurationSwitcher;
};